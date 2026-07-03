#ifndef HIFUIMAGEALGO_H_
#define HIFUIMAGEALGO_H_

#include <QLoggingCategory>

#include "constant.h"
#include "hifuimagealgo_global.h"
//#include ""

class DrawScene;
class RealtimeScene;

Q_DECLARE_LOGGING_CATEGORY(HIFUIMAGEALGO)

class HIFUIMAGEALGO_EXPORT HifuImageAlgo
{

public:
//    Regist(DrawScene* scene,QImage* image, int radius);

//    Regist(DrawScene* scene,QImage* contour);

    ~HifuImageAlgo();

//    bool match(int usAngle,float& x,float& y,float& rotatedAngle,float& score);

//    bool move(const QPointF& p);

    double deepthmeasure(QImage image);

private:

//    DrawScene* m_pScene;
//    QImage* m_pImage;
//    RealtimeScene* _realtimescene;

    QImage* m_pContour;
    int m_iRadius;

};

#endif // REGIST_H_
