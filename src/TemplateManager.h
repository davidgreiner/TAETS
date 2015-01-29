#ifndef TEMPLATEMANAGER_H
#define TEMPLATEMANAGER_H
#include <vector>
#include <string>
#include "TemplateParameter.h"
#include "BinaryTemplate.h"

namespace taets {

class TemplateManager
{
    TemplateManager();
    TemplateManager(TemplateManager const&); // Do not implement
    void operator=(TemplateManager const&); //Do not implement
public:
    static TemplateManager& instance();
    static const std::string location;

    std::vector<TemplateParameter * > prototypes;
    std::vector<BinaryTemplate * > templates;

    BinaryTemplate * getTemplate(std::string name) {
        for(auto t : templates) {
            if(t->name() == name) return t;
        }
        throw std::invalid_argument("requested template does not exist");
    }

    void load();

    void save();
};

}
#endif // TEMPLATEMANAGER_H
