#ifndef ADJCONTRAST_H_
#define ADJCONTRAST_H_

#include <QImage>

class AdjContrast
{
public:
    AdjContrast();
    ~AdjContrast();

    void setContrast(float contrast);
    void setTable(QImage image);

    uchar* getTableR();
    uchar* getTableG();
    uchar* getTableB();

private:

    uchar colorTableR[256];
    uchar colorTableG[256];
    uchar colorTableB[256];
    float m_fContrast;
};

#endif // ADJCONTRAST_H_

