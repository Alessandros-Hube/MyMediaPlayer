#include "Mediacontroll.h"
/**
 * @brief Mediacontroll::Mediacontroll
 * @param parent
 * The constructor initializes mplayer,
 * playlist, and time with new. Sets the playlist in mplayer,
 * connects the playlist signal to the Media slot,
 * and initializes pause and isPress to false.
 */
Mediacontroll::Mediacontroll(QObject *parent) : QObject(parent)
{
    mplayer = new QMediaPlayer(parent);

    playlist = new QMediaPlaylist(parent);
    mplayer->setPlaylist(playlist);

    connect(playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(Media()));

    time = new QTimer(parent);
    pause = false;
    isPress = false;
}

/**
 * @brief Mediacontroll::~Mediacontroll
 * Deletes mplayer, playlist, and time.
 */
Mediacontroll::~Mediacontroll()
{
    delete mplayer;
    delete playlist;
    delete time;
}

/**
 * @brief Mediacontroll::Position
 * Converts the current playback position
 * and emits positionChanged(...)
 * with the current position.
 */
void Mediacontroll::Position()
{
    qint64 pos = mplayer->position() / 1000;

    QTime currentTime((pos / 3600) % 60, (pos / 60) % 60,
                      pos % 60, (pos * 1000) % 1000);

    emit positionChanged(currentTime.toString("mm:ss"));
}

/**
 * @brief Mediacontroll::Duration
 * Converts the maximum duration
 * and emits durationChanged(...)
 * with the current duration.
 */
void Mediacontroll::Duration()
{
    qint64 pos = mplayer->duration() / 1000;

    QTime totalTime((pos / 3600) % 60, (pos / 60) % 60,
                    pos % 60, (pos * 1000) % 1000);

    emit durationChanged(totalTime.toString("mm:ss"));
}

/**
 * @brief Mediacontroll::Media
 * Emits MediaChanged().
 */
void Mediacontroll::Media()
{
    emit MediaChanged();
}

/**
 * @brief Mediacontroll::open
 * @param urls
 * Adds the list of urls to the playlist
 * and plays the next song.
 */
void Mediacontroll::open(const QList<QUrl> urls)
{
    for (int i = 0; i < urls.length(); i++)
    {
        playlist->addMedia(urls.at(i));
    }
    mplayer->playlist()->next();
    pause = true;
}

/**
 * @brief Mediacontroll::Play_Pause
 * Plays or pauses playback
 * depending on the pause toggle.
 */
void Mediacontroll::Play_Pause()
{
    if (pause)
    {
        pause = false;
        mplayer->play();
    }
    else
    {
        pause = true;
        mplayer->pause();
    }
}

/**
 * @brief Mediacontroll::Stop
 * Stops the current playback.
 */
void Mediacontroll::Stop()
{
    pause = false;
    Play_Pause();
    mplayer->stop();
}

/**
 * @brief Mediacontroll::Next_volume_up_clicked
 * Stops the current playback
 * and plays the next song.
 */
void Mediacontroll::Next_volume_up_clicked()
{
    if (!isPress)
    {
        Stop();
        mplayer->playlist()->next();
        Play_Pause();
    }
    else
    {
        isPress = 0;
    }
}

/**
 * @brief Mediacontroll::Next_volume_up_pressed
 * Disconnects time from the minus slot,
 * connects time to the plus slot,
 * and starts time with 250 ms intervals.
 */
void Mediacontroll::Next_volume_up_pressed()
{
    disconnect(time, &QTimer::timeout, this, &Mediacontroll::minus);
    connect(time, &QTimer::timeout, this, &Mediacontroll::plus);
    time->start(250);
}

/**
 * @brief Mediacontroll::plus
 * Increases the volume
 * and emits volumeChanged(...)
 * with the current volume.
 */
void Mediacontroll::plus()
{
    mplayer->setVolume(mplayer->volume() + 5);
    isPress = true;
    emit volumeChanged(mplayer->volume());
}

/**
 * @brief Mediacontroll::Next_volume_up_released
 * Stops the timer.
 */
void Mediacontroll::Next_volume_up_released()
{
    time->stop();
}

/**
 * @brief Mediacontroll::Previous_volume_down_clicked
 * Stops the current playback
 * and plays the previous song.
 */
void Mediacontroll::Previous_volume_down_clicked()
{
    if (!isPress)
    {
        Stop();
        mplayer->playlist()->previous();
        Play_Pause();
    }
    else
    {
        isPress = 0;
    }
}

/**
 * @brief Mediacontroll::Previous_volume_down_pressed
 * Disconnects time from the plus slot,
 * connects time to the minus slot,
 * and starts time with 250 ms intervals.
 */
void Mediacontroll::Previous_volume_down_pressed()
{
    disconnect(time, &QTimer::timeout, this, &Mediacontroll::plus);
    connect(time, &QTimer::timeout, this, &Mediacontroll::minus);
    time->start(250);
}

/**
 * @brief Mediacontroll::minus
 * Decreases the volume
 * and emits volumeChanged(...)
 * with the current volume.
 */
void Mediacontroll::minus()
{
    mplayer->setVolume(mplayer->volume() - 5);
    isPress = true;
    emit volumeChanged(mplayer->volume());
}

/**
 * @brief Mediacontroll::Previous_volume_down_released
 * Stops the timer.
 */
void Mediacontroll::Previous_volume_down_released()
{
    time->stop();
}

/**
 * Set and get methods for all attributes.
 */
bool Mediacontroll::getPause() const
{
    return pause;
}

void Mediacontroll::setPause(bool value)
{
    pause = value;
}

QMediaPlayer *Mediacontroll::getMplayer() const
{
    return mplayer;
}
