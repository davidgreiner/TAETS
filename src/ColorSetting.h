#ifndef TAETS_COLORSETTING_H
#define TAETS_COLORSETTING_H

#include "Color.h"
#include "NodeSetting.h"
#include <QObject>
#include <string>

namespace taets {

class NodeSettingWidgetFactory;

class ColorSetting : public NodeSetting {
    Q_OBJECT

    Color _color;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<NodeSetting>(*this);
        ar & _color;
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    ColorSetting() {}

  public:
    /**
     * Gibt die ausgewählte Farbe zurück.
     */
    Color color() {
        return this->_color;
    }

    /**
     * Gibt den Rotwer der Farbe zurück.
     */
    float red() {
        return this->_color.red();
    }

    /**
     * Gibt den Rotwer der Farbe zurück.
     */
    float blue() {
        return this->_color.blue();
    }

    /**
     * Gibt den Rotwer der Farbe zurück.
     */
    float green() {
        return this->_color.green();
    }

    /**
     * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
     */
    virtual QWidget* createWidget(NodeSettingWidgetFactory* factory);

    ColorSetting(std::string name, Color color);

  public slots:
    void setColor(Color color) {
        if(_color != color) {
            this->_color = color;
            emit valueChanged();
        }
    }

};

} // namespace taets
#endif
