#ifndef TAETS_OUTPUTCONNECTOR_H
#define TAETS_OUTPUTCONNECTOR_H

#include "OutputConnectorBase.h"
#include <typeinfo>

namespace taets {

template<class T>
class OutputConnector : public OutputConnectorBase {
  private:
    T * _data;
    T * _previewData;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<OutputConnectorBase>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    OutputConnector() {}

  public:
    OutputConnector(Node * node, const std::string & name) : OutputConnectorBase(node, name) {
        _data = NULL;
    }

    void setData(T* data) { this->_data = data; }
    void setPreviewData(T* previewData) {this->_previewData = previewData; }

    T * data() { return _data; }
    T * previewData() { return _previewData; }

    virtual std::string typeinfo() {
        return std::string(typeid(T).name());
    }
};

}
#endif
