#ifndef TABLEANALYZER_H
#define TABLEANALYZER_H

#include <Analyzer.h>
#include <FloatSeries.h>
#include <list>

namespace taets {

class TableAnalyzer : public Analyzer
{
    Q_OBJECT
public:
    explicit TableAnalyzer(AnalysisToolController *_parent, QObject *parent = 0);

    void onPreviewFrameChanged(unsigned int frame);

    std::list<std::pair<std::string, DataSeries*> > getSources() {
        return parent()->testData()->dataSeriesInformationOfType<FloatSeries>();
    }

signals:
    void seriesAdded(FloatSeries *series, std::string name);
    void seriesDeleted(FloatSeries *series);
    void seriesValueChanged(FloatSeries *series, float value);

public slots:
    void setDataSeriesEnabled(DataSeries *series, bool enabled);

private:
    unsigned int _frame;
    std::list<FloatSeries*> _enabled_series;
};

} //end namespace taets

#endif // TABLEANALYZER_H
