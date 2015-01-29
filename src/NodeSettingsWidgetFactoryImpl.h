#ifndef NODESETTINGSWIDGETFACTORYIMPL_H
#define NODESETTINGSWIDGETFACTORYIMPL_H

#include "NodeSettingWidgetFactory.h"

/**
 * @brief Standardimplementierung der NodeSettingsWidgetFactory.
 * @see NodeSettingsWidgetFactory
 */
class NodeSettingsWidgetFactoryImpl : public taets::NodeSettingWidgetFactory
{
public:
    QWidget* createColorWidget(taets::ColorSetting* setting);

    QWidget* createFileWidget(taets::FileSetting* setting);

    QWidget* createChoiceWidget(taets::ChoiceSetting* setting);

    QWidget* createNumberWidget(taets::NumberSetting* setting);
};

#endif // NODESETTINGSWIDGETFACTORYIMPL_H
