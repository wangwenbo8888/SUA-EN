#ifndef LOG
#define LOG

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

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
    QDateTime time = QDateTime::currentDateTime();
    QString logFileName = "../" + time.toString("yyyy-MM-dd")
                                + ".log";
    QString warningLogFileName = "../" + time.toString("yyyy-MM-dd")
                                       + " warning.log";
    QByteArray log = time.toString("hh:mm:ss.zzz").toLocal8Bit();
    log += QString(" (%1) >> %2\r\n").arg(context.category).arg(msg);

    switch(type) {
    case QtDebugMsg:
        updateLogFile(logFileName, log);
        break;
    case QtWarningMsg: case QtCriticalMsg: case QtFatalMsg:
        updateLogFile(warningLogFileName, log);
        break;
    }
}

#endif // LOG

