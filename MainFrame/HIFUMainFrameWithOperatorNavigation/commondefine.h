#ifndef COMMONDEFINE_H_
#define COMMONDEFINE_H_

#include <QColor>
#include <qpoint>
#include <qimage.h>
#include "math.h"

class Mesh;

#define PI 3.14159265

const QString NOT_WORK = "color: rgb(146, 146, 146);border-radius:5px;";
const QString WORK = "color: rgb(4, 222, 230);border-radius:5px;";
// const QString SELECTED = "background-color: rgb(4, 221, 229);color:rgb(0,0,0);border-radius:5px;";
// 背景象牙白
const QString SELECTED = "background-color: rgb(250, 255, 240);color:rgb(0,0,0);border-radius:5px;";
const QString OPERATING = "background-color: rgb(0, 255, 0);color:rgb(0,0,0);border-radius:5px;";

const QString CHANGED = "color:rgb(58,95,205);";
const QString UNCHANGED = "color:rgb(255,255,255);";

const QString SELECTED_NO_ROUND = "background-color: rgb(250, 255, 240);color:rgb(0,0,0);";
const QString NOT_SELECTED = "background-color: rgb(55, 55, 55);border-radius:5px;";
const QString EDIT_AVAIABLE = "background-color: rgb(250, 255, 240);background-image: url(:/plan/img/actionbar/button01.png);"
                              "color: rgb(0, 0, 0);";

const QString UNABLE = "background-color: rgb(0, 0, 0);border-radius:5px;";
const QString REDCOLOR = "background-color: rgb(255, 0, 0);border-radius:5px;";

const QString HORIZON_TYPE = //滑条
        "QSlider::groove:horizontal{"
        " border: 1px solid #999999;"
        " height: 5px; "

        "position: absolute;"
        "border-radius :3px;"
        " background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
        " margin: 1px 0;"
        " left: 3px; right: 3px;"
        "}"
        //小滑块
        "QSlider::handle:horizontal {"
        " background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
       "border: 1px solid #5c5c5c;"
        "width: 12px;"
        " height: 12px; "
        "margin: -2px -1; "/* handle is placed by default on the contents rect of the groove. Expand outside the groove */
        "  border-radius: 4px;"
        "}"
        //滑块滑过改变样式
        " QSlider::add-page:horizontal {"
        "border-radius :2px;"
        " margin: 1px 1;"
        "background: white;"
        //  "position: absolute;"
        //  " left: 2px; right: 2px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "border-radius :2px;"
        " margin: 1px 1;"//
        "  background-color:#86dfc6;"
        // "position: absolute;"
        // " left: 2px; right: 2px;"
        "}";


const QColor COMPLETE_COLOR = QColor(99,184,240,200);
const QColor IRADIATING_COLOR= QColor(0,255,0,200);
const QColor UNIRADIATE_COLOR = QColor(65,47,31,200);


//double voltage_1;
//double voltage_2;
//double voltage_3;
//double voltage_4;
//double voltage_5;
//double voltage_6;
//double voltage_7;
//double voltage_8;
//double voltage_9;
//double voltage_10;
//double voltage_11;
//double voltage_12;
//double voltage_13;
//double voltage_14;

// const QColor PREPARE_COLOR= QColor();

#define CURRENT_TABLE_ROWS 6

enum APPEND_LABEL
{
    LABEL_RED,        // 红灯
    LABEL_YELLOW,     // 黄灯
    LABEL_GREEN,      // 绿灯
    SWITCH            // Foot Switch
};

struct TREAT_PARA
{
    int power;         // Power，单位瓦特
    int energy;        // Energy，单位焦耳
    double voltage;       // 电压，单位volts
    int onTime;        // Duration，单位毫秒
    int offTime;       // Interval，单位毫秒
    int pulse;         // Pulse，单位个
    int coolingTime;   // Cooling，单位秒
};

// Treatment协议对应的参数结构
struct Element
{
    int voltage;
    int on_time;
    int off_time;
    int pulse_number;
    int cooling_time;
};

struct Spot_3d_Status
{
    Spot_3d_Status(double x,double y,double& z,const QColor& c)
        : xPos(x)
        , yPos(y)
        , zPos(z)
        , color(c)
    {
        pMesh = NULL;
    }
    double xPos;
    double yPos;
    double zPos;
    QColor color;
    Mesh* pMesh;
    // 颜色
    float r;
    float g;
    float b;
};

struct TOTAL_TIME
{
    TOTAL_TIME()
    {
        blocks = 0;        // 剩余多少个block
        spots = 0;         // 每个block多少个点
        spotsRemain = 0;   // 当前Treatment块剩余多少点
        onTime = 0;        // Duration多少
        offTime = 0;       // Interval多少
        coolingTime = 0;   // Cooling多少
        delayTime = 0;     // 系统延迟Time
        remainTime = -1;    // 当前Treatment剩下总的Time
        spendTime = 0;
    }

    int blocks;        // 剩余多少个block
    int spots;         // 每个block多少个点
    int spotsRemain;   // 当前Treatment块剩余多少点
    int onTime;        // Duration多少
    int offTime;       // Interval多少
    int pulseNumber;   // Pulse
    int coolingTime;   // Cooling多少
    int delayTime;     // 系统延迟Time

    int remainTime;    // Remaining time
    int spendTime;     // 花费Time
};

QImage toGray(QImage image);

void outputTreatmentLog(const QString& str0,const QString& str1=QString(),const QString& str2=QString(),
                        const QString& str3=QString(),const QString& str4=QString(),
                        const QString& str5=QString(),const QString& str6=QString(),
                        const QString& str7=QString(),const QString& str8=QString(),
                        const QString& str9=QString());

QString getDisplayPower(double power);

double getVoltage(int power);

void getsettingvoltage();

bool getVoltageFromDisplay(std::string str,double& value);

#endif // COMMONDEFINE_H_

