#include "FrameAnalyzer.h"

#include <AnalysisToolController.h>
#include <ImageSeries.h>

namespace taets {

FrameAnalyzer::FrameAnalyzer(AnalysisToolController *_parent, QObject *parent) :
    Analyzer(_parent, parent)
{
    m_synced = true;
    m_compare_mode = COMPARE_NORMAL;
    m_relative_scroll = m_absolute_scroll = 0.0;

    m_time_manipulator = new TimeManipulator(_parent->length(), _parent->frameInterval());
    connect(m_time_manipulator, &TimeManipulator::frameChanged, this, &FrameAnalyzer::asyncSetFrame);

    updatePreviewFrame(_parent->frame(), true);
}

FrameAnalyzer::~FrameAnalyzer() {
    delete m_time_manipulator;
}

void FrameAnalyzer::resendSignals()
{
    emit syncedChanged(m_synced);
    updatePreviewFrame();
}

void FrameAnalyzer::onPreviewFrameChanged(unsigned int _frame)
{
    if(m_synced) {
        updatePreviewFrame(_frame);
    }
}

void FrameAnalyzer::onSyncToMasterFrame()
{
    setSynced(true);
}

void FrameAnalyzer::asyncSetFrame(unsigned int _frame)
{
    /* setFrame also gets called when ui Elements are updated. So we have to detect this circular
     * update early and refuse it to fix the analyzer disabling sync */
    if(m_frame == _frame) return;

    setSynced(false);
    updatePreviewFrame(_frame);
}

void FrameAnalyzer::setSynced(bool _synced)
{
    if(m_synced != _synced) {
        m_synced = _synced;
        emit syncedChanged(m_synced);
    }

    if(m_synced) {
        m_time_manipulator->setPlaying(false);
        updatePreviewFrame(parent()->frame());
    }
}

void FrameAnalyzer::setDataSeriesEnabled(DataSeries *_series, bool _enabled)
{
    if(typeid(*_series) != typeid(ImageSeries))
        throw std::invalid_argument("FrameAnalyzer only supports ImageSeries");

    ImageSeries *series = static_cast<ImageSeries*>(_series);

    if(_enabled) {
        if(std::count(m_enabled_series.begin(), m_enabled_series.end(), series) != 0)
            throw std::logic_error("data not consistent");

        m_enabled_series.push_back(series);
    } else {
        if(std::count(m_enabled_series.begin(), m_enabled_series.end(), series) != 1)
            throw std::logic_error("data not consistent");

        m_enabled_series.erase(std::find(m_enabled_series.begin(), m_enabled_series.end(), series));
    }

    updatePreviewFrame();
}

void FrameAnalyzer::setCompareMode(CompareMode _mode)
{
    if(m_compare_mode != _mode) {
        m_compare_mode = _mode;
        updatePreviewFrame();
    }
}

void FrameAnalyzer::setScroll(float _relative, float _absolute)
{
    if(m_relative_scroll != _relative || m_absolute_scroll != _absolute) {
        m_relative_scroll = _relative;
        m_absolute_scroll = _absolute;
        updatePreviewFrame();
    }
}

void FrameAnalyzer::updatePreviewFrame(unsigned int _frame, bool _force)
{
    if(!_force && m_frame == _frame) return;

    m_frame = _frame;
    emit frameChanged(m_frame);
    m_time_manipulator->setFrame(m_frame);

    cv::Mat output;

    switch(m_compare_mode)
    {
    case COMPARE_NORMAL:
        if(m_enabled_series.size() == 1)
            output = getCVMatFromEnabledSeries(0);
        break;

    case COMPARE_DIFFERENCE:
        if(m_enabled_series.size() == 2) {
            output = getCVMatFromEnabledSeries(0).clone();
            output -= getCVMatFromEnabledSeries(1);
        }
        break;

    case COMPARE_OVERLAY:
        if(m_enabled_series.size() == 1) {
            output = getCVMatFromEnabledSeries(0);
        } else if(m_enabled_series.size() > 1) {
            output = getCVMatFromEnabledSeries(0) * (1.0 - m_absolute_scroll);

            unsigned int i;
            for(i = 1; i < m_enabled_series.size(); i++) {
                output += getCVMatFromEnabledSeries(i) * m_absolute_scroll * (1 / (float)(m_enabled_series.size() - 1));
            }
        }
        break;

    case COMPARE_SCROLL:
        if(m_enabled_series.size() >= 2) {
            output = getCVMatFromEnabledSeries(0).clone();

            unsigned int i;
            for(i = 1; i < m_enabled_series.size(); i++) {
                int cols = (m_enabled_series.size() - i) * (m_relative_scroll * output.cols);

                //clamp number of columns to valid range
                cols = cols > output.cols ? output.cols : cols;

                cv::Mat input_c = getCVMatFromEnabledSeries(i).colRange(0, cols);
                cv::Mat output_c = output.colRange(0, cols);

                input_c.copyTo(output_c);
            }
        }
        break;
    }

    if(output.empty()) {
        //the compare mode is not valid
        emit pictureChanged(QPixmap());
    } else {
        //all fine, output contains a valid image
        emit pictureChanged(ImageDataHelper::createQPixmap(output));
    }
}

} //end namespace taets
