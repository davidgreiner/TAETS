#ifndef SOURCESSELECTIONWIDGET_H
#define SOURCESSELECTIONWIDGET_H

#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QEvent>
#include <list>
#include <vector>
#include <utility>
#include <DataSeries.h>

class SourcesSelectionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit SourcesSelectionWidget(QWidget *parent = 0);

    void changeEvent(QEvent *event) {
        if(event->type() == QEvent::LanguageChange)
            retranslate();
        else
            QWidget::changeEvent(event);
    }

    void setSources(std::list<std::pair<std::string, taets::DataSeries*> > sources);

signals:
    void selectedChanged(taets::DataSeries *_item, bool _selected);

public slots:
    void setColor(taets::DataSeries *_item, QColor _color);
    void resetColor(taets::DataSeries *_item);

private slots:
    void checkboxToggled(bool toggled);

private:
    void retranslate();

    struct Entry {
        QCheckBox *checkbox;
        QLabel *color_box;
        taets::DataSeries *series;
        bool enabled;
    };

    Entry *getEntryWith(taets::DataSeries *_series);

    std::vector<Entry> _widgets;
};

#endif // SOURCESSELECTIONWIDGET_H
