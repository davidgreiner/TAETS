#include "NumberSetting.h"
#include <QWidget>
#include "NodeSettingWidgetFactory.h"

namespace taets {

/**
 * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
 */
QWidget* NumberSetting::createWidget(NodeSettingWidgetFactory* factory) {
    return factory->createNumberWidget(this);
}

NumberSetting::NumberSetting(std::string name, float min, float max, float value, bool isInt, bool isSpinner)
    : NodeSetting(name), _min(min), _max(max), _is_integer(isInt), _value(value), _is_spinner(isSpinner) {
}

NumberSetting::NumberSetting() {}

} // namespace taets
