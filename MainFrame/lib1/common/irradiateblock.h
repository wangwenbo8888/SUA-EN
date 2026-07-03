#ifndef IRRADIATEBLOCK_H_
#define IRRADIATEBLOCK_H_

#include "qstring.h"

struct Irradiate_Block
{
    QString id;
    double leftRight;    // 治疗头左右偏转角度
    double frontBack;    // 治疗头前后偏转角度

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

