
#include "ChoiceSetting.h"
#include "NodeSettingWidgetFactory.h"
#include <QWidget>
#include <stdexcept>
#include <string>

namespace taets {

/**
 * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
 */
QWidget* ChoiceSetting::createWidget(NodeSettingWidgetFactory* factory) {
    return factory->createChoiceWidget(this);
}

ChoiceSetting::ChoiceSetting(std::string name, std::vector<std::string> choices, int selection, bool dropdown)
    : NodeSetting(name), _choices(choices), _selection(selection), _dropdown(dropdown) {

}

} // namespace taets
