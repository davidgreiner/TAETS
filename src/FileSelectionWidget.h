#ifndef FILESELECTIONWIDGET_H
#define FILESELECTIONWIDGET_H

#include <QWidget>

class FileSelectionWidget : public QWidget
{
    Q_OBJECT

    QString _filename;
    QString _filter;

public:
    explicit FileSelectionWidget(QString filename = QString(), QWidget *parent = 0);

    QString filename() { return _filename; }

signals:
    void fileChanged(const QString& filename);

public slots:
    void requestFileDialog();
};

#endif // FILESELECTIONWIDGET_H
