#ifndef TAETS_TEMPLATEPARAMETER_H
#define TAETS_TEMPLATEPARAMETER_H

#include <QWidget>
#include "NodeSetting.h"
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>

namespace taets {
class ParameterWidgetFactory;

class TemplateParameter {

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int file_version){
        (void) file_version;
        (void) ar;
    }

  protected:
    TemplateParameter() {}
  public:
    /**
     * Gibt den TemplateParameter als String zurück
     */
    virtual std::string toString() = 0;

    /**
      Gibt den Namen des Parameters zurück
     **/
    virtual std::string name() = 0;

    virtual void setName(std::string name) = 0;

    /**
     * Gibt einen Klon dieses Parameters zurück
     */
    virtual TemplateParameter * clone() = 0;

    /**
     * Gibt den Namen des Parametertyps zurück
     */
    virtual std::string id() = 0;

    /**
     * Gibt die diesem Parameter entsprechende NodeSetting zurück
     */
    virtual NodeSetting * toNodeSetting() = 0;

    virtual QWidget * createWidget(ParameterWidgetFactory * factory) = 0;

    virtual ~TemplateParameter() {}
};

} // namespace taets
#endif
