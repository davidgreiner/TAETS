#ifndef TAETS_FILEPARAM_H
#define TAETS_FILEPARAM_H

#include "TemplateParameter.h"
#include "FileSetting.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

namespace taets {

class FileParam : public FileSetting, public TemplateParameter {
    Q_OBJECT

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        ar & boost::serialization::base_object<FileSetting>(*this);
        ar & boost::serialization::base_object<TemplateParameter>(*this);
    }

  public:
    /**
     * Gibt den FileParameter als String zurück
     */
    virtual std::string toString() {
        return filename();
    }

    FileParam(std::string name, std::string filename, std::string filter);
    FileParam(FileParam * fp);
    FileParam();

    virtual TemplateParameter * clone() {
        return new FileParam(this);
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

    virtual QWidget * createWidget(ParameterWidgetFactory *factory);
};

} // namespace taets
#endif
