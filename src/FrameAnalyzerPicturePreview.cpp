#include "FrameAnalyzerPicturePreview.h"

#include <QPainter>
#include <QPaintEvent>
#include <cmath>

static float clamp(float x, float min, float max);

FrameAnalyzerPicturePreview::FrameAnalyzerPicturePreview(QWidget *parent) :
    QWidget(parent)
{
    m_smooth = false;

    m_zoom = 1.0;
    m_offset = QPointF(0.0, 0.0);
}

void FrameAnalyzerPicturePreview::paintEvent(QPaintEvent *_event)
{
    (void)_event;

    QPainter painter(this);

    painter.setRenderHint(QPainter::SmoothPixmapTransform, m_smooth);

    //space avaliable for drawing
    QRect canvas(0, 0, width()-1, height()-1);

    //part of pixmap to draw
    QRect source(m_offset.toPoint(), m_pixmap.size() * m_zoom);

    float scale_x, scale_y;

    //rectangle, into which the pixmap is drawn (QRect has some sideeffects, so we update it when all values have benn calculated)
    float x, y, w, h;

    scale_x = canvas.width() / (float)source.width();
    scale_y = canvas.height() / (float)source.height();

    if(scale_x < scale_y) {
        w = source.width() * scale_x;
        h = source.height() * scale_x;
        x = 0;
        y = (canvas.height() - h) / 2;
    } else {
        w = source.width() * scale_y;
        h = source.height() * scale_y;
        x = (canvas.width() - w) / 2;
        y = 0;
    }

    m_target_rect.setRect(x, y, w, h);

    painter.drawPixmap(m_target_rect, m_pixmap, source);
}

void FrameAnalyzerPicturePreview::drag(QPoint pos)
{
    float absolute = clamp((pos.x() - m_target_rect.x()) / (float)m_target_rect.width(), 0.0, 1.0);
    float relative = clamp(absolute * m_zoom + m_offset.x()/(float)m_pixmap.width(), 0.0, 1.0);

    emit dragChanged(relative, absolute);
}

static float clamp(float x, float min, float max)
{
    return x > max ? max : x < min ? min : x;
}
