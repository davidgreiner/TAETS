#include "NodeSettingsWidgetFactoryImpl.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>
#include "FileSelectionWidget.h"
#include "ColorSelectionWidget.h"
#include "Color.h"

QGroupBox * createGroupBox(taets::NodeSetting * setting) {
    QGroupBox * box = new QGroupBox(QString::fromStdString(setting->name()));
    QVBoxLayout * layout = new QVBoxLayout();
    box->setLayout(layout);
    return box;
}

QColor toQColor(const taets::Color& color) {
    return QColor(color.red(), color.green(), color.blue());
}

taets::Color toTaetsColor(const QColor& color) {
    return taets::Color(color.red(), color.green(), color.blue());
}

QWidget* NodeSettingsWidgetFactoryImpl::createColorWidget(taets::ColorSetting* setting) {
    QGroupBox * box = createGroupBox(setting);
    ColorSelectionWidget * colorSelect = new ColorSelectionWidget(toQColor(setting->color()));
    colorSelect->connect(colorSelect, &ColorSelectionWidget::colorChanged, [=](QColor color) {
        setting->setColor(toTaetsColor(color));
    });
    box->layout()->addWidget(colorSelect);
    return box;
}

QWidget* NodeSettingsWidgetFactoryImpl::createFileWidget(taets::FileSetting* setting) {
    QGroupBox * box = createGroupBox(setting);
    FileSelectionWidget * fileSelect = new FileSelectionWidget(QString::fromStdString(setting->filename()));
    fileSelect->connect(fileSelect, &FileSelectionWidget::fileChanged, setting,
                        static_cast<void (taets::FileSetting::*)(const QString&)>(&taets::FileSetting::setFilename));
    box->layout()->addWidget(fileSelect);
    return box;
}

QWidget* NodeSettingsWidgetFactoryImpl::createChoiceWidget(taets::ChoiceSetting *setting) {
    QGroupBox * box = createGroupBox(setting);
    if(!setting->isDropDown()) {
        for (int i=0; i < setting->choices().size(); i++) {
            QRadioButton * button = new QRadioButton(QString::fromStdString(setting->choices()[i]));
            if(setting->selection() == i) {
                button->setChecked(true);
            }
            button->connect(button, &QRadioButton::toggled, [=](bool down) {
                if(down) {
                    setting->setSelection(i);
                }
            });
            box->layout()->addWidget(button);
        }
    } else {
        QComboBox * combo = new QComboBox();
        for (std::string choice : setting->choices()) {
            combo->addItem(QString::fromStdString(choice));
        }
        combo->setCurrentIndex(setting->selection());
        combo->connect(combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                       setting, &taets::ChoiceSetting::setSelection);
        box->layout()->addWidget(combo);
    }
    return box;
}

QWidget* NodeSettingsWidgetFactoryImpl::createNumberWidget(taets::NumberSetting* setting) {
    QGroupBox * box = createGroupBox(setting);
    if(setting->isSpinner()) {
        if(setting->isInteger()){
            QSpinBox * spinner = new QSpinBox(box);
            spinner->setMinimum((int) setting->minimum());
            spinner->setMaximum((int) setting->maximum());
            spinner->setValue(setting->intValue());
            spinner->connect(spinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                             setting, &taets::NumberSetting::setIntValue);
            box->layout()->addWidget(spinner);
        } else {
            QDoubleSpinBox * spinner = new QDoubleSpinBox(box);
            spinner->setMinimum(setting->minimum());
            spinner->setMaximum(setting->maximum());
            spinner->setValue(setting->floatValue());
            spinner->connect(spinner, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                             setting, &taets::NumberSetting::setFloatValue);
            box->layout()->addWidget(spinner);
        }
    } else {
        QLabel * label = new QLabel();
        QSlider * slider = new QSlider(Qt::Horizontal);
        if(setting->isInteger()) {
            label->setText(QString::number(setting->intValue()));
            slider->setRange((int)setting->minimum(), (int)setting->maximum());
            slider->setValue(setting->intValue());
            slider->connect(slider, &QSlider::valueChanged, setting, &taets::NumberSetting::setIntValue);
            slider->connect(slider, &QSlider::valueChanged, [=](int value) { label->setText(QString::number(value));});
        } else {
            QString str;
            label->setText(str.setNum(setting->floatValue()));
            slider->setRange(0, 100);
            float range = setting->maximum() - setting->minimum();
            slider->setValue((setting->floatValue() - setting->minimum()) / range * 100);
            slider->connect(slider, &QSlider::valueChanged, [=](int value) { setting->setFloatValue(value * range / 100 + setting->minimum());});
            slider->connect(slider, &QSlider::valueChanged, [=](int value) { QString str; label->setText(str.setNum(setting->floatValue()));});
        }
        box->layout()->addWidget(label);
        box->layout()->addWidget(slider);
    }
    return box;
}
