#ifndef DIAGRAMANALYZER_H
#define DIAGRAMANALYZER_H

#include "Analyzer.h"
#include "FloatSeries.h"

#include <QColor>

namespace taets {

class DiagramAnalyzer : public Analyzer
{
    Q_OBJECT
public:
    explicit DiagramAnalyzer(AnalysisToolController *_parent, QObject *parent = 0);

    void onPreviewFrameChanged(unsigned int frame);

    /**
     * Gibt die Namen und Referenzen aller FloatSeries zurück, damit diese in der GUI angezeigt und ausgewählt werden können.
     */
    std::list<std::pair<std::string, DataSeries*> > getSources() {
        return parent()->testData()->dataSeriesInformationOfType<FloatSeries>();
    }

    unsigned int frame() {
        return parent()->frame();
    }

signals:
    void seriesAdded(FloatSeries *series, QColor color);
    void seriesRemoved(FloatSeries *series);
    void frameChanged(unsigned int frame);

public slots:
    void setDataSeriesEnabled(DataSeries *_series, bool enabled);

private:
    struct SeriesEntry {
        FloatSeries *series;
        QColor color;

        bool operator==(const FloatSeries *other) {
            return series == other;
        }
    };

    std::vector<SeriesEntry> _enabled_series;

    std::list<QColor> _unused_colors;
    std::list<QColor> _used_colors;
};

} //end namespace taets

#endif // DIAGRAMANALYZER_H
