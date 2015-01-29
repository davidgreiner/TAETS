#ifndef LANGUAGEHANDLER_H
#define LANGUAGEHANDLER_H

#include <QLocale>
#include <QTranslator>
#include <QSettings>

/**
 * Singleton Klasse, um die UI-Sprache und -änderungen zu unterstützen.
 */
class LanguageHandler
{
public:
    static LanguageHandler *instance() {
        if(_instance) return _instance;
        else return (_instance = new LanguageHandler);
    }

    void loadLanguageSettings();
    void storeLanguageSettings();

    void setLanguage(QLocale::Language language);

private:
    LanguageHandler();

    static LanguageHandler *_instance;
    QTranslator _translator;
    QLocale::Language _language;
    QSettings _settings;
};

#endif // LANGUAGEHANDLER_H
