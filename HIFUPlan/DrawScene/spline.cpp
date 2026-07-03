
#include "spline.h"

#include "QMap"

Spline::Spline(float scal)
: m_fScal(scal)
{
}

//std::vector<QPointF> Spline::simplyOutline(const std::vector<std::vector<QPointF> >& points)
//{
//    double x;
//    double y;
//    int count = 0;
//    for (int j = 0; j < points.size();++j)
//    {
//        for (int i = 0; i < points[j].size(); ++i)
//        {
//            x += points[j][i].x();
//            y += points[j][i].y();
//            ++count;
//        }
//    }

//    m_pCenter = QPointF(x/count,y/count);

//    QMap<float,QPointF> mapTemp;
//    for (int j = 0; j < points.size();++j)
//    {
//        for (int i = 0; i < points[j].size(); ++i)
//        {
//            const QPointF& point = points[j][i];
//            mapTemp.insert(angle(m_pCenter,point),point);
//        }
//    }

//    m_vOutline.clear();
//    QMap<float,QPointF>::iterator iter = mapTemp.begin();
//    m_vOutline.push_back(iter.value());
//    for (iter = mapTemp.begin();iter != mapTemp.end(); iter+=5)
//    {
//        m_vOutline.push_back(iter.value());
//    }

//}

QPainterPath Spline::getPath()
{
    QPainterPath path;
    path.moveTo(m_vOutline[0]);
    for (int i = 1; i < m_vOutline.size();++i)
    {
        path.lineTo(m_vOutline[i]);
    }

    path.lineTo(m_vOutline[0]);

    return path;
}

//QPointF Spline::findNearest(QMap<float,QPointF> map,float angle)
//{
//    QMap<float,QPointF>::iterator iter = map.begin();
//    float header;
//    float tail;
//    for (iter = map.begin();iter != map.end(); ++iter)
//    {

//    }
//}

//std::vector<std::vector<QPointF> > Spline::deform(int index,QPointF dis)
//{

//}

float Spline::angle(QPointF center, QPointF point)
{
    float angle_temp;
    float xx, yy;

    xx = point.x() - center.x();
    yy = point.y() - center.y();

    if (xx == 0.0)
        angle_temp = PI*0.5;
   else
        angle_temp = atan(fabs(yy / xx));

    if ((xx < 0.0) && (yy >= 0.0))
        angle_temp = PI - angle_temp;
    else if ((xx < 0.0) && (yy < 0.0))
        angle_temp = PI + angle_temp;
   else if ((xx >= 0.0) && (yy < 0.0))
        angle_temp = PI * 2.0 - angle_temp;

   return (angle_temp);
}
