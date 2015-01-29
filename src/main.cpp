#include <QApplication>

#include "GraphEditorWindow.h"
#include "LanguageHandler.h"

static QTranslator translator;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LanguageHandler::instance()->loadLanguageSettings();

    GraphEditorWindow *window = new GraphEditorWindow();
    window->setWindowIcon(QIcon("res/icons/logo.png"));
    window->showMaximized();

    return app.exec();
}
