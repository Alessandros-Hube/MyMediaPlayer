#include "serallport.h"
/**
 * @brief SerallPort::SerallPort
 * @param parent
 * The constructor initializes `arduino` with `new`,
 * `Button1`, `Button2`, `Button3`, and `arduino_is_available`
 * with `false`, `press` with `true`, and `counter` with `0`.
 * It also checks if an Arduino is connected to the computer.
 */
SerallPort::SerallPort(QObject *parent) : QObject(parent)
{

    Button1 = false;
    Button2 = false;
    Button3 = false;
    press = true;
    counter = 0;

    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;

    // Display number of available serial ports
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if (serialPortInfo.hasVendorIdentifier())
        {
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if (serialPortInfo.hasProductIdentifier())
        {
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }

    // Check for Arduino vendor and product IDs to confirm connection
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)
            {
                if (serialPortInfo.productIdentifier() == arduino_uno_product_id)
                {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }
}
/**
 * @brief SerallPort::~SerallPort
 * Closes the Arduino connection and deletes `arduino`.
 */
SerallPort::~SerallPort()
{
    arduino->close();
    delete arduino;
}
/**
 * @brief SerallPort::open
 * Opens the serial connection to the Arduino and connects
 * Arduino signals to appropriate slots.
 */
void SerallPort::open()
{
    if (arduino_is_available)
    {
        // Open and configure the serial port
        arduino->setPortName(arduino_port_name);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        if (arduino->open(QSerialPort::ReadWrite))
        {
            qDebug() << "true";
            emit deviceOpen(0); // Emit that the device is open
        }
        else
        {
            qDebug() << "false";
            emit deviceOpen(1); // Emit failure to open device
        }
    }
    connect(arduino, SIGNAL(readyRead()), this, SLOT(read()));
    connect(arduino, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(close()));
}
/**
 * @brief SerallPort::close
 * Emits `deviceClose(true)` when the device is closed.
 */
void SerallPort::close()
{
    emit deviceClose(true);
}
/**
 * @brief SerallPort::Arduino_Output
 * @param s
 * Sends a message to the Arduino.
 */
void SerallPort::Arduino_Output(QString s)
{
    if (arduino->isOpen())
    { // Checks if the connection is open
        arduino->write(s.toLatin1());
        arduino->flush();
        arduino->waitForBytesWritten(30000);
    }
}
/**
 * @brief SerallPort::Arduino_Input
 * @param value
 * This function is used when the Arduino is connected with a 7-segment display.
 * The Arduino sends resistance values corresponding to a button.
 * Refer to SerialPort communication protocol.
 */
void SerallPort::Arduino_Input(int value)
{
    int val = value;
    press = true;
    if ((val > 220) && (val < 224))
    {
        Button1 = 1;
        Button2 = 0;
        Button3 = 0;
    }

    if ((val < 220) && (val > 210))
    {
        Button1 = 0;
        Button2 = 1;
        Button3 = 0;
    }

    if ((val > 190) && (val < 195))
    {
        Button1 = 0;
        Button2 = 0;
        Button3 = 1;
    }

    if ((val > 225) && (val < 227))
    {
        press = 0;
    }
    Button_Control();
}

/**
 * @brief SerallPort::Button_Control
 * Differentiates between button press and hold, and emits
 * the appropriate enum via `Button_Input(...)`.
 */
void SerallPort::Button_Control()
{
    if ((press == 1))
    {
        counter++;
        if (counter == 150)
        {
            if (Button1 == 1)
            {
                emit Button_Input(Previous_volume_down_pressed);
                Button1 = 0;
                Button2 = 0;
                Button3 = 0;
            }

            if (Button2 == 1)
            {
                emit Button_Input(stop_clicked);
                emit Button_Input(open_clicked);
                Button1 = 0;
                Button2 = 0;
                Button3 = 0;
            }

            if (Button3 == 1)
            {
                emit Button_Input(Next_volume_up_pressed);
                Button1 = 0;
                Button2 = 0;
                Button3 = 0;
            }
            qDebug() << "is press";
            counter = 0;
        }
    }
    if (press == 0)
    {
        if (Button1 == 1)
        {
            emit Button_Input(Previous_volume_down_clicked);
            Button1 = 0;
            Button2 = 0;
            Button3 = 0;
            qDebug() << "is click";
        }

        if (Button2 == 1)
        {
            emit Button_Input(Play_Pause_clicked);
            Button1 = 0;
            Button2 = 0;
            Button3 = 0;
            qDebug() << "is click";
        }

        if (Button3 == 1)
        {
            emit Button_Input(Next_volume_up_clicked);
            Button1 = 0;
            Button2 = 0;
            Button3 = 0;
            qDebug() << "is click";
        }
        emit Button_Input(Previous_volume_down_released);
        emit Button_Input(Next_volume_up_released);
    }
}
/**
 * @brief SerallPort::read
 * Reads incoming message.
 */
void SerallPort::read()
{
    Buffer = Buffer + QString(arduino->readAll());
    if (Buffer.indexOf(";") != -1)
    {                                                    // Waits until the terminating character ";" is received
        QString tb = Buffer.mid(0, Buffer.indexOf(";")); // Temporarily stores received message
        qDebug() << tb;
        Buffer.remove(0, Buffer.indexOf(";") + 1); // Removes message from buffer
        if (tb.length() == 8)
        {
            Buffer.clear();
        }
        QStringList split = tb.split("|"); // Splits message by "|"
        switch (split.at(0).toInt(0, 16))
        {
        case 0x01: // Arduino with 7-segment display
            Arduino_Input(split.at(1).toInt());
            break;
        default:
            break;
        }
    }
}
