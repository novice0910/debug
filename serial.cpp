#include "serial.h"

Serial::Serial()
{
    connect(this,SIGNAL(error(QSerialPort::SerialPortError)),this,
            SLOT(handleSerialError(QSerialPort::SerialPortError)));
    connect(this, SIGNAL(readyRead()), this, SLOT(receive()));
}

Serial::~Serial()
{
    closeSerial();
}

bool Serial::openSerial(QString portName,quint32 baud,char dataBit,char stopBit,char parity)
{
    setPortName(portName);
    bool ret = this->open(QIODevice::ReadWrite);
    setBaudRate(baud);

    switch (dataBit) {
    case '5':
        setDataBits(QSerialPort::Data5);
        break;
    case '6':
        setDataBits(QSerialPort::Data6);
        break;
    case '7':
        setDataBits(QSerialPort::Data7);
        break;
    case '8':
        setDataBits(QSerialPort::Data8);
        break;
    default:
        setDataBits(QSerialPort::Data8);
        break;
    }

    switch (stopBit) {
    case '1':
        setStopBits(QSerialPort::OneStop);
        break;
    case '2':
        setStopBits(QSerialPort::TwoStop);
        break;
    case '3':
        setStopBits(QSerialPort::OneAndHalfStop);
        break;
    default:
        setStopBits(QSerialPort::OneStop);
        break;
    }

    switch (parity) {
    case 'n':
    case 'N':
        setParity(QSerialPort::NoParity);
        break;
    case 'e':
    case 'E':
        setParity(QSerialPort::EvenParity);
        break;
    case 'o':
    case 'O':
        setParity(QSerialPort::OddParity);
        break;
    default:
        break;
    }

    return ret;
}
void Serial::closeSerial()
{
    this->close();
}

quint64 Serial::writeSerial(char *data, quint64 len)
{
    quint64 ret = this->writeData(data,len);
    return ret;
}

quint64 Serial::writeSerial(const QByteArray &data)
{
    quint64 ret = this->write(data);
    return ret;
}

quint64 Serial::readSerial(char *data, quint64 len)
{
    quint64 ret = this->readData(data,len);
    return ret;
}

void Serial::handleSerialError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ResourceError)
    {
    }
}

void Serial::receive()
{

}
