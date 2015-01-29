#include "BinaryNode.h"
#include "BinaryTemplate.h"
#include "ChoiceSetting.h"
#include "TemplateManager.h"
#include "TemplateEditorWindow.h"
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <thread>
#include "ImageSeries.h"
#include "OutputConnector.h"
#include "InputConnectorBase.h"
#include "InputConnector.h"
#include <stdexcept>
#include "ImageDataHelper.h"
#include "Graph.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#if defined _WIN32 || defined _WIN64
#include <windows.h>
#include <tchar.h>
#elif defined __unix__
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace taets {

BinaryNode::BinaryNode(BinaryTemplate *_tmpl, Graph * graph, unsigned int id)
    : ModifierNode(graph, id, _tmpl->name()), _template(_tmpl->derive()) {
    InputConnector<ImageSeries> * in1 = new InputConnector<ImageSeries>(this, tr("Input Video").toStdString());
    _inputConnectors.push_back(in1);
    OutputConnector<ImageSeries> * out1 = new OutputConnector<ImageSeries>(this, tr("Output Video").toStdString());
    _outputConnectors.push_back(out1);

    for(TemplateParameter * param : _template->parameters()) {
        _settings.push_back(param->toNodeSetting());
    }
}

void BinaryNode::compute() {
    std::string command = _template->toString();
    ImageSeries * series = static_cast<OutputConnector<ImageSeries> * >(inputConnectors()[0]->source())->data();
    // Note: Wir erwarten, wieder rohes YUV einlesen zu können.
    // Das ist nicht das normale Verhalten eines encoders
    // Möglicherweise müssen daher die Daten des encoders durch das OS weiter
    // gepiped werden. Dies müsste unterstützt werden durch das vorstellen von
    // /bin/sh (unix)
    // cmd /Q /C (windows)
    std::cout << "Executing: " << command;
    std::cout << " with " << (_template->yuv() ? "yuv" : "rgb") << " data ";
    std::cout << (_template->perFrame() ? "for each frame" : "for the whole video") << std::endl;
    ImageSeries * output = new ImageSeries(series->length(), series->width(), series->height());
    if(!_template->perFrame()) {
        try {
        processVideo(command, series, output);
        } catch (std::exception e) {
            delete output;
            throw e;
        }
    } else {
        ImageSeries * buffer = new ImageSeries(1, series->width(), series->height());
        cv::Mat m = buffer->itemAsCVMatrix(0);

        for(int i = 0; i < series->length(); i++) {
            cv::Mat m2 = series->itemAsCVMatrix(i);
            m2.copyTo(m);
            try {
                processVideo(command, buffer, buffer);
            } catch (std::exception e) {
                delete output;
                delete buffer;
                throw e;
            }
            cv::Mat m3 = output->itemAsCVMatrix(i);       
            m.copyTo(m3);
        }

        delete buffer;
    }
    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(output);
}

void BinaryNode::processVideo(std::string command, ImageSeries * series, ImageSeries * output) {
#if defined _WIN32 || defined _WIN64
    SECURITY_ATTRIBUTES sAttr;
    sAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    sAttr.bInheritHandle = TRUE; // Pipe handles werden vererbt
    sAttr.lpSecurityDescriptor = NULL;

    HANDLE stdin_read = NULL;
    HANDLE stdin_write = NULL;
    HANDLE stdout_read = NULL;
    HANDLE stdout_write = NULL;

    // stdout Pipe erstellen
    if(!CreatePipe(&stdout_read, &stdout_write, &sAttr, 0)) {
        throw std::runtime_error("Fehler beim erstellen der pipe");
    }

    // Nicht aus stdout lesen
    if(!SetHandleInformation(stdout_read, HANDLE_FLAG_INHERIT, 0)) {
        throw std::runtime_error("Fehler beim setzen der pipe-flags");
    }

    // stdin Pipe erstellen
    if(!CreatePipe(&stdin_read, &stdin_write, &sAttr, 0)) {
        throw std::runtime_error("Fehler beim erstellen der pipe");
    }

    // Nicht in stdin schreiben
    if(!SetHandleInformation(stdin_write, HANDLE_FLAG_INHERIT, 0)) {
        throw std::runtime_error("Fehler beim setzen der pipe-flags");
    }

    PROCESS_INFORMATION procInfo;
    STARTUPINFO startInfo;
    bool success = FALSE;

    ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&startInfo, sizeof(STARTUPINFO));
    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.hStdError = stdout_write;
    startInfo.hStdOutput = stdout_write;
    startInfo.hStdInput = stdin_read;
    startInfo.dwFlags |= STARTF_USESTDHANDLES;

    std::wstring wide_command;
    wide_command.assign(command.begin(), command.end());
    // Kindprozess erstellen
    success = CreateProcess(
                NULL,
                (LPWSTR) wide_command.c_str(),    // Kommando
                NULL,               // Sicherheitsattribute
                NULL,               // Hauptthread-Sicherheitsattribute
                TRUE,               // Handles erben
                0,                  // Flags
                NULL,               // Umgebung des parent benutzen
                NULL,               // Ordner des parent benutzen
                &startInfo,
                &procInfo);

    if(!success) {
        throw std::runtime_error("Fehler beim Aufruf des encoders");
    }

    // Video schreiben
    DWORD framesize = series->width() * series->height() * sizeof(Color);
    Color * buffer = new Color[series->width()*series->height()];
    for(int i=0; i<series->length(); i++) {
        cv::Mat frame = series->itemAsCVMatrix(i);
        DWORD written;
        cv::Mat yuvFrame = ImageDataHelper::createCVMatrix(series->width(), series->height(), buffer);
        cvtColor(frame, yuvFrame, cv::COLOR_RGB2YCrCb, 3);

        success = WriteFile(stdin_write, yuvFrame.data, framesize, &written, NULL);
        if(!success) {
            throw std::runtime_error("I/O Fehler beim schreiben an den encoder");
        }
    }

    ImageSeries * output = new ImageSeries(graph()->length(), series->width(), series->height());
    for(int i=0; i<output->length(); i++) {
        DWORD bytesread;
        cv::Mat frame = output->itemAsCVMatrix(i);

        success = ReadFile(stdout_read, frame.data, framesize, &bytesread, NULL);
        if(!success) {
            throw std::runtime_error("I/O Fehler beim lesen aus dem encoder");
        }

        cvtColor(frame, frame, cv::COLOR_YCrCb2RGB, 3);
    }
    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);

    CloseHandle(stdout_read);
    CloseHandle(stdout_write);
    CloseHandle(stdin_read);
    CloseHandle(stdin_write);

    static_cast<OutputConnector<ImageSeries> * >(outputConnectors()[0])->setData(output);
