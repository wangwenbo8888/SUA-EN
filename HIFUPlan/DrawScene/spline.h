#ifndef SPLINE
#define SPLINE

#include "QPointF"
#include <vector>
#include <QMap>

#include "qpainterpath.h"

#include "math.h"

#define PI 3.1415926

class Spline
{
public:

    Spline(float scal);

//    std::vector<QPointF> simplyOutline(const std::vector<std::vector<QPointF> >& points);

    std::vector<std::vector<QPointF> > deform(int index,QPointF dis);

    std::vector<QPointF> getOutline() const
    {
        return m_vOutline;
    }

    QPainterPath getPath();

private:



   float angle(QPointF center, QPointF point);

//   QPointF findNearest(QMap<float,QPointF> map,float angle);

   std::vector<QPointF> m_vOutline;
   float m_fScal;
   QPointF m_pCenter;
};

#endif // SPLINE

