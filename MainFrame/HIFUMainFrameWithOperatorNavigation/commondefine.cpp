#include "commondefine.h"

#include "qimage.h"
#include "qdebug.h"

#include "QSettings"
#include "constant.h"

const double delta = 0.1;


double voltage_1;
double voltage_2;
double voltage_3;
double voltage_4;
double voltage_5;
double voltage_6;
double voltage_7;
double voltage_8;
double voltage_9;
double voltage_10;
double voltage_11;
double voltage_12;
double voltage_13;
double voltage_14;


// 彩色图像转换成灰度图像
QImage toGray(QImage image)
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    }

    return ret;
}

void getsettingvoltage()
{
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    voltage_1 = settings->value("Voltage/Power22").toString().toDouble();
    voltage_2 = settings->value("Voltage/Power54").toString().toDouble();
    voltage_3 = settings->value("Voltage/Power72").toString().toDouble();
    voltage_4 = settings->value("Voltage/Power90").toString().toDouble();
    voltage_5 = settings->value("Voltage/Power99").toString().toDouble();
    voltage_6 = settings->value("Voltage/Power108").toString().toDouble();
    voltage_7 = settings->value("Voltage/Power117").toString().toDouble();
    voltage_8 = settings->value("Voltage/Power126").toString().toDouble();
    voltage_9 = settings->value("Voltage/Power135").toString().toDouble();
    voltage_10 = settings->value("Voltage/Power144").toString().toDouble();
    voltage_11 = settings->value("Voltage/Power153").toString().toDouble();
    voltage_12 = settings->value("Voltage/Power162").toString().toDouble();
    voltage_13 = settings->value("Voltage/Power171").toString().toDouble();
    voltage_14 = settings->value("Voltage/Power180").toString().toDouble();
//    voltage_1 = settings->value("Voltage/Voltage_1=2").toString().toDouble();
}

void outputTreatmentLog(const QString& str0,const QString& str1,const QString& str2,
                        const QString& str3,const QString& str4,const QString& str5,
                        const QString& str6,const QString& str7,const QString& str8,
                        const QString& str9)
{
    // 跟治疗相关的log输出
   qDebug()<<"%%%%%% "<<str0 << " "<< str1 <<" "<<str2 <<" "
           <<str3 << " " << str4 << " "<<str5<<" "<<str6<<" "<<str7<<" "<<str8<<" "<<str9;
}

QString getDisplayPower(double power)
{
//    qDebug()<<"滑动条显示数值："<<power;
    if (fabs(power-1.0)<delta || fabs(power-voltage_1*voltage_1)<delta)
    {
        return QString("22");
    }
    else if (fabs(power-4.0)<delta || fabs(power - voltage_2*voltage_2)<delta)
    {
        return QString("54");
    }
    else if (fabs(power-9.0)<delta|| fabs(power-voltage_3*voltage_3)<delta)
    {
        return QString("72");
    }
    else if (fabs(power-16.0)<delta || fabs(power-voltage_4*voltage_4)<delta)
    {
        return QString("90");
    }
    else if (fabs(power-25.0)<delta || fabs(power-voltage_5*voltage_5)<delta)
    {
        return QString("99");
    }
    else if (fabs(power-36.0)<delta || fabs(power-voltage_6*voltage_6)<delta)
    {
        return QString("108");
    }
    else if (fabs(power-49.0)<delta || fabs(power-voltage_7*voltage_7)<delta)
    {
        return QString("117");
    }
    else if (fabs(power-64.0)<delta || fabs(power-voltage_8*voltage_8)<delta)
    {
        return QString("126");
    }
    else if (fabs(power-81.0)<delta || fabs(power-voltage_9*voltage_9)<delta)
    {
        return QString("135");
    }
    else if (fabs(power-100.0)<delta || fabs(power-voltage_10*voltage_10)<delta)
    {
        return QString("144");
    }
    else if (fabs(power-121.0)<delta || fabs(power-voltage_11*voltage_11)<delta)
    {
        return QString("153");
    }
    else if (fabs(power-144.0)<delta || fabs(power-voltage_12*voltage_12)<delta)
    {
        return QString("162");
    }
    else if (fabs(power-169.0)<delta || fabs(power-voltage_13*voltage_13)<delta)
    {
        return QString("171");
    }
    else if (fabs(power-196.0)<delta || fabs(power-voltage_14*voltage_14)<delta)
    {

        return QString("180");
    }
    else
    {
        return QString("Unknown");
    }

}

// EMC测试设置电压
double getVoltage(int power)
{
    if (power==1)
    {
        return voltage_1;
    }
    else if (power==4)
    {
        return voltage_2;
    }
    else if (power==9)
    {
        return voltage_3;
    }
    else if (power==16)
    {
        return voltage_4;
    }
    else if (power==25)
    {
        return voltage_5;
    }
    else if (power==36)
    {
        return voltage_6;
    }
    else if (power==49)
    {
        return voltage_7;
    }
    else if (power==64)
    {
        return voltage_8;
    }
    else if (power==81)
    {
        return voltage_9;
    }
    else if (power==100)
    {
        return voltage_10;
    }
    else if (power==121)
    {
        return voltage_11;
    }
    else if (power==144)
    {
        return voltage_12;
    }
    else if (power==169)
    {
        return voltage_13;
    }
    else if (power==196)
    {
        return voltage_14;
    }

}

bool getVoltageFromDisplay(std::string str,double& value)
{
    if (str.find("22")!=-1)
    {
        value = 2.6;
        return true;
    }
    else if (str.find("54")!=-1)
    {
        value = 4.2;
        return true;
    }
    else if (str.find("72")!=-1)
    {
        value = 4.9;
        return true;
    }
    else if (str.find("90")!=-1)
    {
        value = 5.5;
        return true;
    }
    else if (str.find("99")!=-1)
    {
        value = 5.8;
        return true;
    }
    else if (str.find("108")!=-1)
    {
        value = 6.1;
        return true;
    }
    else if (str.find("117")!=-1)
    {
        value = 6.3;
        return true;
    }
    else if (str.find("126")!=-1)
    {
        value = 6.6;
        return true;
    }
    else if (str.find("135")!=-1)
    {
        value = 6.8;
        return true;
    }
    else if (str.find("144")!=-1)
    {
        value = 7.01;
        return true;
    }
    else if (str.find("153")!=-1)
    {
        value = 7.3;
        return true;
    }
    else if (str.find("162")!=-1)
    {
        value = 7.5;
        return true;
    }
    else if (str.find("171")!=-1)
    {
        value = 7.7;
        return true;
    }
    else if (str.find("180")!=-1)
    {
        value = 8.01;
        return true;
    }


    return false;
}
