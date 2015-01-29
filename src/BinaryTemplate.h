#ifndef TAETS_BINARYTEMPLATE_H
#define TAETS_BINARYTEMPLATE_H

#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/filesystem.hpp>
#include <TemplateParameter.h>
#include <QCoreApplication>

namespace taets {

/**
 * Die parametrisierte Beschreibung eines Kommandozeilenbefehls
 */
class BinaryTemplate {
    Q_DECLARE_TR_FUNCTIONS(BinaryTemplate)

    /**
     * Die Parameter, die mit den strings abgewechselt werden
     */
    std::vector<TemplateParameter*> _params;

    /**
     * Die Strings, die zwischen den Paramatern stehen
     */
    std::vector<std::string> _text;

    /**
     * Der Name des Templates
     */
    std::string _name;

    /**
     * Ob dieses Template auf YUV Daten oder auf RGB Daten ausgeführt werden soll.
     */
    bool _yuv;

    /**
     * Ob dieses Template auf einer pro-Frame oder einer pro-Video basis ausgeführt wird.
     */
    bool _perFrame;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        (void)version;

        ar & _name;
        ar & _yuv;
        ar & _perFrame;
        ar & _text;
        ar & _params;
    }

  public:
    /**
     * Löse dieses Template zu einer tatsächlichen Kommandozeile auf
     */
    std::string toString();

    BinaryTemplate(std::string name, std::vector<std::string> text, std::vector<TemplateParameter*> params, bool yuv = true, bool perFrame = false);
    BinaryTemplate();

    ~BinaryTemplate();

    /**
     * Erstelle eine Kopie dieses Templates, inklusive Kopien der Parameter und deren Eigenschaften.
     */
    BinaryTemplate * derive();

    /**
     * Gibt den Namen des Templates
     */
    std::string name() { return _name; }

    bool yuv() { return _yuv; }

    void setYUV(bool yuv) { _yuv = yuv; }

    bool perFrame() { return _perFrame; }
    void setPerFrame(bool perFrame) { _perFrame = perFrame; }

    void setName(std::string name) {
        this->_name = name;
    }

    void save(const boost::filesystem::path& path);

    std::vector<TemplateParameter *>& parameters() { return this->_params; }
    std::vector<std::string>& textParts() { return this->_text; }

    static BinaryTemplate * load(const boost::filesystem::path& path);
};

} // namespace taets
#endif
