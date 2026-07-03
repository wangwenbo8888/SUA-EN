#include <QApplication>
#include <QMetaObject>
#include <QMessageBox>
#include "login.h"
#include "log.h"
//屏蔽vtk版本报错信息
#include <vtkOutputWindow.h>

#include <DbgHelp.h>
#include <QDir>
#include <QDateTime>
#include <QDebug>

#include "crashreporter.h"
#include "tablemodel.h"
#include "newmainwindow.h"
#include "logindialog.h"
#include "usermanager.h"
#include "selftestdlg.h"
#include "dupumpcontrol.h"
#include "quitform.h"

#include <windows.h>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    const QString kApplicationVersion = QStringLiteral("0.1.180413");

    QSharedMemory shared("hifu mainframe");
    if(shared.attach())
    {
        return 0;
    }
    shared.create(1);

    // Disable vtk version warnings.
    vtkOutputWindow::SetGlobalWarningDisplay(0);

    qInstallMessageHandler(updateLog);

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("HIFUMainFrame"));
    QCoreApplication::setApplicationVersion(kApplicationVersion);

//    HANDLE hMutex =  CreateMutex(nullptr,TRUE,(LPCWSTR)qApp->applicationName().toStdString().c_str());
//    if(GetLastError() == ERROR_ALIAS_EXISTS){
//        QMessageBox::warning(nullptr,"error","application already running!" );
//        CloseHandle(hMutex);
//        hMutex = NULL;
//        return 1;
//
//    }

    CrashReporter::install(kApplicationVersion);

    SelfTestDlg SelfCheckForm(NULL,NULL);
    SelfCheckForm.showCheck();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    LoginDialog loginForm(NULL);
    QObject::connect(&SelfCheckForm,SIGNAL(showLogin()),&loginForm,SLOT(show()));

    NewMainWindow w(&SelfCheckForm,NULL);
    SelfCheckForm.SetMainWindow(&w);


    UserManager userMgForm(NULL);

//    dupumpcontrol dupumpform(NULL);
//    w.setdupump(& dupumpform);
//    dupumpform.SetMainWindow(&w);


    QObject::connect(&loginForm,SIGNAL(ShowUserManager(bool)),&userMgForm,SLOT(ShowUserManager(bool)));
    QObject::connect(&userMgForm,SIGNAL(ShowMainWindow()),&w,SLOT(show()));
    QObject::connect(&userMgForm,SIGNAL(ShowMainWindow()),&w,SLOT(startdeeptimer()));
    QObject::connect(&w,SIGNAL(showUserManager(bool)),&userMgForm,SLOT(ShowUserManager(bool)));
    //QObject::connect(&w,SIGNAL(selfCheckStatusUpdate(uint,uint,uint)),&SelfCheckForm,SLOT(statusUpdated(uint,uint,uint)));
//    QObject::connect(&w,SIGNAL(selfCheckStatusUpdate(uint,uint,uint)),&dupumpform,SLOT(statusUpdated(uint,uint,uint)));
//    QObject::connect(&w,SIGNAL(MeterUpdate(short)),&dupumpform,SLOT(updateMeterDat(short)));
//    QObject::connect(&w,SIGNAL(pump1ctl()),&dupumpform,SLOT(controlpump1()));
//    QObject::connect(&w,SIGNAL(speed1up()),&dupumpform,SLOT(speed1up()));
//    QObject::connect(&w,SIGNAL(speed1down()),&dupumpform,SLOT(speed1down()));
//    QObject::connect(&w,SIGNAL(pump2ctl()),&dupumpform,SLOT(controlpump2()));
//    QObject::connect(&w,SIGNAL(speed2up()),&dupumpform,SLOT(speed2up()));
//    QObject::connect(&w,SIGNAL(speed2down()),&dupumpform,SLOT(speed2down()));
    QObject::connect(&w,SIGNAL(MainClose()),&userMgForm,SLOT(close()));
    QObject::connect(&w,SIGNAL(showSelfCheckDlg()),&SelfCheckForm,SLOT(showFromMainWindow()));
//    QObject::connect(&w,SIGNAL(showdupumpcontrol()),&dupumpform,SLOT(showFromMainWindow()));
    QObject::connect(&w,SIGNAL(SendSelfTest(bool,bool,bool,bool,bool)),&SelfCheckForm,SLOT(updateStatus(bool,bool,bool,bool,bool)));
    QObject::connect(&userMgForm,SIGNAL(SendPatientInfo(QString ,QString , QString ,QString ,QString ,QString ,QString,int)),
                     &w,SLOT(sendPatientInfo(QString ,QString , QString ,QString ,QString ,QString ,QString,int)));
//    w.getSnapshot();
//    int num = 0;
//    for(int i = 0; i < w.process_vec.size();++i)//计算进程数量
//    {
//        if(w.process_vec[i] == "HIFUMainFrame.exe")
//        num += 1;
//    }

//    if (num >1 )//大于0，用于检测其他进程是否已经开启
//        //大于1，用于检测自身进程是否已经打开
//    {
//       qDebug()<<"程序运行中,如有需要请关闭后台线程";
//       QMessageBox::information(NULL, "Warning", "程序运行中,如有需要请关闭后台线程",
//                                QMessageBox::Yes | QMessageBox::Yes);
//       int pid = getpid();
//       QProcess p;
//       QString cmd = QString("taskkill /F /PID %1 /T").arg(pid);
//       p.execute(cmd);
//       p.close();
//    }
//    else{
//       qDebug()<<"没有该进程";
//    }
    return a.exec();


//    CloseHandle(hMutex);
//    hMutex = NULL;
//    return 0;
}

