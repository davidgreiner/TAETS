
#include "FileSetting.h"
#include "NodeSettingWidgetFactory.h"
#include <QWidget>

namespace taets {

/**
 * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
 */
QWidget* FileSetting::createWidget(NodeSettingWidgetFactory* factory) {
    return factory->createFileWidget(this);
}

FileSetting::FileSetting(std::string name, std::string fn, std::string filter)
    : NodeSetting(name), _filename(fn), _filter(filter) {

}

} // namespace taets
