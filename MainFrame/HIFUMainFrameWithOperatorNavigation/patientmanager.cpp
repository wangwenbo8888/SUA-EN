#include "patientmanager.h"

#include "qdebug.h"

PatientManager* PatientManager::pPM=NULL;
QString PatientManager::rootDir=QString();
QString PatientManager::patientDir=QString();
QString PatientManager::paImageDir_default=QString();
QString PatientManager::paVideoDir_default=QString();
QString PatientManager::paReportDir_default=QString();
QString PatientManager::paTreatmentDir_default=QString();

QString PatientManager::paImageDir=QString();
QString PatientManager::paVideoDir=QString();
QString PatientManager::paReportDir=QString();
QString PatientManager::paTreatmentDir=QString();

QString PatientManager::paImageName=QString();
QString PatientManager::paVideoName=QString();
QString PatientManager::paReportName=QString();
QString PatientManager::paTreatmentName=QString();

QString PatientManager::m_sPatientId=QString();

QString PatientManager::getCurDatetimeNmae()
{
    return QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
}

QDir PatientManager::getRootDrive(const QDir &path)
{
    QDir dir(path.canonicalPath());
    while(dir.cdUp());
    return dir;
}

bool PatientManager::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    else
    {
       bool ok = dir.mkpath(fullPath);
       return ok;
    }
}

PatientManager::~PatientManager()
{
}

PatientManager *PatientManager::getInstance()
{
    if(pPM==NULL)
    {
        pPM=new PatientManager();
        pPM->calRootDir();
    }
    else
        return PatientManager::pPM;
}

bool PatientManager::createPatientDir(QString nameid)
{
   
    PatientManager::m_sPatientId=nameid;
    QString paPath="D:/HIFU-V1.0/HifuPatient/"+nameid+"/";
    if(isDirExist(paPath))
    {
        patientDir=paPath;
        if(isDirExist(patientDir+"Image/")){
            PatientManager::paImageDir_default=patientDir+"Image/";
            PatientManager::paImageDir=paImageDir_default;
        }
        if(isDirExist(patientDir+"Video/")){
            PatientManager::paVideoDir_default=patientDir+"Video/";
            PatientManager::paVideoDir=paVideoDir_default;
        }
        if(isDirExist(patientDir+"Report/")){
            PatientManager::paReportDir_default=patientDir+"Report/";
            PatientManager::paReportDir=paReportDir_default;
        }

        // 保存治疗结构化数据
        if(isDirExist(patientDir+"Treatment/")){
            PatientManager::paTreatmentDir_default=patientDir+"Treatment/";
            PatientManager::paTreatmentDir=paTreatmentDir_default;
        }
    }
    else
        return false;
}

void PatientManager::calRootDir()
{
    QDir curDir(QDir::currentPath());
    rootDir=getRootDrive(curDir).absolutePath();
}

QString PatientManager::getImageName()
{
    if(paImageName.isEmpty())
    {
        QString sn= m_sPatientId+"-"+getCurDatetimeNmae();
        sn.append(".bmp");
        return sn;
    }
    else
        return QString("%1%2").arg(paImageName).arg(".bmp");
}

QString PatientManager::getVideoName()
{
    if(paVideoName.isEmpty())
    {
        QString sn=m_sPatientId+"_"+getCurDatetimeNmae();
        sn.append(".avi");
        return sn;
    }
    else
        return QString("%1%2").arg(paVideoName).arg(".avi");
}

QString PatientManager::getReportName()
{
    if(paReportName.isEmpty())
    {
        QString sn=m_sPatientId+"_"+getCurDatetimeNmae();
        sn.append(".pdf");
        return sn;
    }
    else
        return QString("%1%2").arg(paReportName).arg(".pdf");
}

QString PatientManager::getImagePath()
{
    return paImageDir;
}

QString PatientManager::getVideoPath()
{
    return paVideoDir;
}

QString PatientManager::getReportPath()
{
    return paReportDir;
}

QString PatientManager::getTreatmentPath()
{
    return paTreatmentDir;
}

QString PatientManager::getImageFullPath()
{
//    QString sFull=QString();
//    sFull.append(paImageDir);
//    sFull.append(getImageName());
//    return sFull;
    return savefullpath;
}

QString PatientManager::getVideoFullPath()
{
//    QString sFull=QString();
//    sFull.append(paVideoDir);
//    sFull.append(getVideoName());
    return savefullpath;
}

QString PatientManager::getReportFullPath()
{
//    QString sFull=QString();
//    sFull.append(paReportDir);
//    sFull.append(getReportName());
    return savefullpath;
}

QString PatientManager::autoGetReportFullPath()
{
    QString sFull=QString();
    sFull.append(paReportDir);
    sFull.append(getReportName());
    return sFull;
}

QString PatientManager::getTreatmentFullPath()
{
//    QString sFull=QString();
//    sFull.append(paTreatmentDir);
//    sFull.append(getReportName());
    return savefullpath;
}

QString PatientManager::getTimeName()
{
    return getCurDatetimeNmae();
}

QString PatientManager::getPatientId()
{
    return m_sPatientId;
}

void PatientManager::getfullpath(QString fullpath)
{
     savefullpath = fullpath;
}

void PatientManager::SetFullPath(QString path)
{
    savefullpath = path;
}
