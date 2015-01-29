#ifndef TAETS_CHOICEPARAM_H
#define TAETS_CHOICEPARAM_H

#include "TemplateParameter.h"
#include "ChoiceSetting.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

namespace taets {

class ChoiceParam : public ChoiceSetting, public TemplateParameter {
    Q_OBJECT

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & boost::serialization::base_object<ChoiceSetting>(*this);
        ar & boost::serialization::base_object<TemplateParameter>(*this);
    }

  public:
    /**
     * Gibt den ChoiceParameter als String zurück
     */
    virtual std::string toString() {
        return selectedString();
    }

    ChoiceParam(std::string name, std::vector<std::string> choices, int choice);
    ChoiceParam(ChoiceParam *cp);
    ChoiceParam();

    virtual TemplateParameter * clone() {
        return new ChoiceParam(this);
    }

    virtual std::string name() {
        return ((NodeSetting *) this)->name();
    }

    virtual void setName(std::string name) {
        ((NodeSetting *) this)->setName(name);
    }

    virtual NodeSetting * toNodeSetting() {
        return this;
    }

    virtual std::string id();

    virtual QWidget * createWidget(ParameterWidgetFactory * factory);

  public slots:
    void choiceAdded(std::string choice) {
        _choices.push_back(choice);
        emit valueChanged();
    }

    void choiceChanged(int index, std::string choice) {
        _choices[index] = choice;
        emit valueChanged();
    }

    // TODO: Sanity Check der selection
    void choiceRemoved() {
        _choices.pop_back();
        emit valueChanged();
    }

    void defaultChanged(int index) {
        setSelection(index);
    }

    void dropdownChanged(bool dropdown) {
        if(dropdown != _dropdown) {
            _dropdown = dropdown;
            emit valueChanged();
        }

    }
};

} // namespace taets
#endif
