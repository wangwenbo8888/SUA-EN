#ifndef IRRADIATESPOT_H_
#define IRRADIATESPOT_H_

#include <QPointF>
#include "constant.h"
#include "qdebug.h"
#include "math.h"

#include "commondefine.h"
// #define PI 3.1415926

// 辐照计划文件
enum Layer
{
    Layer_down = 0,   // 下层辐照点
    Layer_middle,     // 中层辐照点
    Layer_up,         // 上层辐照点

    Layer_end
};

enum Irradiate_model
{
    Model_inner_to_outer = 0,    // 从内到外辐照
    Model_outer_to_inner,

    Model_unknown
};

// 辐照点信息
struct NewPoint3d
{
    double x;
    double y;
    double z;
};

int powerToVoltage(int power);
int energyToVoltage(int energy);

// 各辐照点参数
struct Irradiate_Spot
{
    QString id;          // 辐照点id
    QString idBorn;      // 创建的时候对应的点ID
    int focusCount;      // 焦点个数，单焦点还是多焦点
    int pulseCount;      // 脉冲个数
    int coolingTime;     // 冷却时间
    double voltage;         // 电压
    int onTime;          // 施加脉冲持续时间
    int offTime;         // 停止脉冲持续时间
    int usAngle;         // 做计划时对应的超声探头角度
    double spotAngle;        // 辐照点与中心连线与x轴夹角
    Layer layer;         // 辐照点所处的层
    Circle circle;          // 圈
    NewPoint3d position;    // 辐照点对应的空间位置
    bool isIrradiated;   // 是否辐照过

    Irradiate_Spot()
    {
        id = -1;               // 辐照点id
        idBorn = -1;
        focusCount = 0;        // 焦点个数，单焦点还是多焦点
        pulseCount = 0;        // 脉冲个数
        coolingTime = 0;       // 冷却时间
        voltage = 0;           // 电压
        onTime = 0;            // 施加脉冲持续时间
        offTime = 0;           // 停止脉冲持续时间
        usAngle = 0;           // 做计划时对应的超声探头角度
        spotAngle = 0;
        circle = Circle_end;   // 圈
        layer = Layer_end;     // 辐照点所处的层
        isIrradiated= false;
    }

    void operator = (const Irradiate_Spot& spot)
    {
        id     = spot.id;                       // 辐照点id
        idBorn = spot.idBorn;                   // 创建时点id
        focusCount  = spot.focusCount;          // 焦点个数，单焦点还是多焦点
        pulseCount  = spot.pulseCount;          // 脉冲个数
        coolingTime = spot.coolingTime;         // 冷却时间
        voltage = spot.voltage;                 // 电压
        onTime  = spot.onTime;                  // 施加脉冲持续时间
        offTime = spot.offTime;                 // 停止脉冲持续时间
        usAngle = spot.usAngle;                 // 做计划时对应的超声探头角度
        spotAngle   = spot.spotAngle;
        layer   = spot.layer;                   // 辐照点所处的层
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

    // 从下向上，从小角度到大角度，从超声探头正方向到负方向排列
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

    // 获取功率
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
            return "下层";
            break;
        case Layer_middle:
            return "中层";
            break;
        case Layer_up:
            return "上层";
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
//            // 角度
//            return QString(QString::number(usAngle)).append("°");
//            break;
//        case 3:
//            // 能量
//            return QString(QString::number(getEnergy())).append("瓦");
//            break;
//        case 4:
//            // 脉冲数
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
            // 能量
            return getPower();
            break;
        case 4:

            // 脉冲数
            return QString::number(pulseCount);
            break;

        default:
            return QString("");
        }
    }

};

#endif // IRRADIATESPOT_H_

