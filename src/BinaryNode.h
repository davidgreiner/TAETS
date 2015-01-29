#ifndef TAETS_BINARYNODE_H
#define TAETS_BINARYNODE_H

#include "ModifierNode.h"
#include "BinaryTemplate.h"
#include "TemplateManager.h"
#include <QCoreApplication>
#include <QObject>

namespace taets {

class BinaryTemplate;
class Graph;
class ImageSeries;

class BinaryNode : public ModifierNode {
    Q_DECLARE_TR_FUNCTIONS(BinaryNode)

    friend class boost::serialization::access;

    template<class Archive> void save(Archive &ar, const unsigned int) const {
        ar << boost::serialization::base_object<ModifierNode>(*this);
        ar << _template;
    }

    template<class Archive> void load(Archive &ar, const unsigned int) {
        ar >> boost::serialization::base_object<ModifierNode>(*this);

        BinaryTemplate *templ;
        ar >> templ;

        //If template exists in the TemplateManager, take it, else use serialized template as "backup"
        try {
            _template = TemplateManager::instance().getTemplate(templ->name());
        } catch(std::invalid_argument) {
            _template = templ;
        }
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    BinaryNode() {}

  public:
    /**
     * Führt eine binäre Datei aus, die Bilder verarbeitet
     */
    virtual void compute();

    BinaryNode(BinaryTemplate * _tmpl, Graph * graph, unsigned int id);
  private:
    void processVideo(std::string command, ImageSeries * series, ImageSeries * output);
    BinaryTemplate * _template;

};

} // namespace taets
#endif
