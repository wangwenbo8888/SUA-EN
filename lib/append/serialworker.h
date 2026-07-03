#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QThread>

class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = 0);
    ~SerialWorker();

    inline bool exist();

signals:
    void serialReadReady(QByteArray data);
    void serialError();
    void serialInitComplet(bool exist,bool bswitch);

public slots:
    void readdata();
    void senddata(QByteArray data);
    void init(QString portNmae);
    void close();

private:
    QString portName;
    QString request;
    int waitTimeout;
    bool quit;
    bool m_bSendRequest;
    QSerialPort *serial;
    bool m_bExist;//是否成功打开串口
    bool m_bSwitchStatusInit;//初始开关状态
    QByteArray m_SerialReadBuffer;

    void scanSerial();
    bool openSerial();
    void updateSettings();

    bool CheckDevice();
    void updateInitStauts();
    bool DataValidation(QByteArray data_in);
    QByteArray DataAddCRC(QByteArray data_in);
    unsigned char caluCRC(unsigned char *p,unsigned char len);
    void handleNewData();
    void SetEnable();
};

inline bool SerialWorker::exist() { return serial == NULL ? false:true; }

#endif // SERIALWORKER_H
