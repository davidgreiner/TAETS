#include "ParameterWidgetFactory.h"
#include <QWidget>
#include "FileParam.h"

namespace taets {

FileParam::FileParam(std::string name, std::string filename, std::string filter)
    : FileSetting(name, filename, filter) {

}


FileParam::FileParam(FileParam * fp)
    : FileSetting(fp->name(), fp->filename(), "All Files (*.*)") {
}

FileParam::FileParam() : FileSetting(tr("File Parameter").toStdString(), "/dev/null", "All Files (*.*)") {}

QWidget * FileParam::createWidget(ParameterWidgetFactory *factory) {
    return factory->createFileWidget(this);
}

std::string FileParam::id() {
    return tr("File Parameter").toStdString();
}

} // namespace taets
