#ifndef TAETS_NUMBERSETTING_H
#define TAETS_NUMBERSETTING_H


#include "NodeSetting.h"
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <stdexcept>

namespace taets {

class NodeSettingWidgetFactory;

/**
 * Knoteneinstellung in Form einer Fließkommazahl
 */
class NumberSetting : public NodeSetting {
    Q_OBJECT

    float _value;
    float _min;
    float _max;
    bool _is_integer;
    bool _is_spinner;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & boost::serialization::base_object<NodeSetting>(*this);
        ar & _value;
        ar & _min;
        ar & _max;
        ar & _is_integer;
        ar & _is_spinner;
    }

  protected:
    void setMinimum(float min){
        this->_min = min;
    }

    void setMaximum(float max){
        this->_max = max;
    }

    void setInt(bool isInt){
        this->_is_integer = isInt;
    }

    void setSpinner(bool isSpinner){
        this->_is_spinner = isSpinner;
    }

    NumberSetting();

  public:
    NumberSetting(std::string name, float min, float max, float value, bool isInt, bool isSpinner);

    /**
     * Gibt den Wert als Integer zurück.
     */
    int intValue() const {
        return (int) this->_value;
    }

    /**
     * Gibt den Wert als Fließkommazahl zurück.
     */
    float floatValue() const {
        return this->_value;
    }

    /**
     * Gibt true, wenn diese Einstellung einen Integer darstellt
     */
    bool isInteger() const {
        return this->_is_integer;
    }

    /**
     * Gibt den Minimal erlaubten Wert der Setting zurück
     */
    float minimum() const {
        return this->_min;
    }

    /**
     * Gibt den Minimal erlaubten Wert der Setting zurück
     */
    float maximum() const {
        return this->_max;
    }

    /**
     * Gibt true, wenn diese Einstellung durch einen Spinner dargestellt werden soll
     */
    bool isSpinner() const {
        return this->_is_spinner;
    }

    virtual QWidget* createWidget(NodeSettingWidgetFactory* factory);
  public slots:
    void setIntValue(int value) {
        _value = value;
    }

    void setFloatValue(float value) {
        if(isInteger())
            throw std::domain_error("setting is an integer");
        _value = value;
    }
};
}

#endif
