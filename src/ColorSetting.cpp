#include "ColorSetting.h"
#include "NodeSettingWidgetFactory.h"
#include <QWidget>

namespace taets {

/**
 * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
 */
QWidget* ColorSetting::createWidget(NodeSettingWidgetFactory* factory) {
    return factory->createColorWidget(this);
}

ColorSetting::ColorSetting(std::string name, Color color) : NodeSetting(name), _color(color) {

}

}
