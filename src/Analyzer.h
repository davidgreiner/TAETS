#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include <AnalysisToolController.h>

namespace taets {

/**
 * Basisklasse für alle Analyzer.
 * Da alle Analyzer gleichzeitig als Controller für ihr SubWindow dienen und somit signals und slots unterstüzen müssen, erbt diese Klasse von QObject.
 */
class Analyzer : public QObject
{
    Q_OBJECT

private:
    AnalysisToolController *m_parent;

public:
    explicit Analyzer(AnalysisToolController *_parent, QObject *parent = NULL) : QObject(parent) {
        m_parent = _parent;
    }

    virtual ~Analyzer() {}

    AnalysisToolController *parent() {
        return m_parent;
    }

public slots:
    /**
     * Event, das bei einer Änderung des Vorschauframes aufgerufen wird.
     */
    virtual void onPreviewFrameChanged(unsigned int _frame) {
        (void)_frame;
    }

    /**
     * Event, das bei einer Anfrage zur Synchronisation zum Master Frame aufgerufen wird
     */
    virtual void onSyncToMasterFrame() {}
};

} //end namespace taets

#endif // ANALYZER_H