#elif defined(__unix__) || defined(__APPLE__)
#define PIPE_READ 0
#define PIPE_WRITE 1
    int stdin_pipe[2];
    int stdout_pipe[2];
    int childpid;

    if(pipe(stdin_pipe) < 0) {
        throw std::runtime_error("Fehler beim erstellen der encoder-Eingabepipe");
    }
    if(pipe(stdout_pipe) < 0) {
        throw std::runtime_error("Fehler beim erstellen der encoder-Ausgabepipe");
    }

    childpid = fork();
    if(childpid == 0) {
        // Dies ist das Kind
        if(dup2(stdin_pipe[PIPE_READ], STDIN_FILENO) == -1) {
            // Exceptions werfen bringt hier nichts!
            exit(1);
        }
        if(dup2(stdout_pipe[PIPE_WRITE], STDOUT_FILENO) == -1) {
            exit(1);
        }

        close(stdin_pipe[PIPE_WRITE]);
        close(stdout_pipe[PIPE_READ]);

        execl("/bin/sh", "sh", "-c", command.c_str(), (char *)0);
        std::cerr << "Something went wrong" << std::endl;

        // Wenn wir hierher kommen, ist was kaputt.
        exit(1);
    } else if(childpid > 0) {
        // Hier ist der parent
        close(stdin_pipe[PIPE_READ]);
        close(stdout_pipe[PIPE_WRITE]);

        // Video konvertieren
        int pixels = series->width() * series->height();
        int rgbsize = pixels * 3;
        int yuvsize = pixels + pixels / 2; // wow such yuv much small

        if(_template->yuv()) {
            std::thread t1([=]() {
                uchar buffer[yuvsize];
                for(int i = 0; i < series->length(); i++) {
                    cv::Mat in = series->itemAsCVMatrix(i);
                    ImageDataHelper::RGBtoYUV420p(series->width(), series->height(), in, buffer);
                    int written = 0;
                    while(written < yuvsize) {
                        written += write(stdin_pipe[PIPE_WRITE], buffer + written, yuvsize - written);
                    }
                }
                close(stdin_pipe[PIPE_WRITE]);
            });
            std::thread t2([=]() {
                uchar buffer[yuvsize];
                for(int i = 0; i < output->length(); i++) {
                    cv::Mat out = output->itemAsCVMatrix(i);
                    int readed = 0; // deal with it.
                    while(readed < yuvsize) {
                        readed += read(stdout_pipe[PIPE_READ], buffer + readed, yuvsize - readed);
                    }
                    ImageDataHelper::YUV420ptoRGB(output->width(), output->height(), out, buffer);
                }
                close(stdout_pipe[PIPE_READ]);
            });
            t1.join();
            t2.join();
        } else {
            std::thread t1([=]() {
                for(int i = 0; i < series->length(); i++) {
                    cv::Mat in = series->itemAsCVMatrix(i);
                    int written = 0;
                    while(written < rgbsize) {
                        written += write(stdin_pipe[PIPE_WRITE], in.data + written, rgbsize - written);
                    }
                }
                close(stdin_pipe[PIPE_WRITE]);
            });
            std::thread t2([=]() {
                for(int i = 0; i < output->length(); i++) {
                    cv::Mat out = output->itemAsCVMatrix(i);
                    int readed = 0; // deal with it.
                    while(readed < rgbsize) {
                        readed += read(stdout_pipe[PIPE_READ], out.data + readed, rgbsize - readed);
                    }
                }
                close(stdout_pipe[PIPE_READ]);
            });
            t1.join();
            t2.join();
        }

        int status;
        waitpid(childpid, &status, 0);
        if(status != 0)
            throw std::runtime_error(tr("Error while running binary").toStdString());
    } else {
        close(stdin_pipe[PIPE_READ]);
        close(stdin_pipe[PIPE_WRITE]);
        close(stdout_pipe[PIPE_READ]);
        close(stdout_pipe[PIPE_WRITE]);
        throw std::runtime_error("Fork fehlgeschlagen");
    }
#endif

}

} // namespace taets
