#ifndef LOG
#define LOG

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

void updateLogFile(QString name, QByteArray& log) {
    QFile file(name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << log << endl;
        file.close();
    }
}

void updateLog(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);
    QString path = "D:/LOGS";
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(path);
    }
    QDateTime time = QDateTime::currentDateTime();
    QString logFileName = "D:/LOGS/" + time.toString("yyyy-MM-dd")
                                + ".log";
    QString warningLogFileName = "D:/LOGS/" + time.toString("yyyy-MM-dd")
                                       + " warning.log";
    QString errorLogFileName = "D:/LOGS/" + time.toString("yyyy-MM-dd")
                                       + " error.log";
    QByteArray log = time.toString("hh:mm:ss.zzz").toLocal8Bit();
    log += QString(" (%1) >> %2\r\n").arg(context.category).arg(msg);

    switch(type) {
    case QtDebugMsg:
        updateLogFile(logFileName, log);
        break;
    case QtWarningMsg: case QtFatalMsg:
        updateLogFile(warningLogFileName, log);
        break;
    case QtCriticalMsg:
        updateLogFile(errorLogFileName, log);
        break;
    }
}

#endif // LOG

