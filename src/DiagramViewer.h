#ifndef DIAGRAMVIEWER_H
#define DIAGRAMVIEWER_H

#include <QWidget>
#include <cfloat>
#include "FloatSeries.h"

class DiagramViewer : public QWidget
{
    Q_OBJECT
public:
    explicit DiagramViewer(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void frameChanged(unsigned int frame);

public slots:
    void addSeries(taets::FloatSeries *series, QColor color);
    void removeSeries(taets::FloatSeries *series);
    void setFrame(unsigned int frame);
    void resetViewport();

private:
    struct Element {
        taets::FloatSeries *series;
        QColor color;
        float min, max;
        QPainterPath path;

        Element(taets::FloatSeries *_series, QColor _color) {
            series = _series;
            color = _color;

            min = FLT_MAX;
            max = FLT_MIN;

            //construct min, max and painter path
            unsigned int i;
            for(i = 0; i < series->length(); i++) {
                float v = series->itemAsFloat(i);
                if(min > v) min = v;
                if(max < v) max = v;

                if(i == 0)
                    path.moveTo(i, v);
                else
                    path.lineTo(i, v);
            }
        }

        bool operator==(const taets::FloatSeries *other) {
            return series == other;
        }
    };

    std::list<Element> _series;
    unsigned int _frame;
    int _xmin, _xmax;
    bool _viewport_initialized;

    int _drag_x_origin, _drag_y_origin;
};

#endif // DIAGRAMVIEWER_H
