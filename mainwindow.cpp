#include "mainwindow.h"
#include "ui_mainwindow.h"
/**
 * @brief MainWindow::MainWindow
 * @param parent
 * The constructor initializes all pointer attributes
 * with new and sets up some signal and slot connections.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    take = new QLabel(this);
    take->setGeometry(0, 0, 10, 10); // Setting label size

    Play = new Mediacontroll(this);

    serall = new SerallPort(this);
    connect(Play, &Mediacontroll::positionChanged, serall, &SerallPort::Arduino_Output);   // This connection ensures that the current playback position is sent to the Arduino
    connect(serall, SIGNAL(deviceOpen(bool)), ui->centralWidget, SLOT(setEnabled(bool)));  // Disables the UI when an Arduino is connected (works only at program start)
    connect(serall, SIGNAL(deviceClose(bool)), ui->centralWidget, SLOT(setEnabled(bool))); // Enables the UI when the Arduino is disconnected from the computer
    connect(serall, &SerallPort::Button_Input, this, &MainWindow::Button_Switch);          // The button press received from the Arduino is passed to Button_Switch
    serall->open();                                                                        // Opens the serial connection to the Arduino

    placeholder = new QLabel(this);
    placeholder->setGeometry(0, 0, 0, 0); // Setting label size

    mvideo = new QVideoWidget(this);
    Play->getMplayer()->setVideoOutput(mvideo); // Sets the video widget in Play
    ui->l->insertWidget(0, mvideo);             // Inserts the video widget into a layout in the UI

    slider = new QSlider(this);
    slider->setGeometry(10, 80, 249, 19);            // Setting slider size
    slider->setOrientation(Qt::Horizontal);          // Setting orientation
    ui->horizontalLayout_3->insertWidget(0, slider); // Inserts into a layout

    slider->setSingleStep(0); // Slider movement setting
    slider->setPageStep(0);

    connect(Play->getMplayer(), &QMediaPlayer::positionChanged, slider, &QSlider::setValue); // Passes the playback position to the slider
    connect(slider, &QSlider::valueChanged, Play, &Mediacontroll::Position);                 // Changes playback position according to slider position
    connect(Play, SIGNAL(positionChanged(QString)), ui->position, SLOT(setText(QString)));   // Passes elapsed time to the UI
    connect(Play, &Mediacontroll::positionChanged, slider, &QSlider::setToolTip);            // Updates tooltip
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(moved()));                          // Dynamic playback change by slider

    connect(Play->getMplayer(), &QMediaPlayer::durationChanged, slider, &QSlider::setMaximum); // Sets slider max range
    connect(slider, &QSlider::rangeChanged, Play, &Mediacontroll::Duration);                   // Sets slider range
    connect(Play, SIGNAL(durationChanged(QString)), ui->duration, SLOT(setText(QString)));     // Passes max duration to the UI

    connect(Play, SIGNAL(volumeChanged(int)), ui->dial, SLOT(setValue(int))); // Passes volume change to the UI

    connect(Play, SIGNAL(MediaChanged()), this, SLOT(Media_Update())); // Media change is passed to Media_Update

    take->setText("0/0");

    ui->statusBar->insertWidget(0, placeholder, 1); // Sets placeholder in the status bar

    ui->statusBar->insertPermanentWidget(1, take); // Sets take in the status bar
}
/**
 * @brief MainWindow::~MainWindow
 * Writes the value zero to the Arduino
 * delete ui and serial
 */
MainWindow::~MainWindow()
{
    delete ui;
    serall->Arduino_Output("0");
    delete serall;
}
/**
 * @brief MainWindow::moved
 * Passes the slider value to Play
 */
void MainWindow::moved()
{
    Play->getMplayer()->setPosition(slider->value());
}
/**
 * @brief MainWindow::resize
 * Sets the UI size according to data type
 */
void MainWindow::resize()
{
    if ((title.right(3) == "mp4") || (title.right(3) == "wmv"))
    { // Fullscreen for video formats
        mvideo->show();
        showFullScreen();
        mvideo->setMinimumSize(width() - 22, height() - 122); // Normal size for audio formats
    }
    if (title.right(3) == "mp3")
    {
        mvideo->hide();
        this->showNormal();
        this->setGeometry(304, 298, 416, 131);
    }
}
/**
 * @brief MainWindow::Button_Switch
 * @param val
 * Calls the corresponding slots
 */
