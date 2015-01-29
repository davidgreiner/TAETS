#include "NodeSettingsListWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include "NodeSetting.h"


NodeSettingsWidgetFactoryImpl * NodeSettingsListWidget::_factory = nullptr;

NodeSettingsListWidget::NodeSettingsListWidget(QWidget *parent) :
    QWidget(parent) {
    if(!_factory)
        _factory = new NodeSettingsWidgetFactoryImpl;

    setLayout(new QVBoxLayout);
    setSettings(nullptr); //initialize empty layout
}

void NodeSettingsListWidget::setSettings(taets::Node * node) {
    // wir leeren das layout, auf die schnellste art.
    QWidget * w = new QWidget(); // temporäres dummy widget
    w->setLayout(layout()); // reparentet das layout
    delete w; // löscht alle Kinder des Widgets.

    QVBoxLayout * layout = new QVBoxLayout();

    //add node settings, if there is a node
    if(node) {
        for(taets::NodeSetting * setting : node->getSettings()) {
            layout->addWidget(setting->createWidget(_factory));
            connect(setting, &taets::NodeSetting::valueChanged, this, &NodeSettingsListWidget::valueChange);
        }
    }

    //spacer
    layout->addStretch(1);
    setLayout(layout);
}

void NodeSettingsListWidget::valueChange() {
    emit settingsChanged();
}
