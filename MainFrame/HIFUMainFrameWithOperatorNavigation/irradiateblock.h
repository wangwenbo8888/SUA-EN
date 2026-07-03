#ifndef IRRADIATEBLOCK_H_
#define IRRADIATEBLOCK_H_

#include "qstring.h"

struct Irradiate_Block
{
    QString id;
    double leftRight;    // Probe左右偏转Angle
    double frontBack;    // Probe前后偏转Angle

    QString para2String(int i)
    {
        switch(i)
        {
            case 0:
                return id;
            break;

            case 1:
            return QString::number(leftRight);
            break;

            case 2:
                return QString::number(frontBack);
            break;

            default:
                return "";

        }
    }
};

#endif // IRRADIATEBLOCK_H_

