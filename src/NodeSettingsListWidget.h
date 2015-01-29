#ifndef NODESETTINGSLISTWIDGET_H
#define NODESETTINGSLISTWIDGET_H

#include "Node.h"
#include <QWidget>

#include "NodeSettingsWidgetFactoryImpl.h"

class NodeSettingsListWidget : public QWidget {
    Q_OBJECT
public:
    explicit NodeSettingsListWidget(QWidget *parent = 0);

signals:
    /**
     * Wird emitted, wenn eine Einstellung geändert wurde.
     */
    void settingsChanged();

public slots:
    /**
     * Zeigt die Einstellungswidgets der Node an. nullptr löscht alle Widgets.
     */
    void setSettings(taets::Node * node);

    /**
     * Entfernt alle Widgets.
     */
    void removeSettings() {
        setSettings(nullptr);
    }

private slots:
    void valueChange();

private:
    static NodeSettingsWidgetFactoryImpl *_factory;
};

#endif // NODESETTINGSLISTWIDGET_H
