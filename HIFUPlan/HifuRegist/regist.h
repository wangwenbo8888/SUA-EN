#ifndef REGIST_H_
#define REGIST_H_

#include <QLoggingCategory>

#include "constant.h"
#include "regist_global.h"

class DrawScene;


double RESOLUTION = 2.07692;                           // unit: pixel/mm
double ZERO_DEVIATION = 2.2*RESOLUTION;
double X_OFFSET = 274;                                    // unit: pixel
double Y_OFFSET = 294;
// 换算成140mm应该是多少个像素
double FOCAL_PLANE_140MM = 140 * RESOLUTION+ZERO_DEVIATION;   // unit: pixel/mm
float delta = 7.2;

Q_DECLARE_LOGGING_CATEGORY(HIFUREGIST)

class HIFUREGIST_EXPORT Regist
{

public:
    Regist(DrawScene* scene,QImage* image, int radius);

    Regist(DrawScene* scene,QImage* contour);

    ~Regist();

    bool match(int usAngle,float& x,float& y,float& rotatedAngle,float& score);

    bool move(const QPointF& p);

private:

    DrawScene* m_pScene;
    QImage* m_pImage;

    QImage* m_pContour;
    int m_iRadius;

};

#endif // REGIST_H_
