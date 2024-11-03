#ifndef MEDIACONTROLL_H
#define MEDIACONTROLL_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QTime>

class Mediacontroll : public QObject
{
    Q_OBJECT
public:
    explicit Mediacontroll(QObject *parent = 0); // Constructor
    ~Mediacontroll();                            // Destructor

    void Next_volume_up_clicked(); // Next song
    void Next_volume_up_pressed(); // Increase volume
    void Next_volume_up_released();

    void Previous_volume_down_clicked(); // Previous song
    void Previous_volume_down_pressed(); // Decrease volume
    void Previous_volume_down_released();

    void Play_Pause(); // Pause or play the playback
    void Stop();       // Stop playback

    QMediaPlayer *getMplayer() const; // Get Mplayer

    void open(const QList<QUrl> urls); // Open media
    /* Get and set methods */
    bool getPause() const;
    void setPause(bool value);

signals:
    void positionChanged(QString); // When the current time changes
    void durationChanged(QString); // When the maximum time changes
    void volumeChanged(int);       // When the volume changes
    void MediaChanged();           // When the media changes

public slots:
    void Position(); // Slot for current position
    void Duration(); // Slot for current duration
    void Media();    // Slot for media
    void plus();     // Slot to increase volume
    void minus();    // Slot to decrease volume

private:
    QMediaPlayer *mplayer = NULL;    // Object of the QMediaPlayer class
    QMediaPlaylist *playlist = NULL; // Object of the QMediaPlaylist class
    QTimer *time = NULL;             // Object of the QTimer class
    bool pause;                      // Toggle for pause
    bool isPress;                    // Toggle for press status
};

#endif // MEDIACONTROLL_H
