

#include "vect2f.h"
#include "math.h"

Vect2f::Vect2f(QPointF p)
: x(p.x())
, y(p.y())
{

}

QPointF Vect2f::normal() const
{
    float model = sqrt(x*x+y*y);
    return(QPointF(x/model,y/model));
}

// 正交分量
QPointF Vect2f::ortho() const
{
    float model = sqrt(x*x+y*y);
    return(QPointF(-y/model,x/model));
}
