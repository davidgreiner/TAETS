#include "ColorSelectionWidget.h"
#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QPalette>
#include <QColorDialog>
#include <iostream>

inline void changeFrameBackground(QFrame * colorFrame, const QColor& color) {
    QPalette pal(colorFrame->palette());
    pal.setColor(QPalette::Background, color);
    colorFrame->setPalette(pal);
    colorFrame->update();
}

ColorSelectionWidget::ColorSelectionWidget(QColor color, QWidget *parent) :
    QWidget(parent), _color(color)
{
    QHBoxLayout * layout = new QHBoxLayout();
    QFrame * colorFrame = new QFrame();
    colorFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    colorFrame->setLineWidth(1);
    colorFrame->setMinimumHeight(22);
    colorFrame->setMinimumWidth(150);
    colorFrame->setAutoFillBackground(true);
    changeFrameBackground(colorFrame, color);

    QPushButton * button = new QPushButton(tr("Choose color..."));
    connect(button, &QPushButton::pressed, this, &ColorSelectionWidget::colorDialogRequested);

    layout->addWidget(colorFrame);
    layout->addWidget(button);
    setLayout(layout);

    connect(this, &ColorSelectionWidget::colorChanged, [=](QColor col) {
        changeFrameBackground(colorFrame, col);
    });
}

void ColorSelectionWidget::colorDialogRequested() {
    QColor selected = QColorDialog::getColor(_color, nullptr, tr("Choose color..."));
    if(selected.isValid()) {
        setColor(selected);
    }
}

void ColorSelectionWidget::setColor(const QColor& color) {
    if(color != _color) {
        _color = color;
        emit colorChanged(_color);
    }
}
