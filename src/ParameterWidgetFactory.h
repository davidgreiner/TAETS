#ifndef PARAMETERWIDGETFACTORY_H
#define PARAMETERWIDGETFACTORY_H
#include <QWidget>
#include "NumberParam.h"
#include "FileParam.h"
#include "ChoiceParam.h"

namespace taets {

class ParameterWidgetFactory
{
public:
    virtual QWidget * createNumberWidget(NumberParam * param) = 0;
    virtual QWidget * createFileWidget(FileParam * param) = 0;
    virtual QWidget * createChoiceWidget(ChoiceParam * param) = 0;
};

}

#endif // PARAMETERWIDGETFACTORY_H
