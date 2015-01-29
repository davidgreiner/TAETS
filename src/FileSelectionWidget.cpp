#include "FileSelectionWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

FileSelectionWidget::FileSelectionWidget(QString filename, QWidget *parent) :
    QWidget(parent), _filename(filename)
{
    QHBoxLayout * layout = new QHBoxLayout();
    QPushButton * button = new QPushButton(tr("select file..."));
    connect(button, &QPushButton::pressed, this, &FileSelectionWidget::requestFileDialog);
    QLabel * label = new QLabel(filename);
    label->setFixedWidth(120);
    label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    connect(this, &FileSelectionWidget::fileChanged, label, &QLabel::setText);
    layout->addWidget(label);
    layout->addWidget(button);
    setLayout(layout);
}

void FileSelectionWidget::requestFileDialog() {
    QString selection = QFileDialog::getOpenFileName(nullptr, tr("select file..."),
                                                     QString(), _filter);
    if(!selection.isNull()) {
        _filename = selection;
        emit fileChanged(_filename);
    }
}