void MainWindow::Button_Switch(SerallPort::Button val)
{
    switch (val)
    {
    case SerallPort::Previous_volume_down_pressed:
        on_Previous_volume_down_pressed();
        break;
    case SerallPort::stop_clicked:
        on_stop_clicked();
        break;
    case SerallPort::Next_volume_up_pressed:
        on_Next_volume_up_pressed();
        break;
    case SerallPort::Previous_volume_down_clicked:
        on_Previous_volume_down_clicked();
        break;
    case SerallPort::Play_Pause_clicked:
        on_Play_Pause_clicked();
        break;
    case SerallPort::Next_volume_up_clicked:
        on_Next_volume_up_clicked();
        break;
    case SerallPort::Previous_volume_down_released:
        on_Previous_volume_down_released();
        break;
    case SerallPort::open_clicked:
        Play->setPause(false);
        on_open_clicked();
        break;
    case SerallPort::Next_volume_up_released:
        on_Next_volume_up_released();
        break;
    }
}
/**
 * @brief MainWindow::Media_Update
 * Updates the track number.
 * Updates the title
 */
void MainWindow::Media_Update()
{
    take->setText(QString::number(Play->getMplayer()->playlist()->currentIndex() + 1) + "/" + QString::number(Play->getMplayer()->playlist()->mediaCount()));
    title = Play->getMplayer()->playlist()->currentMedia().canonicalUrl().toString();
    title.remove(0, title.lastIndexOf("/") + 1);
    ui->statusBar->showMessage(title);

    if (title.length() == 0)
    { // Calls on_Play_Pause_clicked if title length is zero
        on_Play_Pause_clicked();
    }
}
/**
 * @brief MainWindow::on_open_clicked
 * Opens the open dialog and sets the QString list
 * in Play. Sets the volume.
 * Calls on_Play_Pause_clicked()
 */
void MainWindow::on_open_clicked()
{
    QStringList file = QFileDialog::getOpenFileNames(this, "Open",
                                                     "/",
                                                     "*.mp3 *.mp4");

    if (file.isEmpty())
    {
        return;
    }

    Play->open(QUrl::fromStringList(file));

    Play->getMplayer()->setVolume(ui->dial->value());
    ui->position->setText("00:00");

    on_Play_Pause_clicked();
}
/**
 * @brief MainWindow::on_Play_Pause_clicked
 * Play calls Play_Pause() and UI adjusts accordingly
 */
void MainWindow::on_Play_Pause_clicked()
{
    if (Play->getMplayer()->playlist()->mediaCount() == 0)
    { // Calls on_open_clicked if no media selected
        on_open_clicked();
    }
    else
    {
        Play->Play_Pause();

        if (Play->getPause() == false)
        {
            ui->Play_Pause->setIcon(QIcon(QPixmap(":/Icon/Pause.png")));
            ui->Play_Pause->setIconSize(QSize(25, 30));
            ui->Play_Pause->setToolTip("Pause");
        }
        else
        {
            ui->Play_Pause->setIcon(QIcon(QPixmap(":/Icon/Play.ico")));
            ui->Play_Pause->setIconSize(QSize(25, 30));
            ui->Play_Pause->setToolTip("Play");
        }
    }

    resize();
}
/**
 * @brief MainWindow::on_stop_clicked
 * Stops the current playback and adjusts the UI
 * through on_Play_Pause_clicked()
 * Sets UI to normal view
 */
void MainWindow::on_stop_clicked()
{
    Play->setPause(false);
    on_Play_Pause_clicked();
    Play->Stop();
    mvideo->hide();

    ui->position->setText("0:00");
    this->showNormal();
    this->setGeometry(304, 298, 416, 131);
}
/**
 * @brief MainWindow::on_dial_valueChanged
 * @param value
 * Sends volume value to Arduino
 * Sets volume value in Play
 * and updates the tooltip
 */
void MainWindow::on_dial_valueChanged(int value)
{
    serall->Arduino_Output(QString::number(ui->dial->value()));
    Play->getMplayer()->setVolume(value);
    ui->dial->setToolTip("Volume " + QString::number(value));
}

