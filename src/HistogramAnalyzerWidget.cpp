#include "HistogramAnalyzerWidget.h"
#include "ui_HistogramAnalyzerWidget.h"

HistogramAnalyzerWidget::HistogramAnalyzerWidget(taets::AnalysisToolController * _controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramAnalyzerWidget) {
    ui->setupUi(this);

    _analyzer = _controller->createAnalyzer<taets::HistogramAnalyzer>();

    ui->sourcesSelection->setSources(_analyzer->getSources());

    makeConnections();
}

HistogramAnalyzerWidget::~HistogramAnalyzerWidget() {
    delete _analyzer;
    delete ui;
}

void HistogramAnalyzerWidget::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QWidget::changeEvent(event);
}

void HistogramAnalyzerWidget::makeConnections() {
    connect(ui->sourcesSelection, &SourcesSelectionWidget::selectedChanged, _analyzer, &taets::HistogramAnalyzer::setDataSeriesEnabled);

    connect(_analyzer, &taets::HistogramAnalyzer::histogramsChanged, ui->histogramViewer, &HistogramViewer::setHistograms);

    connect(ui->redPushButton, &QPushButton::toggled, ui->histogramViewer, &HistogramViewer::setRedEnabled);
    connect(ui->greenPushButton, &QPushButton::toggled, ui->histogramViewer, &HistogramViewer::setGreenEnabled);
    connect(ui->bluePushButton, &QPushButton::toggled, ui->histogramViewer, &HistogramViewer::setBlueEnabled);
    connect(ui->luminancePushButton, &QPushButton::toggled, ui->histogramViewer, &HistogramViewer::setLuminanceEnabled);

    connect(ui->histogramViewer, &HistogramViewer::redEnabledChanged, ui->redPushButton, &QPushButton::setChecked);
    connect(ui->histogramViewer, &HistogramViewer::greenEnabledChanged, ui->greenPushButton, &QPushButton::setChecked);
    connect(ui->histogramViewer, &HistogramViewer::blueEnabledChanged, ui->bluePushButton, &QPushButton::setChecked);
    connect(ui->histogramViewer, &HistogramViewer::luminanceEnabledChanged, ui->luminancePushButton, &QPushButton::setChecked);

    connect(ui->logarithmicCheckBox, &QCheckBox::toggled, ui->histogramViewer, &HistogramViewer::setLogarithmic);
    connect(ui->histogramViewer, &HistogramViewer::logarithmicChanged, ui->logarithmicCheckBox, &QCheckBox::setChecked);
}
