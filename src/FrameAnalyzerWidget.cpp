#include "FrameAnalyzerWidget.h"
#include "ui_FrameAnalyzerWidget.h"

FrameAnalyzerWidget::FrameAnalyzerWidget(taets::AnalysisToolController *_controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameAnalyzerWidget)
{
    ui->setupUi(this);

    m_analyzer = _controller->createAnalyzer<taets::FrameAnalyzer>();

    ui->frameSpinBox->setMaximum(m_analyzer->parent()->length() - 1);
    ui->sourcesSelection->setSources(m_analyzer->getSources());

    makeConnections();

    m_analyzer->resendSignals();
}

FrameAnalyzerWidget::~FrameAnalyzerWidget()
{
    delete m_analyzer;
    delete ui;
}

void FrameAnalyzerWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QWidget::changeEvent(event);
}

void FrameAnalyzerWidget::compareModeChanged(bool _checked)
{
    if(!_checked) return;

    taets::FrameAnalyzer::CompareMode compare_mode;

    //TODO: yeah, this is ugly. maybe there is another way?
    if(ui->modeNormalRadioButton->isChecked())
        compare_mode = taets::FrameAnalyzer::COMPARE_NORMAL;
    else if(ui->modeDifferenceRadioButton->isChecked())
        compare_mode = taets::FrameAnalyzer::COMPARE_DIFFERENCE;
    else if(ui->modeOverlayRadioButton->isChecked())
        compare_mode = taets::FrameAnalyzer::COMPARE_OVERLAY;
    else if(ui->modeSlideRadioButton->isChecked())
        compare_mode = taets::FrameAnalyzer::COMPARE_SCROLL;
    else
        throw std::runtime_error("this should never happen!");

    m_analyzer->setCompareMode(compare_mode);
}

void FrameAnalyzerWidget::makeConnections()
{
    connect(m_analyzer, &taets::FrameAnalyzer::frameChanged, ui->frameSpinBox, &QSpinBox::setValue);
    connect(ui->frameSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_analyzer, &taets::FrameAnalyzer::asyncSetFrame);

    connect(m_analyzer, &taets::FrameAnalyzer::syncedChanged, ui->syncButton, &QPushButton::setChecked);
    connect(ui->syncButton, &QPushButton::toggled, m_analyzer, &taets::FrameAnalyzer::setSynced);

    connect(m_analyzer, &taets::FrameAnalyzer::pictureChanged, ui->picturePreview, &FrameAnalyzerPicturePreview::setPixmap);
    connect(ui->picturePreview, &FrameAnalyzerPicturePreview::dragChanged, m_analyzer, &taets::FrameAnalyzer::setScroll);

    connect(ui->sourcesSelection, &SourcesSelectionWidget::selectedChanged, m_analyzer, &taets::FrameAnalyzer::setDataSeriesEnabled);

    connect(ui->modeNormalRadioButton, &QRadioButton::clicked, this, &FrameAnalyzerWidget::compareModeChanged);
    connect(ui->modeDifferenceRadioButton, &QRadioButton::clicked, this, &FrameAnalyzerWidget::compareModeChanged);
    connect(ui->modeOverlayRadioButton, &QRadioButton::clicked, this, &FrameAnalyzerWidget::compareModeChanged);
    connect(ui->modeSlideRadioButton, &QRadioButton::clicked, this, &FrameAnalyzerWidget::compareModeChanged);

    connect(ui->filterCheckBox, &QCheckBox::toggled, ui->picturePreview, &FrameAnalyzerPicturePreview::setSmooth);
    connect(ui->picturePreview, &FrameAnalyzerPicturePreview::smoothChanged, ui->filterCheckBox, &QCheckBox::setChecked);
    connect(ui->fitButton, &QPushButton::clicked, ui->picturePreview, &FrameAnalyzerPicturePreview::fit);

    connect(ui->togglePlayButton, &QPushButton::clicked, m_analyzer->timeManipulator(), &TimeManipulator::setPlaying);
    connect(m_analyzer->timeManipulator(), &TimeManipulator::playingChanged, ui->togglePlayButton, &QPushButton::setChecked);
    connect(ui->stopButton, &QPushButton::clicked, m_analyzer->timeManipulator(), &TimeManipulator::stop);
    connect(ui->stepButton, &QPushButton::clicked, m_analyzer->timeManipulator(), &TimeManipulator::step);
    connect(ui->stepReverseButton, &QPushButton::clicked, m_analyzer->timeManipulator(), &TimeManipulator::stepReverse);
    connect(ui->seekFirstButton, &QPushButton::clicked, m_analyzer->timeManipulator(), &TimeManipulator::seekFirst);
    connect(ui->seekLastButton, &QPushButton::clicked, m_analyzer->timeManipulator(), &TimeManipulator::seekLast);
}
