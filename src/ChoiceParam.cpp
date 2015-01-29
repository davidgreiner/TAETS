#include <QWidget>
#include "ChoiceParam.h"
#include "ParameterWidgetFactory.h"

namespace taets {

ChoiceParam::ChoiceParam(std::string name, std::vector<std::string> choices, int choice)
    : ChoiceSetting(name, choices, choice) {

}

ChoiceParam::ChoiceParam(ChoiceParam * cp)
    : ChoiceSetting(cp->name(), cp->choices(), cp->selection()) {

}

QWidget * ChoiceParam::createWidget(ParameterWidgetFactory * factory) {
    factory->createChoiceWidget(this);
}

ChoiceParam::ChoiceParam() : ChoiceSetting(tr("Choice Parameter").toStdString(), *(new std::vector<std::string>()), 0) {}

std::string ChoiceParam::id() {
    return tr("Choice Parameter").toStdString();
}

} // namespace taets
