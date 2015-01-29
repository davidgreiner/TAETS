#include "TimeManipulator.h"

TimeManipulator::TimeManipulator(unsigned int _length, unsigned int _interval, bool _loop, QObject *parent) :
    QObject(parent)
{
    m_length = _length;
    m_interval = _interval;
    m_loop = _loop;

    m_frame = 0;
    m_playing = false;

    m_timer = new QTimer();
    m_timer->setInterval(_interval);
    connect(m_timer, &QTimer::timeout, this, &TimeManipulator::nextFrame);
}

TimeManipulator::~TimeManipulator() {
    delete m_timer;
}

void TimeManipulator::setPlaying(bool _playing)
{
    // no state change
    if(m_playing == _playing)
        return;

    if(m_playing) {
        m_timer->stop();
    } else {
        m_timer->start();
    }

    m_playing = _playing;

    emit playingChanged(m_playing);
}

void TimeManipulator::setLength(unsigned int _length)
{
    if(m_length == _length)
        return;

    m_length = _length;

    if(m_frame >= _length)
        seekLast();
}

void TimeManipulator::setFrame(unsigned int _frame)
{
    // no state change
    if(m_frame == _frame)
        return;

    if(_frame >= m_length) {
        seekLast(); // recursion to suppress unnecessary state changes
        return;
    }

    m_frame = _frame;

    emit frameChanged(m_frame);
}

void TimeManipulator::setLoop(bool _loop)
{
    if(m_loop != _loop) {
        m_loop = _loop;
        emit loopChanged(m_loop);
    }
}

void TimeManipulator::nextFrame()
{
    unsigned int new_frame = m_frame + 1;

    // loop or halt playback, if we reached the end of the video
    if(new_frame >= m_length) {
        if(m_loop) {
            new_frame = 0;
            emit looped(); // frame attribute is invalid!
        } else {
            setPlaying(false);
            return;
        }
    }

    setFrame(new_frame);
}
