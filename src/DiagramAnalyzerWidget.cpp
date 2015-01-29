#include "DiagramAnalyzerWidget.h"
#include "ui_DiagramAnalyzerWidget.h"

DiagramAnalyzerWidget::DiagramAnalyzerWidget(taets::AnalysisToolController *_parent, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagramAnalyzerWidget)
{
    ui->setupUi(this);

    _analyzer = _parent->createAnalyzer<taets::DiagramAnalyzer>();

    ui->sourcesSelection->setSources(_analyzer->getSources());
    ui->diagramViewer->setFrame(_analyzer->frame());

    makeConnections();
}

DiagramAnalyzerWidget::~DiagramAnalyzerWidget()
{
    delete _analyzer;
    delete ui;
}

void DiagramAnalyzerWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QWidget::changeEvent(event);
}

void DiagramAnalyzerWidget::makeConnections() {
    connect(ui->sourcesSelection, &SourcesSelectionWidget::selectedChanged, _analyzer, &taets::DiagramAnalyzer::setDataSeriesEnabled);
    connect(_analyzer, &taets::DiagramAnalyzer::seriesAdded, ui->sourcesSelection, &SourcesSelectionWidget::setColor);
    connect(_analyzer, &taets::DiagramAnalyzer::seriesRemoved, ui->sourcesSelection, &SourcesSelectionWidget::resetColor);

    connect(_analyzer, &taets::DiagramAnalyzer::seriesAdded, ui->diagramViewer, &DiagramViewer::addSeries);
    connect(_analyzer, &taets::DiagramAnalyzer::seriesRemoved, ui->diagramViewer, &DiagramViewer::removeSeries);
    connect(_analyzer, &taets::DiagramAnalyzer::frameChanged, ui->diagramViewer, &DiagramViewer::setFrame);

    connect(ui->resetViewportButton, &QPushButton::clicked, ui->diagramViewer, &DiagramViewer::resetViewport);
}
