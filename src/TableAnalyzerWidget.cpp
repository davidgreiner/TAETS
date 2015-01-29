#include "TableAnalyzerWidget.h"
#include "ui_TableAnalyzerWidget.h"

TableAnalyzerWidget::TableAnalyzerWidget(taets::AnalysisToolController *_parent, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableAnalyzerWidget) {
    ui->setupUi(this);

    _analyzer = _parent->createAnalyzer<taets::TableAnalyzer>();

    buildDialog();
    makeConnections();
}

TableAnalyzerWidget::~TableAnalyzerWidget() {
    delete _selection_widget;
    delete _selection_dialog;
    delete _analyzer;
    delete ui;
}

void TableAnalyzerWidget::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QWidget::changeEvent(event);
}

void TableAnalyzerWidget::addSeries(taets::FloatSeries *series, std::string name) {
    QLabel *output = new QLabel();
    output->setAlignment(Qt::AlignRight);
    ui->tableLayout->addRow(name.c_str(), output);
    _output_labels[series] = output;
}

void TableAnalyzerWidget::removeSeries(taets::FloatSeries *series) {
    ui->tableLayout->labelForField(_output_labels[series])->deleteLater();
    _output_labels[series]->deleteLater();
    _output_labels.erase(_output_labels.find(series));
}

void TableAnalyzerWidget::updateSeriesValue(taets::FloatSeries *series, float value) {
    _output_labels[series]->setText(QString::number(value, 'f'));
}

void TableAnalyzerWidget::buildDialog() {
    _selection_dialog = new QDialog();
    _selection_dialog->setLayout(new QGridLayout());

    _selection_widget = new SourcesSelectionWidget();
    _selection_widget->setSources(_analyzer->getSources());
    _selection_dialog->layout()->addWidget(_selection_widget);
}

void TableAnalyzerWidget::makeConnections() {
    connect(_analyzer, &taets::TableAnalyzer::seriesAdded, this, &TableAnalyzerWidget::addSeries);
    connect(_analyzer, &taets::TableAnalyzer::seriesDeleted, this, &TableAnalyzerWidget::removeSeries);
    connect(_analyzer, &taets::TableAnalyzer::seriesValueChanged, this, &TableAnalyzerWidget::updateSeriesValue);

    connect(ui->selectSourcesButton, &QPushButton::clicked, _selection_dialog, &QDialog::open);

    connect(_selection_widget, &SourcesSelectionWidget::selectedChanged, _analyzer, &taets::TableAnalyzer::setDataSeriesEnabled);
}
