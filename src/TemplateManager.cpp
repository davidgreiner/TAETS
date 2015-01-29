#include "TemplateManager.h"
#include "NumberParam.h"
#include "ChoiceParam.h"
#include "FileParam.h"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <utility> //hehe, ist nicht unsere
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace taets {

const std::string TemplateManager::location = "res/templates";

TemplateManager& TemplateManager::instance() {
    static TemplateManager instance;
    return instance;
}

TemplateManager::TemplateManager() {
    prototypes.push_back(new NumberParam());
    prototypes.push_back(new ChoiceParam());
    prototypes.push_back(new FileParam());
    load();
}

void TemplateManager::load() {
    BOOST_FOREACH(BinaryTemplate * ptr, templates)
        delete ptr;

    templates.clear();


    boost::filesystem::path templDir(location);
    boost::filesystem::directory_iterator start(templDir), end;

    BOOST_FOREACH(const boost::filesystem::path& p, std::make_pair(start, end))
    {
        if(!is_regular_file(p))
            continue;

        try {
            templates.push_back(BinaryTemplate::load(p));
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
}

void TemplateManager::save() {
    boost::filesystem::path templDir(location);

    // Ordner leeren
    boost::filesystem::directory_iterator start(templDir), end;

    BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(start, end))
    {
        if(!is_regular_file(p))
            continue;
        remove(p);
    }

    BOOST_FOREACH(BinaryTemplate * ptr, templates) {
        std::string filename(ptr->name());
        std::replace(filename.begin(), filename.end(), ' ', '_');
        ptr->save(templDir / filename);
    }
}
}
