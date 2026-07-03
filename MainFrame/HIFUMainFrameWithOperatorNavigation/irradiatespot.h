#ifndef IRRADIATESPOT_H_
#define IRRADIATESPOT_H_

#include <QPointF>
#include "constant.h"
#include "qdebug.h"
#include "math.h"

#include "commondefine.h"
// #define PI 3.1415926

// Sonication Plan文件
enum Layer
{
    Layer_down = 0,   // Lowersonication point
    Layer_middle,     // Middlesonication point
    Layer_up,         // Uppersonication point

    Layer_end
};

enum Irradiate_model
{
    Model_inner_to_outer = 0,    // 从内到外sonication
    Model_outer_to_inner,

    Model_unknown
};

// sonication point信息
struct NewPoint3d
{
    double x;
    double y;
    double z;
};

int powerToVoltage(int power);
int energyToVoltage(int energy);

// 各sonication point参数
struct Irradiate_Spot
{
    QString id;          // sonication pointid
    QString idBorn;      // 创建的时候对应的点ID
    int focusCount;      // 焦点个数，单焦点还是多焦点
    int pulseCount;      // Pulse
    int coolingTime;     // Cooling
    double voltage;         // 电压
    int onTime;          // 施加脉冲Duration
    int offTime;         // Stop脉冲Duration
    int usAngle;         // 做Plan时对应的Ultrasound ProbeAngle
    double spotAngle;        // sonication point与中心连线与x轴夹角
    Layer layer;         // sonication point所处的层
    Circle circle;          // 圈
    NewPoint3d position;    // sonication point对应的空间位置
    bool isIrradiated;   // 是否sonication过

    Irradiate_Spot()
    {
        id = -1;               // sonication pointid
        idBorn = -1;
        focusCount = 0;        // 焦点个数，单焦点还是多焦点
        pulseCount = 0;        // Pulse
        coolingTime = 0;       // Cooling
        voltage = 0;           // 电压
        onTime = 0;            // 施加脉冲Duration
        offTime = 0;           // Stop脉冲Duration
        usAngle = 0;           // 做Plan时对应的Ultrasound ProbeAngle
        spotAngle = 0;
        circle = Circle_end;   // 圈
        layer = Layer_end;     // sonication point所处的层
        isIrradiated= false;
    }

    void operator = (const Irradiate_Spot& spot)
    {
        id     = spot.id;                       // sonication pointid
        idBorn = spot.idBorn;                   // 创建时点id
        focusCount  = spot.focusCount;          // 焦点个数，单焦点还是多焦点
        pulseCount  = spot.pulseCount;          // Pulse
        coolingTime = spot.coolingTime;         // Cooling
        voltage = spot.voltage;                 // 电压
        onTime  = spot.onTime;                  // 施加脉冲Duration
        offTime = spot.offTime;                 // Stop脉冲Duration
        usAngle = spot.usAngle;                 // 做Plan时对应的Ultrasound ProbeAngle
        spotAngle   = spot.spotAngle;
        layer   = spot.layer;                   // sonication point所处的层
        circle  = spot.circle;                  // 半径平方
        position.x   = spot.position.x;
        position.y   = spot.position.y;
        position.z   = spot.position.z;
        isIrradiated = spot.isIrradiated;
    }

    float angle(QPointF center, QPointF point)
    {
        float angle_temp;
        float xx, yy;

        xx = point.x() - center.x();
        yy = point.y() - center.y();

        if (xx == 0.0)
            angle_temp = PI / 2.0;
       else
            angle_temp = atan(fabs(yy / xx));

        if ((xx < 0.0) && (yy >= 0.0))
            angle_temp = PI - angle_temp;
        else if ((xx < 0.0) && (yy < 0.0))
            angle_temp = PI + angle_temp;
       else if ((xx >= 0.0) && (yy < 0.0))
            angle_temp = - angle_temp;

       return (angle_temp);
    }

    // 从下向上，从小Angle到大Angle，从Ultrasound Probe正方向到负方向排列
    bool operator < (const Irradiate_Spot& spot) const
    {
        if (layer < spot.layer)
        {
            return true;
        }
        else if (layer == spot.layer
                 && circle < spot.circle)
        {
            return true;
        }
        else if (layer == spot.layer
                 && circle == spot.circle)
        {
            return spotAngle < spot.spotAngle;
        }
    }

    bool operator > (const Irradiate_Spot& spot) const
    {
        if (layer < spot.layer)
        {
            return true;
        }
        else if (layer == spot.layer
                 && circle > spot.circle)
        {
            return true;
        }
        else if (layer == spot.layer
                 && circle == spot.circle)
        {
            return spotAngle < spot.spotAngle;
        }
    }

    // 获取Power
    QString getPower()
    {
        return getDisplayPower(voltage*voltage);
    }

    QString getEnergy()
    {
        //return getDisplayPower(voltage*voltage)*onTime*pulseCount*0.001;
        return QString("");
    }

    QString getLayer(Layer layer)
    {
        switch (layer) {
        case Layer_down:
            return "Lower";
            break;
        case Layer_middle:
            return "Middle";
            break;
        case Layer_up:
            return "Upper";
            break;

        default:
            return "null";
            break;
        }
    }


//    QString getTotalPlanFromColumn(int index)
//    {
//        switch (index)
//        {
//        case 0:
//            // ID
//            return id;
//            break;
//        case 1:
//            // 层
//            return getLayer(layer);
//            break;
//        case 2:
//            // Angle
//            return QString(QString::number(usAngle)).append("°");
//            break;
//        case 3:
//            // Energy
//            return QString(QString::number(getEnergy())).append("W");
//            break;
//        case 4:
//            // Pulses
//            return QString(QString::number(pulseCount));
//            break;

//        default:
//            return QString("");
//        }
//    }

    QString getPlanParaFromColumn(int index)
    {
        switch (index)
        {
        case 0:
            // ID
            return id;
            break;
        case 1:
            // 层
            return getLayer(layer);
            break;

        case 2:
            return QString::number(usAngle);
            break;

        case 3:
            // Energy
            return getPower();
            break;
        case 4:

            // Pulses
            return QString::number(pulseCount);
            break;

        default:
            return QString("");
        }
    }

};

#endif // IRRADIATESPOT_H_

