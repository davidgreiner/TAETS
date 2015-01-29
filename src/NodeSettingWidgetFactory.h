#ifndef TAETS_NODESETTINGWIDGETFACTORY_H
#define TAETS_NODESETTINGWIDGETFACTORY_H

#include <QWidget>
#include "ColorSetting.h"
#include "FileSetting.h"
#include "ChoiceSetting.h"
#include "NumberSetting.h"

namespace taets {

/**
 * Abstrakte Fabrik, die NodeSetting benutzen, um auf Anfrage der GUI passende Widgets zu generieren. Eine konkrete Fabrik muss vom View implementiert werden.
 */
class NodeSettingWidgetFactory {
  public:
    /**
     * Erstellt ein neues Farbauswahl-Widget.
     */
    virtual QWidget* createColorWidget(ColorSetting* setting) = 0;

    /**
     * Erstellt ein neues Dateiauswahl-Widget.
     */
    virtual QWidget* createFileWidget(FileSetting* setting) = 0;

    /**
     * Erstellt ein neues Auswahl-Widget.
     */
    virtual QWidget* createChoiceWidget(ChoiceSetting* setting) = 0;

    /**
     * Erstellt ein neues Zahlenauswahl-Widget in der Form, die das Objekt angibt.
     */
    virtual QWidget* createNumberWidget(NumberSetting* setting) = 0;
};

}
#endif
