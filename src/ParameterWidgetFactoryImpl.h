#ifndef PARAMETERWIDGETFACTORYIMPL_H
#define PARAMETERWIDGETFACTORYIMPL_H

#include "ParameterWidgetFactory.h"
#include "BinaryTemplate.h"
#include <QGroupBox>
#include <string>
#include <QCoreApplication>

class ParameterWidgetFactoryImpl : public taets::ParameterWidgetFactory
{
    Q_DECLARE_TR_FUNCTIONS(ParameterWidgetFactoryImpl)

public:
    ParameterWidgetFactoryImpl();

    virtual QWidget * createNumberWidget(taets::NumberParam * param);
    virtual QWidget * createFileWidget(taets::FileParam * param);
    virtual QWidget * createChoiceWidget(taets::ChoiceParam * param);
};

#endif // PARAMETERWIDGETFACTORYIMPL_H
