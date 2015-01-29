#ifndef HISTOGRAMANALYZER_H
#define HISTOGRAMANALYZER_H

#include "Analyzer.h"
#include "ImageSeries.h"
#include "Histogram.h"
#include <map>

namespace taets {

class HistogramAnalyzer : public Analyzer
{
    Q_OBJECT
public:
    explicit HistogramAnalyzer(AnalysisToolController * _parent, QObject *parent = 0);
    ~HistogramAnalyzer();

    void onPreviewFrameChanged(unsigned int frame);

    /**
     * Gibt die Namen und Referenzen aller ImageSeries zurück, damit diese in der GUI angezeigt und ausgewählt werden können.
     */
    std::list<std::pair<std::string, DataSeries*> > getSources() {
        return parent()->testData()->dataSeriesInformationOfType<ImageSeries>();
    }

signals:
    void histogramsChanged(std::vector<Histogram*> histograms);

public slots:
    void setDataSeriesEnabled(DataSeries *_series, bool enabled);

private:
    void recalculateHistograms(bool frame_changed);

    unsigned int _frame;

    std::map<ImageSeries*, Histogram*> _enabled_series;
};

} //end namespace taets

#endif // HISTOGRAMANALYZER_H
