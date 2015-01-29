#include "TemplateEditorWindow.h"
#include "ui_TemplateEditorWindow.h"
#include "TemplateManager.h"
#include "ParameterWidgetFactoryImpl.h"
#include <QSpacerItem>
#include <iostream>
#include <QStringListModel>

TemplateEditorWindow::TemplateEditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TemplateEditorWindow),
    templates(taets::TemplateManager::instance().templates)
{
    ui->setupUi(this);
    factory = new ParameterWidgetFactoryImpl;
    repopulateList();
    connect(ui->templateList->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &TemplateEditorWindow::changeTemplateSelection);
    connect(ui->templateList->model(), &QAbstractItemModel::dataChanged,
            this, &TemplateEditorWindow::editTemplateName);
    if(templates.size() != 0) {
        ui->templateList->selectionModel()->select(
                    ui->templateList->model()->index(0,0), QItemSelectionModel::Select);
    } else {
        ui->removeItem->setEnabled(false);
        ui->deleteTemplate->setEnabled(false);
        ui->addItem->setEnabled(false);
        ui->perFrameCheck->setEnabled(false);
        ui->yuvCheck->setEnabled(false);
    }
}

void TemplateEditorWindow::repopulateList() {
    QStringList l;
    foreach(taets::BinaryTemplate * p, templates) {
        l.append(QString::fromStdString(p->name()));
    }
    delete ui->templateList->model();
    ui->templateList->setModel(new QStringListModel(l));
}

TemplateEditorWindow::~TemplateEditorWindow()
{
    delete ui;
}

void TemplateEditorWindow::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    else
        QWidget::changeEvent(event);
}

void TemplateEditorWindow::saveChanges() {
    taets::TemplateManager::instance().save();
}

void TemplateEditorWindow::discardChanges() {
    taets::TemplateManager::instance().load();
    repopulateList();
}

void TemplateEditorWindow::changeTemplateSelection() {
    foreach(QWidget * p, structureWidgets) {
        ui->templateStructure->layout()->removeWidget(p);
        delete p;
    }
    structureWidgets.clear();


    taets::BinaryTemplate * selected = selectedTemplate();

    if(selected != NULL) {
        if(selected->parameters().size() > 0)
            ui->removeItem->setEnabled(true);
        else
            ui->removeItem->setEnabled(false);

        ui->deleteTemplate->setEnabled(true);
        ui->addItem->setEnabled(true);
        ui->yuvCheck->setEnabled(true);
        ui->perFrameCheck->setEnabled(true);
        ui->yuvCheck->setChecked(selected->yuv());
        ui->perFrameCheck->setChecked(selected->perFrame());

        for(int i=0; i < selected->parameters().size(); i++) {
            addTextInput(i);
            addParameterSelector(i);
        }
        // Die Textkomponenten sind garantiert genau einer mehr
        addTextInput(selected->parameters().size());

        repopulateParameters();
    } else {
        ui->removeItem->setEnabled(false);
        ui->deleteTemplate->setEnabled(false);
        ui->addItem->setEnabled(false);
        ui->yuvCheck->setEnabled(false);
        ui->perFrameCheck->setEnabled(false);
    }
}

void TemplateEditorWindow::toggleYUV(bool state) {
    taets::BinaryTemplate * selected = selectedTemplate();
    selected->setYUV(state);
}

void TemplateEditorWindow::togglePerFrame(bool state) {
    taets::BinaryTemplate * selected = selectedTemplate();
    selected->setPerFrame(state);
}

void TemplateEditorWindow::deleteTemplate() {
    QListView * view = ui->templateList;
    QAbstractItemModel * model = view->model();
    int index = view->selectionModel()->selectedRows()[0].row();
    delete templates[index];
    templates.erase(templates.begin() + index);
    model->removeRow(index);
}

void TemplateEditorWindow::newTemplate() {
    taets::BinaryTemplate * tmpl = new taets::BinaryTemplate;
    templates.push_back(tmpl);
    QListView * view = ui->templateList;
    QAbstractItemModel * model = view->model();
    model->insertRow(model->rowCount());
    model->setData(model->index(model->rowCount() - 1, 0), QString::fromStdString(tmpl->name()));
    // Letztes Eelement auswählen
    view->selectionModel()->select(model->index(model->rowCount() - 1, 0), QItemSelectionModel::Select);
    view->edit(model->index(model->rowCount() - 1, 0));
}

void TemplateEditorWindow::editTemplateName(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
    int s = topLeft.row();
    int e = bottomRight.row();

    // An diesem Punkt will ich einfach nur noch Sachen werfen...
    if(s != e)
        throw e - s;

    QString str = ui->templateList->model()->data(topLeft).toString();

    templates[s]->setName(str.toStdString());
}

