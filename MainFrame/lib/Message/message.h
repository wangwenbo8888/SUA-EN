#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QMap>

#include "variable.h"
#include "constant.h"
#include "message_global.h"

class MESSAGESHARED_EXPORT Message : public QObject
{
    Q_OBJECT

public:
    Message(QObject* parent=0);

public slots:
    void work(PackageFlag, DataFlag);

signals:
    void statusUpdated(DataFlag);

private:
    // TODO: handle the message queue
//    QQueue<PackageFlag, DataFlag> m_msgQueue;
    QMap<PackageFlag, QString> m_mapPackage;
    QMap<DataFlag, QString> m_mapData;

    void initFlagString();
    void handleInquiry(DataFlag);
    void handleSonication(DataFlag);
    void handleFeedback(DataFlag);
    void writeLog(QString);

    inline const QString mapFromPackage(PackageFlag pFlag) { return m_mapPackage[pFlag]; }
    inline const QString mapFromData(DataFlag dFlag) { return m_mapData[dFlag]; }
};

#endif // MESSAGE_H
