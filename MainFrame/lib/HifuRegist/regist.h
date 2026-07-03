#ifndef REGIST_H_
#define REGIST_H_

#include <QLoggingCategory>

#include "constant.h"
#include "regist_global.h"

class DrawScene;

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
