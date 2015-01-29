#ifndef TAETS_NUMBERPARAM_H
#define TAETS_NUMBERPARAM_H


#include "TemplateParameter.h"
#include "NumberSetting.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

namespace taets {

class NumberParam : public NumberSetting, public TemplateParameter {
    Q_OBJECT

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & boost::serialization::base_object<NumberSetting>(*this);
        ar & boost::serialization::base_object<TemplateParameter>(*this);
    }

  public:
    virtual std::string toString();

    NumberParam(std::string name, float min, float max, float value, bool is_integer, bool is_spinner);
    NumberParam(NumberParam * np);
    NumberParam();

    virtual TemplateParameter * clone() {
        return new NumberParam(this);
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

    // TODO: dies entfernen
    virtual std::string id();

    virtual QWidget * createWidget(ParameterWidgetFactory *factory);

  public slots:
    void minimumChanged(float min) {
        setMinimum(min);
    }

    void maximumChanged(float max) {
        setMaximum(max);
    }

    void valueChanged(float value) {
        if(!isInteger())
            setFloatValue(value);
        else
            setIntValue((int) value);
    }

    void isIntChanged(bool isInt) {
        setInt(isInt);
    }

    void isIntChanged(int state) {
        isIntChanged(state == Qt::Checked);
    }

    void isSpinnerChanged(bool isSpinner) {
        setSpinner(isSpinner);
    }

    void isSpinnerChanged(int state) {
        isSpinnerChanged(state == Qt::Checked);
    }

    void nameChanged(std::string name) {
        setName(name);
    }

};

} // namespace taets


#endif
