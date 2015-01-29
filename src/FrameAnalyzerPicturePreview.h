#ifndef FRAMEANALYZERPICTUREPREVIEW_H
#define FRAMEANALYZERPICTUREPREVIEW_H

#include <QFrame>
#include <QPixmap>
#include <QMouseEvent>
#include <cmath>

class FrameAnalyzerPicturePreview : public QWidget
{
    Q_OBJECT
public:
    explicit FrameAnalyzerPicturePreview(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *_event);

    void mouseMoveEvent(QMouseEvent *_event) {
        if(_event->buttons() & Qt::LeftButton) {
            drag(_event->pos());
        } else if(_event->buttons() & Qt::MiddleButton) {
            m_offset += (m_offset_drag_old_pos - _event->pos()) * m_zoom;
            m_offset_drag_old_pos = _event->pos();
            update();
        }
    }

    void mousePressEvent(QMouseEvent *_event) {
        if(_event->buttons() & Qt::LeftButton) {
            drag(_event->pos());
        } else if(_event->buttons() & Qt::MiddleButton) {
            m_offset_drag_old_pos = _event->pos();
        }
    }

    void wheelEvent(QWheelEvent *_event) {
        float delta = _event->angleDelta().y() / 1000.0;

        QPointF move = (delta * m_zoom * QPointF(m_pixmap.size().width(), m_pixmap.size().height())) / 2;

        float new_zoom = m_zoom + m_zoom * delta;
        if(new_zoom > (1.0/0.75) || new_zoom < 1.0 / (float)std::min(m_pixmap.size().width(), m_pixmap.size().height())) return;

        m_zoom = new_zoom;
        m_offset -= move;

        update();
    }

signals:
    void dragChanged(float _relative, float _absolute);
    void smoothChanged(bool _smooth);

public slots:
    void setPixmap(const QPixmap &_pixmap) {
        m_pixmap = _pixmap;
        update();
    }

    void setSmooth(bool _smooth) {
        if(m_smooth != _smooth) {
            m_smooth = _smooth;
            emit smoothChanged(m_smooth);
            update();
        }
    }

    void fit() {
        m_zoom = 1.0;
        m_offset = QPointF(0.0, 0.0);
        update();
    }

private:
    void drag(QPoint pos);

    QPixmap m_pixmap;
    QRect m_target_rect;
    bool m_smooth;

    float m_zoom;
    QPointF m_offset;
    QPointF m_offset_drag_old_pos;
};

#endif // FRAMEANALYZERPICTUREPREVIEW_H
