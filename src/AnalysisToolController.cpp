#include "AnalysisToolController.h"

#include <Analyzer.h>

namespace taets {

AnalysisToolController::AnalysisToolController(TestData *_test_data)
{
    m_test_data = _test_data;
    m_time_manipulator = new TimeManipulator(length(), frameInterval());

    connect(m_time_manipulator, &TimeManipulator::frameChanged, this, &AnalysisToolController::setFrame);
}

AnalysisToolController::~AnalysisToolController()
{
    delete m_time_manipulator;
}

void AnalysisToolController::syncToMasterFrame()
{
    emit syncedToMasterFrame();
}

void AnalysisToolController::setFrame(unsigned int _frame)
{
    emit previewFrameChanged(_frame);
}

} //end namespace taets
