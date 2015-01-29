#ifndef ANALYSISTOOLCONTROLLER_H
#define ANALYSISTOOLCONTROLLER_H

#include <QObject>
#include <list>
#include <TestData.h>
#include <TimeManipulator.h>

namespace taets { class Analyzer; }

namespace taets {

class AnalysisToolController : public QObject
{
    Q_OBJECT

public:
    AnalysisToolController(TestData *_test_data);

    ~AnalysisToolController();

    /**
     * Erstellt einen neuen Analyzer und fügt ihn dem Controller hinzu.
     * Der Typ muss eine abgeleitete Klasse von Analyzer sein.
     */
    template<class T>
    T *createAnalyzer() {
        T *analyzer = new T(this);
        connect(this, &AnalysisToolController::previewFrameChanged, analyzer, &T::onPreviewFrameChanged);
        connect(this, &AnalysisToolController::syncedToMasterFrame, analyzer, &T::onSyncToMasterFrame);
        return analyzer;
    }

    /**
     * Gibt Testdaten zurück
     */
    TestData *testData() {
        return m_test_data;
    }

    /**
     * Gibt den TimeManipulator zurück. Darf nur von der GUI verwendet werden, um die Widgets an die Signale und Slots zu binden.
     */
    TimeManipulator *timeManipulator() {
        return m_time_manipulator;
    }

    unsigned int length() {
        return m_test_data->length();
    }

    unsigned int frame() {
        return m_time_manipulator->frame();
    }

    unsigned int frameInterval() {
        return 40;
    }

signals:
    void previewFrameChanged(int _frame);
    void syncedToMasterFrame();

public slots:
    /**
     * Synchronisiere die graphische Benutzeroberfläche zu einem Frame
     */
    void syncToMasterFrame();

private slots:
    /**
     * Wird vom TimeManipulator aufgerufen, wenn sich der Frame geändert hat.
     */
    void setFrame(unsigned int _frame);

private:
    TestData *m_test_data;
    TimeManipulator *m_time_manipulator;
};

} //end namespace taets

#endif // ANALYSISTOOLCONTROLLER_H
