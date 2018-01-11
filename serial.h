#ifndef SERIAL_H
#define SERIAL_H

#include <QtSerialPort/QSerialPort>
#include <QSocketNotifier>
#include <QObject>

class Serial :public QSerialPort
{
    Q_OBJECT
public:
    Serial();
    ~Serial();
    bool openSerial(QString portName, quint32 baud, char dataBit, char stopBit, char parity);
    quint64 writeSerial(char *data,quint64 len);
    quint64 readSerial(char *data,quint64 len);
    quint64 writeSerial(const QByteArray &data);
    void closeSerial();
public slots:
    virtual void receive();
    void handleSerialError(QSerialPort::SerialPortError error);  //错误处理

private:

};

#endif // SERIAL_H
