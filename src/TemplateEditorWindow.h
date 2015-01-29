#ifndef TEMPLATEEDITORWINDOW_H
#define TEMPLATEEDITORWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include "BinaryTemplate.h"
#include <vector>
#include "ParameterWidgetFactory.h"

namespace Ui {
class TemplateEditorWindow;
}

class TemplateEditorWindow : public QMainWindow
{
    Q_OBJECT

    /**
     * Bietet einen alias auf das (weitaus längere)
     * taets::TemplateManager::instance().templates
     */
    std::vector<taets::BinaryTemplate *> & templates;
    std::vector<QWidget *> parameterWidgets;
    std::vector<QWidget *> structureWidgets;

    taets::ParameterWidgetFactory * factory;

    /**
     * Baut die Liste der Templates nach einem erneuten Laden neu auf
     */
    void repopulateList();

    /**
     * Baut die Widgets, die die Parameter bearbeiten neu auf
     */
    void repopulateParameters();

    /**
     * Fügt einen QLineEdit hinzu, der zum eingeben eines Textteils des Templates dient
     */
    void addTextInput(int paramIndex);

    /**
     * Fügt eine QComboBox hinzu, die zum auswählen eines Parametertyps des Templates dient
     */
    void addParameterSelector(int paramIndex);

    /**
     * Gemütlichkeitsmethode. Gibt das derzeit ausgewählte BinaryTemplate.
     */
    taets::BinaryTemplate * selectedTemplate();

public:
    explicit TemplateEditorWindow(QWidget *parent = 0);
    ~TemplateEditorWindow();

    void changeEvent(QEvent *event);

private:
    Ui::TemplateEditorWindow *ui;

public slots:
    /**
     * Wähle ein anderes Template aus und baue das Interface neu auf
     */
    void changeTemplateSelection();

    /**
     * Bearbeite den Namen des ausgewählten templates
     */
    void editTemplateName(const QModelIndex & topleft, const QModelIndex & bottomRight);

    /**
     * Speichere alle Templates auf die Festplatte
     * ACHTUNG: Die Boost version ist hier von großer Bedeutung
     */
    void saveChanges();

    /**
     * Lade alle Templates neu und verwirf alle ungespeicherten Veränderungen
     */
    void discardChanges();
    void newTemplate();
    void deleteTemplate();
    void addTemplateComponent();
    void removeTemplateComponent();
    void changeParameterType(int paramIndex, int typeIndex);
    void editTextPart(int paramIndex, const QString & text);
    void toggleYUV(bool state);
    void togglePerFrame(bool state);
};

#endif // TEMPLATEEDITORWINDOW_H
