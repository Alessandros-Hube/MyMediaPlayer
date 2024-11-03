#ifndef SERALLPORT_H
#define SERALLPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>

class SerallPort : public QObject
{
    Q_OBJECT
public:
    explicit SerallPort(QObject *parent = 0); // Constructor
    ~SerallPort();                            // Destructor
    /* Enum for transmission of pressed button */
    enum Button
    {
        Previous_volume_down_pressed,
        stop_clicked,
        Next_volume_up_pressed,
        Previous_volume_down_clicked,
        Play_Pause_clicked,
        Next_volume_up_clicked,
        Previous_volume_down_released,
        open_clicked,
        Next_volume_up_released
    };

    void open(); // Opens serial connection

signals:

    void deviceOpen(bool);     // Emitted when Arduino is connected to PC
    void deviceClose(bool);    // Emitted when Arduino is disconnected from PC
    void Button_Input(Button); // Emitted when a button is pressed on the Arduino

public slots:

    void close(); // Slot for closing the serial connection

    void Arduino_Output(QString s); // Slot for Arduino output
    void Arduino_Input(int value);  // Slot for Arduino 7-segment display
    void Button_Control();          // Slot for button control

    void read(); // Reads incoming messages from Arduino

private:
    QSerialPort *arduino = NULL; // Object of class QSerialPort
    static const quint16 arduino_uno_vendor_id = 10755;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name; // Temporary storage for port name
    bool arduino_is_available; // Availability status of Arduino

    bool Button1; // Toggle for Button1
    bool Button2; // Toggle for Button2
    bool Button3; // Toggle for Button3
    bool press;   // Status of whether pressed
    int counter;  // Helper variable

    QString Buffer; // Temporary storage for incoming messages
};

#endif // SERALLPORT_H
