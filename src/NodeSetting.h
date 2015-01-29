#ifndef TAETS_NODESETTING_H
#define TAETS_NODESETTING_H

#include <QWidget>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

namespace taets {

class NodeSettingWidgetFactory;


/**
 * Basisklasse für Knoteneinstellungen. Instanzen davon werden in der rechten Seitenleiste angezeigt, um spezifische Einstellugen für einenen Knoten vorzunehmen.
 */
class NodeSetting : public QObject {
    Q_OBJECT

    std::string _name;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & _name;
    }

  protected:
    NodeSetting() {

    }

  signals:
    void valueChanged();

  public:

    void setName(std::string name) {
        if(name.compare(_name) != 0) {
            this->_name = name;
            emit valueChanged();
        }
    }

    /**
     * @return Name des NodeSettings
     */
    virtual std::string name() const {
        return this->_name;
    }

    NodeSetting(std::string name);

    /**
     * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
     */
    virtual QWidget* createWidget(NodeSettingWidgetFactory* factory) = 0;

};

}
#endif