void TemplateEditorWindow::repopulateParameters() {
    foreach(QWidget * p, parameterWidgets) {
        ui->settingsArea->layout()->removeWidget(p);
        delete p;
    }
    QLayoutItem * item = ui->settingsArea->layout()->itemAt(ui->settingsArea->layout()->count() -1);
    ui->settingsArea->layout()->removeItem(item);
    delete item;
    parameterWidgets.clear();

    taets::BinaryTemplate * selected = selectedTemplate();

    foreach(taets::TemplateParameter * p, selected->parameters()) {
        QWidget * parameterWidget = p->createWidget(factory);
        parameterWidgets.push_back(parameterWidget);
        ui->settingsArea->layout()->addWidget(parameterWidget);
    }
    QSpacerItem * spacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->settingsArea->layout()->addItem(spacer);
}

void TemplateEditorWindow::addParameterSelector(int paramIndex) {
    taets::TemplateManager& manager = taets::TemplateManager::instance();
    QComboBox * paramSelect = new QComboBox;
    foreach(taets::TemplateParameter * p, manager.prototypes) {
        paramSelect->addItem(QString::fromStdString(p->name()));
    }

    taets::BinaryTemplate * selected = selectedTemplate();

    taets::TemplateParameter * param = selected->parameters()[paramIndex];

    // der richtige Typ muss am Anfang ausgewählt sein
    for(int i=0; i < manager.prototypes.size(); i++) {
        if(manager.prototypes[i]->id() == param->id()) {
            paramSelect->setCurrentIndex(i);
        }
    }

    connect(paramSelect, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) {
                changeParameterType(paramIndex, index);
            });
    ui->templateStructure->layout()->addWidget(paramSelect);
    structureWidgets.push_back(paramSelect);
}

void TemplateEditorWindow::addTextInput(int paramIndex) {
    QLineEdit * edit = new QLineEdit();

    edit->setMinimumWidth(100);

    taets::BinaryTemplate * selected = selectedTemplate();
    edit->setText(QString::fromStdString(selected->textParts()[paramIndex]));

    connect(edit, &QLineEdit::textChanged, [=](const QString& text) {
        editTextPart(paramIndex, text);
    });
    ui->templateStructure->layout()->addWidget(edit);
    structureWidgets.push_back(edit);
}

taets::BinaryTemplate * TemplateEditorWindow::selectedTemplate() {
    if(ui->templateList->selectionModel()->selectedRows().size() == 0) {
        return NULL;
    }
    int s = ui->templateList->selectionModel()->selectedRows()[0].row();
    return templates[s];
}

void TemplateEditorWindow::changeParameterType(int paramIndex, int typeIndex) {
    taets::BinaryTemplate * selected = selectedTemplate();
    taets::TemplateParameter * p = selected->parameters()[paramIndex];
    delete p;
    p = taets::TemplateManager::instance().prototypes[typeIndex]->clone();
    selected->parameters()[paramIndex] = p;

    repopulateParameters();
}

void TemplateEditorWindow::editTextPart(int paramIndex, const QString &text) {
    taets::BinaryTemplate * selected = selectedTemplate();
    selected->textParts()[paramIndex] = text.toStdString();
}

void TemplateEditorWindow::addTemplateComponent() {
    taets::BinaryTemplate * selected = selectedTemplate();
    taets::TemplateManager& manager = taets::TemplateManager::instance();
    selected->parameters().push_back(manager.prototypes[0]->clone());
    selected->textParts().push_back("");
    addParameterSelector(selected->parameters().size() - 1);
    addTextInput(selected->parameters().size());

    ui->removeItem->setEnabled(true);
    repopulateParameters();
}

void TemplateEditorWindow::removeTemplateComponent() {
    taets::BinaryTemplate * selected = selectedTemplate();

    // Nichts kann entfernt werden wenn nichts da ist
    if(selected->parameters().size() == 0) return;

    QLayout * layout = ui->templateStructure->layout();
    QWidget * widget = structureWidgets[structureWidgets.size() -1];
    structureWidgets.pop_back();
    layout->removeWidget(widget);
    delete widget;
    widget = structureWidgets[structureWidgets.size() -1];
    structureWidgets.pop_back();
    delete widget;
    // Der obige Block trennt auch automatisch die Verbindungen, die
    // durch connect() aufgebaut wurden

    selected->parameters().pop_back();
    selected->textParts().pop_back();

    if(selected->parameters().size() == 0)
        ui->removeItem->setEnabled(false);

    repopulateParameters();
}
