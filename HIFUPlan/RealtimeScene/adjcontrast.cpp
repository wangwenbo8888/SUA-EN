
#include "adjcontrast.h"
#include "math.h"

AdjContrast::AdjContrast()
    : m_fContrast(1.0f)
{
    for (int i = 0; i < 256;++i)
    {
        colorTableR[i] = i;
        colorTableG[i] = i;
        colorTableB[i] = i;
    }
}

AdjContrast::~AdjContrast()
{

}

void AdjContrast::setContrast(float contrast)
{
    m_fContrast = contrast;
}

int clamp(const int x)
{
    if (x > 255)
        return 255;
    if (x < 0)
        return 0;

    return x;
}

void AdjContrast::setTable(QImage image)
{
    if (fabs(m_fContrast-1.0f)<0.1)
    {
        return;
    }

    QImage temp = image;
    switch(image.format())
    {
    case QImage::Format_RGB32:
         int row, col;
         int srcWidth, srcHeight;
         int rgbmeans[3];
         double redSum, greenSum, blueSum;
         double total;

         //r、g、b像素值累加
         redSum = 0;
         greenSum = 0;
         blueSum = 0;
            //
         srcWidth = temp.width();
         srcHeight = temp.height();
         total = srcWidth * srcHeight;

         int tr = 0, tg = 0, tb = 0;
         //获取rgb means
         for(row = 0; row < srcHeight; row++)
         {
             uchar* ptr = temp.bits()+4*srcWidth*row;
             for(col = 0; col < srcWidth; col++)
             {
                 tr =  ptr[2];
                 tg =  ptr[1];
                 tb =  ptr[0];

                 redSum += tr;
                 greenSum += tg;
                 blueSum +=tb;

                 ptr += 4;
             }
         }
         rgbmeans[0] = (int)(redSum / total);
         rgbmeans[1] = (int)(greenSum / total);
         rgbmeans[2] = (int)(blueSum / total);

         for (int i = 0; i < 256;++i)
         {
             colorTableR[i] = clamp((i-rgbmeans[0])+(int)rgbmeans[0]*m_fContrast);
             colorTableG[i] = clamp((i-rgbmeans[1])+(int)rgbmeans[1]*m_fContrast);
             colorTableB[i] = clamp((i-rgbmeans[2])+(int)rgbmeans[2]*m_fContrast);
         }

        break;
    }
}

uchar* AdjContrast::getTableR()
{
    return colorTableR;
}

uchar* AdjContrast::getTableG()
{
    return colorTableG;
}

uchar* AdjContrast::getTableB()
{
    return colorTableB;
}
