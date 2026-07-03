#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H
#include <QObject>
#include <QDir>
#include <QDateTime>
//#include "filesavedialog.h"

enum PAFILETYPE{
    PA_IMAGE,
    PA_VIDEO,
    PA_REPORT
};

class PatientManager
{
public:    
    ~PatientManager();
    static PatientManager* getInstance();
    bool createPatientDir(QString nameid);
    void calRootDir();

    QString getImageName();
    QString getVideoName();
    QString getReportName();
    QString getTreatmentName();

    QString getImagePath();
    QString getVideoPath();
    QString getReportPath();
    QString getTreatmentPath();

    void SetFullPath(QString path);

    QString getImageFullPath();
    QString getVideoFullPath();
    QString getReportFullPath();
    QString getTreatmentFullPath();
    QString autoGetReportFullPath();

    QString getTimeName();

    QString getPatientId();

    static QString rootDir;
    static QString patientDir;
    static QString paImageDir_default;
    static QString paVideoDir_default;
    static QString paReportDir_default;
    static QString paTreatmentDir_default;

    static QString paImageDir;
    static QString paVideoDir;
    static QString paReportDir;
    static QString paTreatmentDir;

    static QString paImageName;
    static QString paVideoName;
    static QString paReportName;
    static QString paTreatmentName;

    static QString m_sPatientId;

    QString getCurDatetimeNmae();
    bool isDirExist(QString fullPath);

private:
    //PatientManager();
//    FileSaveDialog *_filesavedialog;

    QString savefullpath;
    static PatientManager *pPM;

    QDir getRootDrive(const QDir &path);

public slots:
    void getfullpath(QString path);


};

#endif // PATIENTMANAGER_H
