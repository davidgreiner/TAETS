#include <QWidget>
#include "NumberParam.h"
#include <iostream>
#include <sstream>
#include <ParameterWidgetFactory.h>

namespace taets {

NumberParam::NumberParam(std::string name, float min, float max, float value, bool is_integer, bool is_spinner)
    : NumberSetting(name, min, max, value, is_integer, is_spinner) {

}

NumberParam::NumberParam(NumberParam * np)
    : NumberSetting(np->name(), np->minimum(), np->maximum(), np->floatValue(),
                    np->isInteger(), np->isSpinner()) {

}

NumberParam::NumberParam() : NumberSetting(tr("Number Parameter").toStdString(), 0, 100, 50, true, false) {}

QWidget * NumberParam::createWidget(ParameterWidgetFactory * factory) {
    return factory->createNumberWidget(this);
}

std::string NumberParam::toString() {
    std::stringstream ss;
    if(isInteger())
        ss << intValue();
    else
        ss << floatValue();
    return ss.str();
}

std::string NumberParam::id() {
    return tr("Number Parameter").toStdString();
}


} // namespace taets
