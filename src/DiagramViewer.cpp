#include "DiagramViewer.h"

#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <cfloat>

#define SIMPLE_PEN(c) QPen(c, 0)

DiagramViewer::DiagramViewer(QWidget *parent) :
    QWidget(parent) {
    _frame = 0;
    _viewport_initialized = false;
}

void DiagramViewer::paintEvent(QPaintEvent *event) {
    (void)event;

    QPainter painter(this);

    //check if there is anything to draw
    if(_series.size() == 0) return;

    std::list<Element>::iterator it;

    float length = _xmax - _xmin;

    float top = FLT_MIN, bottom = FLT_MAX;
    for(it = _series.begin(); it != _series.end(); it++) {
        if(it->min < bottom) bottom = it->min;
        if(it->max > top) top = it->max;
    }

    if(top - bottom < FLT_EPSILON) {
        top += 0.5;
        bottom -= 0.5;
    }

    float x_scale = width() / length;
    float y_scale = -(height() - 1) / (top - bottom);

    //draw coordinate axes
    painter.setPen(SIMPLE_PEN(Qt::black));
    painter.drawLine(QLineF(0, height(), 0, 0));
    painter.drawLine(QLineF(0, -top * y_scale, width(), -top * y_scale));

    //draw coordinate axes text
    //x
    int xtext_y = std::max<float>(std::min<float>(-top * y_scale + 5 + painter.fontMetrics().height(),
                                                  height() - 10 - painter.fontMetrics().height()),
                                  20 + painter.fontMetrics().height());
    painter.drawText(10, xtext_y, QString::number(_xmin));
    QString xmax_text = QString::number(_xmax);
    painter.drawText(width() - 9 - painter.fontMetrics().width(xmax_text), xtext_y, xmax_text);
    //y
    painter.drawText(10, 15, QString::number(top));
    painter.drawText(10, height() - 5, QString::number(bottom));

    //set up transform
    painter.scale(x_scale, y_scale);
    painter.translate(-_xmin, -top);

    //draw series
    for(it = _series.begin(); it != _series.end(); it++) {
        painter.setPen(SIMPLE_PEN(it->color));
        painter.drawPath(it->path);
    }

    //draw frame indicator
    painter.setPen(SIMPLE_PEN(Qt::red));
    painter.drawLine(QLineF(_frame, bottom, _frame, top));
}

void DiagramViewer::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::MiddleButton || event->buttons() & Qt::RightButton) {
        _drag_x_origin = event->x();
        _drag_y_origin = event->y();
    }
}

void DiagramViewer::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::MiddleButton) {
        if(abs(event->x() - _drag_x_origin) >= 10) {
            int changed = (event->x() - _drag_x_origin) / 10;
            _xmin += changed;
            _xmax -= changed;
            if(_xmax <= _xmin) {
                //revert changes if the result mirrors the diagram (sooo lazy :D)
                _xmin -= changed;
                _xmax += changed;
            }
            _drag_x_origin = event->x() + (event->x() - _drag_x_origin) % 10;
            update();
        }
    }
    if(event->buttons() & Qt::RightButton) {
        int stepsize = width() / (_xmax - _xmin);
        if(abs(event->x() - _drag_x_origin) >= stepsize) {
            int changed = (event->x() - _drag_x_origin) / stepsize;
            _xmin -= changed;
            _xmax -= changed;
            _drag_x_origin = event->x() + (event->x() - _drag_x_origin) % stepsize;
            update();
        }
    }
}

void DiagramViewer::addSeries(taets::FloatSeries *series, QColor color) {
    _series.push_back(Element(series, color));
    if(!_viewport_initialized) resetViewport();
    else update();
}

void DiagramViewer::removeSeries(taets::FloatSeries *series) {
    _series.erase(std::find(_series.begin(), _series.end(), series));
    update();
}

void DiagramViewer::setFrame(unsigned int frame) {
    if(_frame != frame) {
        _frame = frame;
        emit frameChanged(frame);
        update();
    }
}

void DiagramViewer::resetViewport() {
    if(_series.empty()) _viewport_initialized = false;
    else {
        _xmin = 0;
        _xmax = _series.front().series->length() - 1;

        if(_xmax == _xmin) _xmax = 1; //legend fix for length == 1

        _viewport_initialized = true;
        update();
    }
}
