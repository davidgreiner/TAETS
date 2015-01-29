#include "GraphEditorWindow.h"
#include "ui_GraphEditorWindow.h"
#include "TemplateEditorWindow.h"
#include "LanguageHandler.h"
#include "AnalysisToolWindow.h"
#include "Graph.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QInputDialog>
#include <QMessageBox>

GraphEditorWindow::GraphEditorWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::GraphEditorWindow) {
    ui->setupUi(this);

    graphCanvas = ui->graphCanvas;

    makeConnections();
}

GraphEditorWindow::~GraphEditorWindow() {
    delete ui;
}

void GraphEditorWindow::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QWidget::changeEvent(event);
}

void GraphEditorWindow::newProject() {
    bool ok;
    int length = QInputDialog::getInt(this, tr("Project length"), tr("Please enter the project length in frames"), 100, 1, 250000, 1, &ok);
    if(ok) {
        graphCanvas->newGraph(new Graph(length));
    }
}

void GraphEditorWindow::loadProject(const QString &filename) {
    graphCanvas->newGraph(Graph::load(filename.toStdString()));
}

void GraphEditorWindow::saveProject(const QString &filename) {
    graphCanvas->currentGraph()->save(filename.toStdString());
}

void GraphEditorWindow::queryOutputFile() {
    QString file = QFileDialog::getSaveFileName(nullptr, tr("Set Output File"), QDir::currentPath(), tr("TAETS Test Data (*.dat)"));
    if(!file.isNull()) graphCanvas->currentGraph()->setOutputFilename(file.toStdString());
}

void GraphEditorWindow::compute() {
    if(graphCanvas->currentGraph()->outputFilename().empty()) {
        queryOutputFile();
    }
    try {
        ui->centralwidget->setEnabled(false);
        graphCanvas->currentGraph()->computeEverything();
        emit calculationFinished();
        AnalysisToolWindow * window = new AnalysisToolWindow(graphCanvas->currentGraph()->outputFilename());
        window->setWindowIcon(QIcon("res/icons/logo.png"));
        window->show();
        //TODO: Hide self
    } catch(std::exception & e) {
        QMessageBox::critical(this, tr("Computation failed"), QString::fromStdString(e.what()));
    }
	ui->centralwidget->setEnabled(true);
}

void GraphEditorWindow::selectedNodeChanged() {
    ui->nodeSettingsList->setSettings(graphCanvas->selectedNode());
}

void GraphEditorWindow::makeConnections() {
    connect(ui->actionNew_Project, &QAction::triggered, this, &GraphEditorWindow::newProject);

    connect(ui->actionOpen_Project, &QAction::triggered, [=] {
        QString file = QFileDialog::getOpenFileName(nullptr, tr("Load Project"), QString(), tr("Project Files (*.*)"));
        if(!file.isNull()) loadProject(file);
    });

    connect(ui->actionSave_Project, &QAction::triggered, [=] {
        QString file = QFileDialog::getSaveFileName(nullptr, tr("Save Project"), QString(), tr("Project Files (*.*)"));
        if(!file.isNull()) saveProject(file);
    });

    connect(ui->actionCompute, &QAction::triggered, this, &GraphEditorWindow::compute);

    connect(ui->actionSetProjectLength, &QAction::triggered, [=] {
        bool ok;
        int length = QInputDialog::getInt(this, tr("Project length"), tr("Please enter the project length in frames"), 100, 0, 250000, 1, &ok);
        if(ok) {
            graphCanvas->currentGraph()->setLength(length);
        }
    });

    connect(ui->actionSetComputationOutput, &QAction::triggered, this, &GraphEditorWindow::queryOutputFile);

    connect(ui->actionOpen_Inspector_Window, &QAction::triggered, [=] {
        AnalysisToolWindow * window = new AnalysisToolWindow(graphCanvas->currentGraph()->outputFilename());
        window->show();
    });

    connect(ui->actionOpenTemplateEditor, &QAction::triggered, [=] {
       TemplateEditorWindow *window = new TemplateEditorWindow();
       window->show();
    });

    connect(graphCanvas, &GraphCanvas::selectedNodeChanged, this, &GraphEditorWindow::selectedNodeChanged);

    connect(ui->actionSetLanguageEnglish, &QAction::triggered, []{ LanguageHandler::instance()->setLanguage(QLocale::English); });
    connect(ui->actionSetLanguageGerman, &QAction::triggered, []{ LanguageHandler::instance()->setLanguage(QLocale::German); });
}
