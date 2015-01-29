#include "boost_class_exports.h"
#include "BinaryTemplate.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <boost/filesystem/fstream.hpp>

namespace taets {

std::string BinaryTemplate::toString() {
    std::ostringstream ss;
    for(int i = 0; i < _params.size(); i++) {
        ss << _text[i];
        ss << _params[i]->toString();
    }
    ss << _text[_text.size() - 1];
    return ss.str();
}

BinaryTemplate::BinaryTemplate() : _name(tr("Template").toStdString()), _yuv(true), _perFrame(false) {
    _text.push_back("");
}

BinaryTemplate::BinaryTemplate(std::string name, std::vector<std::string> text, std::vector<TemplateParameter *> params, bool yuv, bool perFrame)
    : _name(name), _text(text), _params(params), _yuv(yuv), _perFrame(perFrame) {
    if(_text.size() != _params.size() + 1) {
        throw std::runtime_error(std::string("Text must be one larger than parameters"));
    }
}

BinaryTemplate * BinaryTemplate::derive() {
    std::vector<TemplateParameter *> copy;
    for(TemplateParameter * elem : _params) {
        copy.push_back(elem->clone());
    }
    return new BinaryTemplate(_name, _text, copy, _yuv, _perFrame);
}

void BinaryTemplate::save(const boost::filesystem::path& path) {
    boost::filesystem::ofstream os(path);
    boost::archive::text_oarchive ar(os);
    ar << *this;
}

BinaryTemplate * BinaryTemplate::load(const boost::filesystem::path& path) {
    boost::filesystem::ifstream is(path);
    boost::archive::text_iarchive ar(is);
    BinaryTemplate * templ = new BinaryTemplate();
    ar >> *templ;
    return templ;
}

BinaryTemplate::~BinaryTemplate() {
    for(TemplateParameter * param : _params) {
        delete param;
    }
}

} // namespace taets
