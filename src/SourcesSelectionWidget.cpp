#include "SourcesSelectionWidget.h"

#include <QGridLayout>
#include <stdexcept>

SourcesSelectionWidget::SourcesSelectionWidget(QWidget *parent) :
    QGroupBox(parent)
{
    retranslate();
}

void SourcesSelectionWidget::setSources(std::list<std::pair<std::string, taets::DataSeries*> > sources)
{
    QGridLayout *layout = new QGridLayout(this);

    unsigned int row_index = 0;
    std::list<std::pair<std::string, taets::DataSeries*> >::iterator it;
    for(it = sources.begin(); it != sources.end(); it++, row_index++) {
        Entry e;
        e.series = it->second;

        e.checkbox = new QCheckBox(it->first.c_str());
        connect(e.checkbox, &QCheckBox::toggled, this, &SourcesSelectionWidget::checkboxToggled);

        e.enabled = false;

        e.color_box = new QLabel();
        e.color_box->setFrameShape(QFrame::StyledPanel);
        e.color_box->setFrameShadow(QFrame::Raised);
        e.color_box->setVisible(false);

        layout->addWidget(e.checkbox, row_index, 0);
        layout->addWidget(e.color_box, row_index, 1);

        _widgets.push_back(e);
    }

    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), row_index, 0, 1, 2);
}

void SourcesSelectionWidget::setColor(taets::DataSeries *_item, QColor _color)
{
    QLabel *l = getEntryWith(_item)->color_box;

    QPixmap pm(16, 16);
    pm.fill(_color);

    l->setPixmap(pm);
    l->setVisible(true);
}

void SourcesSelectionWidget::resetColor(taets::DataSeries *_item)
{
    QLabel *l = getEntryWith(_item)->color_box;
    l->setVisible(false);
}

void SourcesSelectionWidget::checkboxToggled(bool toggled)
{
    //find checkbox which changed its state
    std::vector<Entry>::iterator it;
    for(it = _widgets.begin(); it != _widgets.end(); it++) {
        if(it->checkbox->isChecked() != it->enabled) {
            it->enabled = it->checkbox->isChecked();
            emit selectedChanged(it->series, it->enabled);
            return;
        }
    }
    throw std::logic_error("no checkbox changed its state");
}

void SourcesSelectionWidget::retranslate()
{
    setTitle(tr("Sources"));
}

SourcesSelectionWidget::Entry *SourcesSelectionWidget::getEntryWith(taets::DataSeries *_series)
{
    std::vector<Entry>::iterator it;
    for(it = _widgets.begin(); it != _widgets.end(); it++) {
        if(it->series == _series) return &*it;
    }
    throw std::logic_error("no such entry");
}
