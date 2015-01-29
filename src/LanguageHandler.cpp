#include "LanguageHandler.h"

#include <QApplication>

#include <iostream>

LanguageHandler *LanguageHandler::_instance = NULL;

LanguageHandler::LanguageHandler() : _settings("edu.kit.itec.ces", "TAETS") {
    QApplication::installTranslator(&_translator);
    _language = QLocale::AnyLanguage;
}

void LanguageHandler::loadLanguageSettings() {
    QLocale locale = _settings.value("language", QLocale::system()).toLocale();
    setLanguage(locale.language());
}

void LanguageHandler::storeLanguageSettings() {
    _settings.setValue("language", QLocale(_language));
}

void LanguageHandler::setLanguage(QLocale::Language language) {
    if(_language == language) return;

    _language = language;
    QLocale locale(_language);
    if(!_translator.load(locale, "taets", "_", ":/languages"))
    {
        std::cout << "no language file found for language " << QLocale::languageToString(_language).toStdString() << std::endl;
        return;
    }

    storeLanguageSettings();
}
