#ifndef TAETS_CHOICESETTING_H
#define TAETS_CHOICESETTING_H


#include <stdexcept>
#include "NodeSetting.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

namespace taets {

class NodeSettingWidgetFactory;

class ChoiceSetting : public NodeSetting {
    Q_OBJECT

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & boost::serialization::base_object<NodeSetting>(*this);
        ar & _selection;
        ar & _choices;
        ar & _dropdown;
    }
  protected:
    ChoiceSetting() {}

    /**
     * Auswählbare Elemente.
     */
    std::vector<std::string> _choices;
    int _selection;
    bool _dropdown;

  signals:
    void selectionChanged(int index);

  public:
    /**
     * Gibt den Index des ausgewählten Elements zurück.
     */
    unsigned int selection() const {
        return this->_selection;
    }

    /**
     * Gibt den String des ausgewählten Elements zurück.
     */
    std::string selectedString() const {
        return _choices[_selection];
    }

    /**
      * Gibt die verfügbaren Wahlmöglichkeiten
      */
    std::vector<std::string> choices() const {
        return this->_choices;
    }

    /**
     * Gibt zurück, ob diese ChoiceSetting durch ein Dropdown-Menü oder durch
     * RadioButtons angezeigt werden soll.
     */
    bool isDropDown() {
        return _dropdown;
    }

    /**
     * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
     */
    virtual QWidget* createWidget(NodeSettingWidgetFactory* factory);

    ChoiceSetting(std::string name, std::vector<std::string> choices, int selection, bool dropdown = false);

  public slots:
    void setSelection(int selection) {
        if(selection >= _choices.size())
            throw std::range_error("Selection out of range");
        this->_selection = selection;
        emit selectionChanged(selection);
        emit valueChanged();
    }
};

} // namespace taets
#endif
