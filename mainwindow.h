#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QSlider>
#include <QVideoWidget>
#include <QFileDialog>
#include <QMainWindow>

#include "mediacontroll.h"
#include "serallport.h"

#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0); // Constructor
    ~MainWindow(); // Destructor

public slots:

    void moved(); // Slot for slider movement

    void resize(); // Slot for size adjustment

    void Button_Switch(SerallPort::Button val); // Slot for controlling the UI via serial

    void Media_Update(); // Slot for media update in status bar

protected:

    void keyPressEvent(QKeyEvent *e); // Key press

    void keyReleaseEvent(QKeyEvent *); // Key release

private slots:

    void on_open_clicked(); // Slot for Open button

    void on_Play_Pause_clicked(); // Slot for Play/Pause button

    void on_stop_clicked(); // Slot for Stop button

    void on_dial_valueChanged(int value); // Slot for changing dial value (volume adjustment)

    void on_Next_volume_up_pressed(); // Slot for Next volume up button pressed

    void on_Previous_volume_down_pressed(); // Slot for Previous volume down button pressed

    void on_Previous_volume_down_released(); // Slot for Previous volume down button released

    void on_Next_volume_up_released(); // Slot for Next volume up button released

    void on_Next_volume_up_clicked(); // Slot for Next volume up button clicked

    void on_Previous_volume_down_clicked(); // Slot for Previous volume down button clicked

private:
    Ui::MainWindow *ui;

    QVideoWidget *mvideo = NULL; // Video widget for video playback
    QSlider *slider = NULL; // Slider for displaying the current playback position
    QLabel *take = NULL; // Label for the track number
    QLabel *placeholder = NULL; // This label holds a spacer in the status bar
    QString title; // Storage location for the title of the current playback

    Mediacontroll *Play = NULL; // An object of the Mediacontroll class

    SerallPort *serall = NULL; // An object of the SerallPort class
};

#endif // MAINWINDOW_H
