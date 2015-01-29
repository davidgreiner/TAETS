#include "ParameterWidgetFactoryImpl.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <cfloat>

// Prototypen für Hilfsfunktionen
void addChoice(QGridLayout *layout, taets::ChoiceParam *param, int i);
void removeChoice(QGridLayout *layout, taets::ChoiceParam *param);

ParameterWidgetFactoryImpl::ParameterWidgetFactoryImpl()
{
}

QGroupBox * createGroupBox(taets::TemplateParameter * param) {
    QGroupBox * box = new QGroupBox(QString::fromStdString(param->id()));
    box->setFlat(false);
    QVBoxLayout * layout = new QVBoxLayout();
    QLineEdit * nameBox = new QLineEdit();
    nameBox->setText(QString::fromStdString(param->name()));
    nameBox->connect(nameBox, &QLineEdit::textChanged,
    [=](const QString& text) {
        param->setName(text.toStdString());
    });
    layout->addWidget(nameBox);
    box->setLayout(layout);
    return box;
}

QWidget * ParameterWidgetFactoryImpl::createNumberWidget(taets::NumberParam * param) {
    QGroupBox * box = createGroupBox(param);
    QGridLayout * gridLayout = new QGridLayout();
    gridLayout->setMargin(0);
    QCheckBox * sliderBox = new QCheckBox();
    sliderBox->setText(tr("Spinner"));
    sliderBox->setChecked(param->isSpinner());
    sliderBox->connect(sliderBox, static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                       param, static_cast<void (taets::NumberParam::*)(int)>(&taets::NumberParam::isSpinnerChanged));
    gridLayout->addWidget(sliderBox, 0, 0, 1, 2);

    QCheckBox * intBox = new QCheckBox();
    intBox->setText(tr("Integer"));
    intBox->setChecked(param->isInteger());
    intBox->connect(intBox, static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                    param, static_cast<void (taets::NumberParam::*)(int)>(&taets::NumberParam::isIntChanged));
    gridLayout->addWidget(intBox, 0, 2, 1, 2);

    QDoubleSpinBox * minBox = new QDoubleSpinBox;
    minBox->setRange(-DBL_MAX, DBL_MAX); // <- Hier siehen sie, was das Problem mit C ist.
                               // Hätte es mich umgebracht, hier DOUBLE_MIN zu schreiben?
    minBox->setValue(param->minimum());
    minBox->connect(minBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    param, &taets::NumberParam::minimumChanged);
    gridLayout->addWidget(minBox, 1, 0, 1, 1);

    QDoubleSpinBox * valueBox = new QDoubleSpinBox;
    valueBox->setRange(-DBL_MAX, DBL_MAX);
    valueBox->setValue(param->floatValue());
    valueBox->connect(valueBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                      param, &taets::NumberParam::valueChanged);
    gridLayout->addWidget(valueBox, 1, 1, 1, 2);

    QDoubleSpinBox * maxBox = new QDoubleSpinBox;
    maxBox->setRange(-DBL_MAX, DBL_MAX);
    maxBox->setValue(param->maximum());
    maxBox->connect(maxBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    param, &taets::NumberParam::maximumChanged);
    gridLayout->addWidget(maxBox, 1, 3, 1, 1);

    minBox->connect(minBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](double value) {
        maxBox->setMinimum(value);
        valueBox->setMinimum(value);
    });

    maxBox->connect(maxBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](double value) {
        minBox->setMaximum(value);
        valueBox->setMaximum(value);
    });

    //Direkt als item hinzufügen geht nicht, also in Frame einbetten
    //box->layout()->addItem(gridLayout);
    QFrame * frame = new QFrame();
    frame->setLayout(gridLayout);
    box->layout()->addWidget(frame);

    return box;
}

QWidget * ParameterWidgetFactoryImpl::createFileWidget(taets::FileParam * param) {
    return createGroupBox(param);
}

QWidget * ParameterWidgetFactoryImpl::createChoiceWidget(taets::ChoiceParam * param) {
    QGroupBox * box = createGroupBox(param);

    QWidget * dropdownSelect = new QWidget();
    dropdownSelect->setLayout(new QHBoxLayout());
    QButtonGroup * buttonGroup = new QButtonGroup(dropdownSelect);
    QRadioButton * dropdownNo = new QRadioButton(tr("radio buttons"));
    dropdownNo->setChecked(true);
    QRadioButton * dropdownYes = new QRadioButton(tr("dropdown"));
    buttonGroup->addButton(dropdownNo);
    buttonGroup->addButton(dropdownYes);
    dropdownSelect->layout()->addWidget(dropdownNo);
    dropdownSelect->layout()->addWidget(dropdownYes);

    dropdownSelect->connect(dropdownYes, &QRadioButton::toggled, param, &taets::ChoiceParam::dropdownChanged);

    box->layout()->addWidget(dropdownSelect);

    QPushButton * addOption = new QPushButton(tr("Add option"));
    box->layout()->addWidget(addOption);

    QGridLayout * layout = new QGridLayout();

    layout->setMargin(0);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(0, 1);
    // Seid ihr bereit für die hacks?
    addOption->connect(addOption, &QPushButton::pressed, [=](){
        param->choiceAdded(tr("Choice").toStdString());
        addChoice(layout, param, param->choices().size() - 1);
    });
    QWidget * frame = new QWidget();
    frame->setLayout(layout);
    box->layout()->addWidget(frame);

    QPushButton * removeOption = new QPushButton(tr("Remove Option"));
    box->layout()->addWidget(removeOption);
    removeOption->connect(removeOption, &QPushButton::pressed, [=]() {
        removeChoice(layout, param);
        param->choiceRemoved();
    });

    for(int i=0; i < param->choices().size(); i++) {
        addChoice(layout, param, i);
    }

    return box;
}

// TODO: Sanity Checks der selection
void addChoice(QGridLayout * layout, taets::ChoiceParam * param, int i) {
    QRadioButton * radio = new QRadioButton();
    if(i == param->selection())
        radio->setChecked(true);
    QLineEdit * edit = new QLineEdit(QString::fromStdString(param->choices()[i]));
    edit->connect(edit, &QLineEdit::textChanged, [=](const QString& text) {
        param->choiceChanged(i, text.toStdString());
    });
    radio->connect(radio, &QRadioButton::toggled, [=](bool state) {
        if(state)
            param->defaultChanged(i);
    });
    layout->addWidget(radio, i, 0, 1, 1);
    layout->addWidget(edit, i, 1, 1, 1);
}

void removeChoice(QGridLayout * layout, taets::ChoiceParam * param) {
    int len = param->choices().size() - 1;
    QLayoutItem * radio = layout->itemAtPosition(len, 0);
    layout->removeItem(radio);
    delete radio;
    QLayoutItem * edit = layout->itemAtPosition(len, 1);
    layout->removeItem(edit);
    delete edit;
}