/**
 * @brief MainWindow::on_Next_volume_up_pressed
 * Play calls Next_volume_up_pressed()
 */
void MainWindow::on_Next_volume_up_pressed()
{
    Play->Next_volume_up_pressed();
}

/**
 * @brief MainWindow::on_Next_volume_up_released
 * Play calls Next_volume_up_released()
 */
void MainWindow::on_Next_volume_up_released()
{
    Play->Next_volume_up_released();
}

/**
 * @brief MainWindow::on_Previous_volume_down_pressed
 * Play calls Previous_volume_down_pressed()
 */
void MainWindow::on_Previous_volume_down_pressed()
{
    Play->Previous_volume_down_pressed();
}

/**
 * @brief MainWindow::on_Previous_volume_down_released
 * Play calls Previous_volume_down_released()
 */
void MainWindow::on_Previous_volume_down_released()
{
    Play->Previous_volume_down_released();
}

/**
 * @brief MainWindow::on_Next_volume_up_clicked
 * Play calls Next_volume_up_clicked()
 * UI is updated according to size and playback status
 */
void MainWindow::on_Next_volume_up_clicked()
{
    Play->Next_volume_up_clicked();

    if (Play->getPause())
    {
        ui->Play_Pause->setIcon(QIcon(QPixmap(":/Icon/Play.ico")));
        ui->Play_Pause->setIconSize(QSize(25, 30));
        ui->Play_Pause->setToolTip("Play");
    }
    else
    {
        ui->Play_Pause->setIcon(QIcon(QPixmap(":/Icon/Pause.png")));
        ui->Play_Pause->setIconSize(QSize(25, 30));
        ui->Play_Pause->setToolTip("Pause");
    }

    resize();
}

/**
 * @brief MainWindow::on_Previous_volume_down_clicked
 * Play calls Previous_volume_down_clicked()
 * UI is updated according to size and playback status
 */
void MainWindow::on_Previous_volume_down_clicked()
{
    Play->Previous_volume_down_clicked();

    if (Play->getPause())
    {
        ui->Play_Pause->setIcon(QIcon(QPixmap(":/Icon/Play.ico")));
        ui->Play_Pause->setIconSize(QSize(25, 30));
        ui->Play_Pause->setToolTip("Play");
    }
    else
    {
        ui->Play_Pause->setIcon(QIcon(QPixmap(":/Icon/Pause.png")));
        ui->Play_Pause->setIconSize(QSize(25, 30));
        ui->Play_Pause->setToolTip("Pause");
    }

    resize();
}

/**
 * @brief MainWindow::keyPressEvent
 * @param e
 * Keyboard controls
 */
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    { // Minimizes the UI and stops playback
        Play->setPause(false);
        on_Play_Pause_clicked();
        mvideo->hide();
        showNormal();
        setGeometry(304, 298, 416, 131);
    }

    if (e->key() == Qt::Key_O)
    { // Pauses playback and calls on_open_clicked()
        Play->setPause(false);
        on_open_clicked();
    }

    if (e->key() == Qt::Key_Space)
    { // Calls on_Play_Pause_clicked()
        on_Play_Pause_clicked();
    }

    if (e->key() == Qt::Key_Right)
    { // Calls on_Next_volume_up_clicked()
        on_Next_volume_up_clicked();
    }

    if (e->key() == Qt::Key_Left)
    { // Calls on_Previous_volume_down_clicked()
        on_Previous_volume_down_clicked();
    }

    if (e->key() == Qt::Key_Plus)
    { // Calls on_Next_volume_up_pressed()
        on_Next_volume_up_pressed();
    }

    if (e->key() == Qt::Key_Minus)
    { // Calls on_Previous_volume_down_pressed()
        on_Previous_volume_down_pressed();
    }
}

/**
 * @brief MainWindow::keyReleaseEvent
 * Calls on_Previous_volume_down_released()
 * Then calls on_Next_volume_up_released()
 */
void MainWindow::keyReleaseEvent(QKeyEvent *)
{
    on_Previous_volume_down_released();
    on_Next_volume_up_released();
}
