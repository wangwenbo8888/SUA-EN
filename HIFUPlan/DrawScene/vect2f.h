#ifndef VECT2F_H_
#define VECT2F_H_

#include <QPointF>

class Vect2f
{
public:
    Vect2f(QPointF p);

    QPointF normal() const;

    // 单位化后的正交向量
    QPointF ortho() const;


private:

    float x;
    float y;
};

#endif // VECT2F_H_

