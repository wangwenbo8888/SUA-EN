#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QDateTime>

#ifdef ASM_CRASH_REPORT
#include "CrashReport.h"
#endif
#include "exchndl.h"
#include "io.h"

char CrashReportPath[256];

LONG CreateCrashHandler(EXCEPTION_POINTERS *pException){
    QDateTime CurDTime = QDateTime::currentDateTime();
    QString current_date = CurDTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"Application Crash Record @"<<current_date;

    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
    QString errFlag(QString::number(record->ExceptionFlags, 16));
    QString errPara(QString::number(record->NumberParameters, 16));

    QString path=QApplication::applicationDirPath()+"/"+QApplication::applicationName();
    QString filepath=path+".RPT";

    QString  message;
    message = QStringLiteral( "异常日志保存在:\n%1\n请联系技术支持人员寻求帮助." ).arg(filepath );
    QString detailText=QString(" ErrorCode: %1\n ErrorAddr: %2\n ErrorFlag: %3\n ErrorPara: %4\n DateTime: %5").arg(errCode).arg(errAddr).arg(errFlag).arg(errPara).arg(current_date);

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(QApplication::applicationName()+" Crash Report");
    msgBox.setText(QString("非常抱歉!\n %1 程序运行异常，现在需要退出.").arg(QApplication::applicationName()));
    msgBox.setInformativeText(message);
    msgBox.setDetailedText(detailText);
    msgBox.exec();

    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CreateCrashHandler);

    ExcHndlInit();

    MainWindow w;
    w.show();

    return a.exec();
}
