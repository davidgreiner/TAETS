#ifndef TIMEMANIPULATOR_H
#define TIMEMANIPULATOR_H

#include <QObject>
#include <QTimer>

class TimeManipulator : public QObject
{
    Q_OBJECT
public:
    explicit TimeManipulator(unsigned int _length, unsigned int _interval, bool _loop = true, QObject *parent = 0);
    ~TimeManipulator();

    bool playing() {
        return m_playing;
    }

    unsigned int frame() {
        return m_frame;
    }

    unsigned int length() {
        return m_length;
    }

    bool loop() {
        return m_loop;
    }

signals:
    /**
     * Wird emitted, wenn das loop()-Attribut verändert wurde.
     */
    void loopChanged(bool _loop);

    /**
     * Wird emitted, wenn das frame()-Attribut verändert wurde und ein neuer Frame angzuzeigen ist.
     */
    void frameChanged(unsigned int _frame);

    /**
     * Wird emitted, wenn das playing()-Attribut verändert wurde.
     */
    void playingChanged(bool _playing);

    /**
     * Wird emitted, wenn das Video durch die Loop-Funktion zurück an den Anfang springt.
     * Achtung! Ein Handler für dieses Signal darf nicht auf das frame()-Attribut zugreifen! Es ist invalide!
     */
    void looped();

public slots:
    /**
     * Startet oder stoppt die Wiedergabe.
     */
    void setPlaying(bool _playing);

    /**
     * Stoppt die Wiedergabe und springt zurück zum Anfang.
     */
    void stop() {
        setPlaying(false);
        seekFirst();
    }

    /**
     * Springt ein Bild vorwärts.
     */
    void step() {
        setFrame(frame() + 1);
    }

    /**
     * Springt ein Bild zurück.
     */
    void stepReverse() {
        if(frame() > 0)
            setFrame(frame() - 1);
    }

    /**
     * Springt an den Anfang.
     */
    void seekFirst() {
        setFrame(0);
    }

    /**
     * Springt an das Ende.
     */
    void seekLast() {
        setFrame(length() - 1);
    }

    /**
     * Aktualisiert die Länge. frameChanged wird aufgerufen, wenn der aktuelle Frame außerhalb des Bereichs liegt.
     */
    void setLength(unsigned int _length);

    /**
     * Springt an einen bestimmten Frame. Ist die Eingabe zu groß, wird an den letzen Frame des Videos gesprungen.
     * Diese Methode wird auch von nextFrame() genutzt. Allerdings wird dann richtiger Input garantiert.
     */
    void setFrame(unsigned int _frame);

    /**
     * Aktiviert bzw. Deaktiviert die Loop-Funktion
     */
    void setLoop(bool _loop);

private slots:
    /**
     * Wird vom Timer aufgerufen, wenn der nächste Frame anzuzeigen ist.
     */
    void nextFrame();

private:
    unsigned int m_length;
    unsigned int m_interval;
    bool m_loop;

    unsigned int m_frame;
    bool m_playing;

    QTimer *m_timer;
};

#endif // TIMEMANIPULATOR_H
