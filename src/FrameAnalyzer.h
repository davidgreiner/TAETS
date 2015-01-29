#ifndef FRAMEANALYZER_H
#define FRAMEANALYZER_H

#include <Analyzer.h>
#include <ImageSeries.h>

namespace taets {

class FrameAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    enum CompareMode
    {
        COMPARE_NORMAL,
        COMPARE_DIFFERENCE,
        COMPARE_OVERLAY,
        COMPARE_SCROLL
    };

    explicit FrameAnalyzer(AnalysisToolController *_parent, QObject *parent = 0);

    ~FrameAnalyzer();

    /**
     * Wird von der GUI Aufgerufen, um nach erfolgreicher Signals und Slots Initialisierung alle GUI-Elemente zu updaten.
     */
    void resendSignals();

    /**
     * Event, das bei einer Änderung des Vorschauframes aufgerufen wird.
     */
    void onPreviewFrameChanged(unsigned int _frame);

    /**
     * Event, das bei einer Anfrage zur Synchronisation zum Master Frame aufgerufen wird
     */
    void onSyncToMasterFrame();

    /**
     * Gibt die Namen und Referenzen aller ImageSeries zurück, damit diese in der GUI angezeigt und ausgewählt werden können.
     */
    std::list<std::pair<std::string, DataSeries*> > getSources() {
        return parent()->testData()->dataSeriesInformationOfType<ImageSeries>();
    }

    /**
     * Gibt den TimeManipulator zurück. Darf nur von der GUI verwendet werden, um die Widgets an die Signale und Slots zu binden.
     */
    TimeManipulator *timeManipulator() {
        return m_time_manipulator;
    }

signals:
    void frameChanged(unsigned int _frame);
    void syncedChanged(bool _synced);
    void pictureChanged(QPixmap _picture);

public slots:
    void asyncSetFrame(unsigned int _frame);
    void setSynced(bool _synced);

    /**
     * Wird aufgerufen, wenn der Benutzer eine DataSeries aktiviert bzw. deaktiviert.
     */
    void setDataSeriesEnabled(DataSeries *_series, bool _enabled);

    /**
     * Setzt den Vergleichmodus.
     */
    void setCompareMode(CompareMode _mode);

    /**
     * Setzt den Scrollwert für manche Vergleicher. Die Werte müssen zwischen 0.0 und 1.0 liegen.
     */
    void setScroll(float _relative, float _absolute);

private:
    void updatePreviewFrame(unsigned int _frame, bool _force = false);

    void updatePreviewFrame() {
        updatePreviewFrame(m_frame, true);
    }

    cv::Mat getCVMatFromEnabledSeries(unsigned int _index) {
        ImageSeries *series = m_enabled_series.at(_index);
        return series->itemAsCVMatrix(m_frame);
    }

    bool m_synced;
    unsigned int m_frame;
    std::vector<ImageSeries*> m_enabled_series;
    CompareMode m_compare_mode;
    float m_relative_scroll, m_absolute_scroll;

    TimeManipulator *m_time_manipulator;
};

} //end namespace taets

#endif // FRAMEANALYZER_H
