#ifndef TAETS_FILESETTING_H
#define TAETS_FILESETTING_H


#include "NodeSetting.h"
#include <QObject>
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

namespace taets {
class NodeSettingWidgetFactory;

class FileSetting : public NodeSetting {
    Q_OBJECT

    std::string _filename;
    std::string _filter;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & boost::serialization::base_object<NodeSetting>(*this);
        ar & _filename;
        ar & _filter;
    }

  protected:
    FileSetting() {}

  signals:
    void filenameChanged(std::string filename);

  public:
    std::string filename() {
        return this->_filename;
    }

    /**
     * Erstellt ein zum konkreten Setting passendes Widget mithilfe der Abstrakten Fabrik.
     */
    virtual QWidget* createWidget(NodeSettingWidgetFactory* factory);

    FileSetting(std::string name, std::string filename, std::string filter);

  public slots:
    void setFilename(std::string filename) {
        if(filename.compare(_filename) != 0) {
            _filename = filename;
            emit valueChanged();
        }
    }

    void setFilename(const QString& filename) {
        setFilename(filename.toStdString());
    }

    void setFilename(const char filename[]) {
        setFilename(std::string(filename));
    }
};

} // namespace taets
#endif
