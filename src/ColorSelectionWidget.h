#ifndef COLORSELECTIONWIDGET_H
#define COLORSELECTIONWIDGET_H

#include <QWidget>

class ColorSelectionWidget : public QWidget
{
    Q_OBJECT

    QColor _color;
public:
    explicit ColorSelectionWidget(QColor color = Qt::white, QWidget *parent = 0);
    QColor color() { return _color; }

signals:
    void colorChanged(QColor _color);

public slots:
    void colorDialogRequested();
    void setColor(const QColor& color);
};

#endif // COLORSELECTIONWIDGET_H
