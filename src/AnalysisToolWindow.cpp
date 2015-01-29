#include "AnalysisToolWindow.h"
#include "ui_AnalysisToolWindow.h"

#include "ImageSeries.h"
#include "FloatSeries.h"
#include <FrameAnalyzer.h>
#include <FrameAnalyzerWidget.h>
#include "HistogramAnalyzerWidget.h"
#include "DiagramAnalyzerWidget.h"
#include "TableAnalyzerWidget.h"
#include "LanguageHandler.h"

#include <QSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>

AnalysisToolWindow::AnalysisToolWindow(const std::string & testdata_filename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisToolWindow)
{
    m_initialization_stage = 2;
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);

    ui->mdiArea->addAction(ui->newFrameAnalyzerAction);
    ui->mdiArea->addAction(ui->newHistogramAnalyzerAction);
    ui->mdiArea->addAction(ui->newDiagramAnalyzerAction);
    ui->mdiArea->addAction(ui->newTableAnalyzerAction);

    setAttribute(Qt::WA_DeleteOnClose, true);

    try {
        if(testdata_filename.empty()) {
            QString filename = QFileDialog::getOpenFileName(this, tr("Open Testdata"), QDir::currentPath(), tr("TAETS Testdata (*.dat)"));

            if(filename.isNull()) throw std::runtime_error("No file selected");

            m_test_data = taets::TestData::load(filename.toStdString());
        } else {
            m_test_data = taets::TestData::load(testdata_filename);
        }
    } catch(...) {
        QMessageBox::critical(this, tr("Error loading Testdata"), tr("No Testdata could be loaded."));
        m_test_data = new taets::TestData(1);
    }

    m_controller = new taets::AnalysisToolController(m_test_data);

    ui->frameSpinBox->setMaximum(m_controller->length() - 1);
    ui->frameSlider->setMaximum(m_controller->length() - 1);

    makeConnections();
}

AnalysisToolWindow::~AnalysisToolWindow()
{
    delete m_controller;
    delete m_test_data;
    delete ui;
}

void AnalysisToolWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    } else {
        QWidget::changeEvent(event);
    }
}

void AnalysisToolWindow::resizeEvent(QResizeEvent *event)
{
    if(m_initialization_stage == 0) {
        setStandardLayout();
        m_initialization_stage = -1; //initialized
    }
    if(m_initialization_stage > 0) {
        m_initialization_stage--;
    }
}

void AnalysisToolWindow::createFrameAnalyzer()
{
    QMdiSubWindow * window = ui->mdiArea->addSubWindow(new FrameAnalyzerWidget(m_controller));
    window->resize(1000, 500);
    window->show();
}

void AnalysisToolWindow::createHistogramAnalyzer()
{
    QMdiSubWindow *window = ui->mdiArea->addSubWindow(new HistogramAnalyzerWidget(m_controller));
    window->resize(500, 300);
    window->show();
}

void AnalysisToolWindow::createDiagramAnalyzer()
{
    QMdiSubWindow *window = ui->mdiArea->addSubWindow(new DiagramAnalyzerWidget(m_controller));
    window->resize(1000, 300);
    window->show();
}

void AnalysisToolWindow::createTableAnalyzer()
{
    QMdiSubWindow *window = ui->mdiArea->addSubWindow(new TableAnalyzerWidget(m_controller));
    window->resize(200, 130);
    window->show();
}

void AnalysisToolWindow::setStandardLayout()
{
    ui->mdiArea->closeAllSubWindows();

    QMdiSubWindow *frame_window = ui->mdiArea->addSubWindow(new FrameAnalyzerWidget(m_controller));
    QMdiSubWindow *histogram_window = ui->mdiArea->addSubWindow(new HistogramAnalyzerWidget(m_controller));
    QMdiSubWindow *diagram_window = ui->mdiArea->addSubWindow(new DiagramAnalyzerWidget(m_controller));
    QMdiSubWindow *table_window = ui->mdiArea->addSubWindow(new TableAnalyzerWidget(m_controller));

    QSize container = ui->mdiArea->size();

    frame_window->resize(container.width() - 500, container.height() - 300);
    frame_window->move(0, 0);

    histogram_window->resize(500, 300);
    histogram_window->move(container.width() - 500, 0);

    table_window->resize(500, container.height() - 300 - 300);
    table_window->move(container.width() - 500, 300);

    diagram_window->resize(container.width(), 300);
    diagram_window->move(0, container.height() - 300);

    frame_window->show();
    histogram_window->show();
    diagram_window->show();
    table_window->show();
}

void AnalysisToolWindow::makeConnections()
{
    connect(ui->closeAction, &QAction::triggered, this, &QMainWindow::close);

    connect(ui->newFrameAnalyzerAction, &QAction::triggered, this, &AnalysisToolWindow::createFrameAnalyzer);
    connect(ui->newHistogramAnalyzerAction, &QAction::triggered, this, &AnalysisToolWindow::createHistogramAnalyzer);
    connect(ui->newDiagramAnalyzerAction, &QAction::triggered, this, &AnalysisToolWindow::createDiagramAnalyzer);
    connect(ui->newTableAnalyzerAction, &QAction::triggered, this, &AnalysisToolWindow::createTableAnalyzer);

    connect(m_controller->timeManipulator(), &TimeManipulator::frameChanged, ui->frameSpinBox, &QSpinBox::setValue);
    connect(m_controller->timeManipulator(), &TimeManipulator::frameChanged, ui->frameSlider, &QSlider::setValue);
    connect(m_controller->timeManipulator(), &TimeManipulator::playingChanged, ui->togglePlayButton, &QPushButton::setChecked);

    connect(ui->togglePlayButton, &QPushButton::clicked, m_controller->timeManipulator(), &TimeManipulator::setPlaying);
    connect(ui->stopButton, &QPushButton::clicked, m_controller->timeManipulator(), &TimeManipulator::stop);
    connect(ui->seekFirstButton, &QPushButton::clicked, m_controller->timeManipulator(), &TimeManipulator::seekFirst);
    connect(ui->seekLastButton, &QPushButton::clicked, m_controller->timeManipulator(), &TimeManipulator::seekLast);
    connect(ui->stepButton, &QPushButton::clicked, m_controller->timeManipulator(), &TimeManipulator::step);
    connect(ui->stepReverseButton, &QPushButton::clicked, m_controller->timeManipulator(), &TimeManipulator::stepReverse);

    connect(ui->frameSlider, &QSlider::valueChanged, m_controller->timeManipulator(), &TimeManipulator::setFrame);
    connect(ui->frameSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_controller->timeManipulator(), &TimeManipulator::setFrame);

    connect(ui->syncToMasterFrameButton, &QPushButton::clicked, m_controller, &taets::AnalysisToolController::syncToMasterFrame);

    connect(ui->setStandardLayoutAction, &QAction::triggered, this, &AnalysisToolWindow::setStandardLayout);

    connect(ui->setLanguageEnglishAction, &QAction::triggered, []{ LanguageHandler::instance()->setLanguage(QLocale::English); });
    connect(ui->setLanguageGermanAction, &QAction::triggered, []{ LanguageHandler::instance()->setLanguage(QLocale::German); });
}
