

#include <math.h>
#include <QImage>
#include <QPixmap>
#include "time.h"

#include "marchingcubes.h"
#include "hifu3d.h"
#include "drawscene.h"
#ifdef DEBUG
#include <QDebug>
#endif

#include "createmodel.h"

#define DELTA 0.0000001
#define PI    3.1415926


Q_LOGGING_CATEGORY(HIFU3D,"HIFU3D")

Hifu3d::Hifu3d(QObject *parent)
{
    _pData = NULL;
    _pImages = NULL;
    _pMaskImages = NULL;
    _pImagesCublic = NULL;

    _RAbove = NULL;
    _RBelow = NULL;
    _ThetaAbove = NULL;
    _ThetaBelow = NULL;

    _InterpRAbove = NULL;
    _InterpRBelow = NULL;
    _InterpThetaAbove = NULL;
    _InterpThetaBelow = NULL;

    _RSeries = NULL;
    _ThetaSeries = NULL;

    _RAboveSquare = NULL;
    _RBelowSquare = NULL;
    _ThetaAboveSquare = NULL;
    _ThetaBelowSquare = NULL;

    _RCublicCoeff = NULL;      // 一个像素点的四个系数连续排列
    _ThetaCublicCoeff = NULL;  // 一个像素点的四个系数连续排列

//    CreateModel create;
//    Mesh ball;
//    ball.ReadStl("E:\\USgHIFU\\lib\\transducer.stl");
//    create.createBall(&ball);
//    ball.MoveCenter();
//    ball.WriteStl("E:\\ball.stl");

}

Hifu3d::~Hifu3d()
{
    if (_pData!=NULL)
    {
        delete []_pData;
        _pData = NULL;
    }

    if (_RAbove != NULL)
    {
        delete []_RAbove;
        _RAbove = NULL;
    }

    if (_RBelow != NULL)
    {
        delete []_RBelow;
        _RBelow = NULL;
    }

    if (_ThetaAbove != NULL)
    {
        delete []_ThetaAbove;
        _ThetaAbove = NULL;
    }

    if (_ThetaBelow != NULL)
    {
        delete []_ThetaBelow;
        _ThetaBelow = NULL;
    }

    if (_InterpRAbove != NULL)
    {
        delete []_InterpRAbove;
        _InterpRAbove = NULL;
    }

    if (_InterpRBelow != NULL)
    {
        delete []_InterpRBelow;
        _InterpRBelow = NULL;
    }

    if (_InterpThetaAbove != NULL)
    {
        delete []_InterpThetaAbove;
        _InterpThetaAbove = NULL;
    }

    if (_InterpThetaBelow != NULL)
    {
        delete []_InterpThetaBelow;
        _InterpThetaBelow = NULL;
    }

    if (_pImages!=NULL)
    {
        delete []_pImages;
        _pImages = NULL;
    }

    if (_pMaskImages != NULL)
    {
        delete []_pMaskImages;
        _pMaskImages = NULL;
    }

    if (_RSeries!=NULL)
    {
        delete []_RSeries;
        _RSeries = NULL;
    }

    if (_ThetaSeries!=NULL)
    {
        delete []_ThetaSeries;
        _ThetaSeries = NULL;
    }

    if (_RAboveSquare != NULL)
    {
        delete []_RAboveSquare;
        _RAboveSquare = NULL;
    }

    if (_RBelowSquare != NULL)
    {
        delete []_RBelowSquare;
        _RBelowSquare = NULL;
    }

    if (_ThetaAboveSquare != NULL)
    {
        delete []_ThetaAboveSquare;
        _ThetaAboveSquare = NULL;
    }

    if (_ThetaBelowSquare != NULL)
    {
        delete []_ThetaBelowSquare;
        _ThetaBelowSquare = NULL;
    }

    if (_RCublicCoeff != NULL)      // 一个像素点的四个系数连续排列
    {
        delete []_RCublicCoeff;
        _RCublicCoeff = NULL;
    }

    if (_ThetaCublicCoeff != NULL)  // 一个像素点的四个系数连续排列
    {
        delete []_ThetaCublicCoeff;
        _ThetaCublicCoeff = NULL;
    }

    if (_pImagesCublic!=NULL)
    {
        delete []_pImagesCublic;
        _pImagesCublic = NULL;
    }

}


Mesh* Hifu3d::getMesh(uchar* mask)
{
    setMask(mask);
    qDebug()<<"set mask success !";
    RadiToParallelByLine();
    qDebug()<<"radio to parallel success !";
    SMCProcessor algo(_pMaskImages,_wide,_wide,_high);
    qDebug()<<"construct algo success !";
    return algo.GetSurface();
}

bool Hifu3d::LoadImages(const QMap<int,DrawScene*>& scenes,int focusX,int focusY,int wide, int high,int num)
{
    _wide = wide;
    _high = high;
    _num = num;
    _focusX = focusX;
    _focusY = focusY;

    int dataSize = wide*high*num;
    if (_pData==NULL)
    {
        _pData = new uchar[dataSize];
    }
    memset(_pData,0,dataSize);
    uchar* pdata = _pData;

    QMap<int,DrawScene*>::const_iterator iter = scenes.begin();
    for (;iter != scenes.end(); ++iter)
    {
        pdata = _pData + wide*high*sizeof(uchar)*(iter.key()+90);
        QImage image = iter.value()->getPixmap().toImage();
        QImage newImage = image.copy(focusX-wide*0.5,focusY-high*0.5,wide,high);
        newImage = ToGray(newImage);
        memcpy(pdata,newImage.bits(),wide*high*sizeof(uchar));
    }

    CalcInterpCoeff();

    return true;
}

bool Hifu3d::LoadImages(const QString& path,int focusX,int focusY,int wide, int high,int num)
{
    qDebug()<<"image wide is "<<wide;
    qDebug()<<"image height is "<<high;
    qDebug()<<"image num is "<<num;

    _wide = wide;
    _high = high;
    _num = num;
    _focusX = focusX;
    _focusY = focusY;

    // clock_t time0 = clock();

    QString fileNameHeader(path);
    int dataSize = wide*high*num;
    _pData = new uchar[dataSize];
    memset(_pData,0,dataSize);
    fileNameHeader.append("\\Target_Image_Sequence_");
    uchar* pdata = _pData;
    for (int i = 0; i < _num; ++i)
    {
        QString fileName(fileNameHeader);
        fileName.append(QString::number(i-90));
        fileName.append("_degree.jpg");
        QImage image = QImage(fileName);
        qDebug()<<"load image file name is "<<fileName;
        QImage newImage = image.copy(focusX-wide*0.5,focusY-high*0.5,wide,high);
        newImage = ToGray(newImage);
        qDebug()<<"cliped image to gray success!";
//        QString name("C:\\Users\\HIFU\\Desktop\\fold\\test");
//        name.append(QString::number(i));
//        name.append(".jpeg");
//        newImage.save(name);
        memcpy(pdata,newImage.bits(),wide*high*sizeof(uchar));
        qDebug()<<"copy data success!";
        pdata += wide*high*sizeof(uchar);
    }

//    clock_t time1 = clock();
//    qDebug()<<"read and clip image time consume "<< time1-time0;
//    FILE* file = fopen("D:\\source.bin","wb");
//    if (file != NULL)
//    {
//        fwrite(_pData,1,_wide*_high*_num,file);
//        fclose(file);
//        file = NULL;
//    }

    CalcInterpCoeff();

//    clock_t time2 = clock();
//    qDebug()<<"calc coeff time consume is "<< time2-time1;

//    file = fopen("D:\\RAbove.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_RAbove,1,_wide*_wide*sizeof(int),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\RBelow.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_RBelow,1,_wide*_wide*sizeof(int),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\ThetaAbove.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_ThetaAbove,1,_wide*_wide*sizeof(int),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\ThetaBelow.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_ThetaBelow,1,_wide*_wide*sizeof(int),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\InterpRAbove.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_InterpRAbove,1,_wide*_wide*sizeof(float),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\InterpRBelow.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_InterpRBelow,1,_wide*_wide*sizeof(float),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\InterpThetaAbove.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_InterpThetaAbove,1,_wide*_wide*sizeof(float),file);
//        fclose(file);
//        file = NULL;
//    }

//    file = fopen("D:\\InterpThetaBelow.bin","wb");
//    if (file!=NULL)
//    {
//        fwrite(_InterpThetaBelow,1,_wide*_wide*sizeof(float),file);
//        fclose(file);
//        file = NULL;
//    }

    return true;
}

float Hifu3d::sign(float x)
{
    if (x > DELTA)
    {
        return 1.0;
    }
    else if (fabs(x)<=DELTA)
    {
        return 0.0;
    }
    else
    {
        return -1.0;
    }
}

void Hifu3d::setMask(uchar* data)
{
    _pMask = data;
}

float Hifu3d::GetRadius(float x, float y)
{
    if (fabs(y)<DELTA)
    {
        return x;
    }
    else
    {
        return sign(y)*sqrt(x*x+y*y);
    }
}

float Hifu3d::GetTheta(float x, float y)
{
    if (fabs(y)<DELTA)
    {
        return 0.0;
    }
    else
    {
        return acos(sign(y)*x/sqrt(x*x+y*y));
    }
}

int Hifu3d::GetAboveSquareIndex(int index, int size)
{
    if (index<size-1)
    {
        return ++index;
    }
    else
    {
        return size-1;
    }
}

int Hifu3d::GetBelowSquareIndex(int index)
{
    if (index > 0)
    {
        return --index;
    }
    else
    {
        return 0;
    }
}

// data里面数据的顺序是从小到大
int Hifu3d::GetAboveIndex(float value, float* data, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (data[i]>value)
        {
            return i;
        }
    }
}

// data里面数据的顺序是从小到大
int Hifu3d::GetBelowIndex(float value, float* data, int size)
{
    for (int i = size-1; i >= 0; --i)
    {
        if (data[i]<=value)
        {
            return i;
        }
    }
}

//
//         |1-2*abs(x)^2+abs(x)^3             0<=abs(x)<1
//  s(x) = |4-8*abs(x)+5*abs(x)^2-abs(x)^3    1<=abs(s)<2
//         |0                                 abs(x)>=2
//
float Hifu3d::SFun(float f)
{
    float abs = fabs(f);
    if (abs<1.0)
    {
        return 1.0-2.0*abs*abs+abs*abs*abs;
    }
    else if(abs < 2.0)
    {
        return 4.0-8.0*abs+5.0*abs*abs-abs*abs*abs;
    }
    else
    {
        return 0.0;
    }
}

void Hifu3d::CalcInterpCoeff()
{
    int size = _wide*_wide;
    _RAbove = new int[size];
    memset(_RAbove,0,size*sizeof(int));
    _RBelow = new int[size];
    memset(_RBelow,0,size*sizeof(int));
    _ThetaAbove = new int[size];
    memset(_ThetaAbove,0,size*sizeof(int));
    _ThetaBelow  = new int[size];
    memset(_ThetaBelow,0,size*sizeof(int));

    _InterpRAbove = new float[size];
    memset(_InterpRAbove,0,size*sizeof(int));
    _InterpRBelow  = new float[size];
    memset(_InterpRBelow,0,size*sizeof(int));
    _InterpThetaAbove = new float[size];
    memset(_InterpThetaAbove,0,size*sizeof(int));
    _InterpThetaBelow = new float[size];
    memset(_InterpThetaBelow,0,size*sizeof(int));

    _RSeries = new float[_wide];
    memset(_RSeries,0,_wide*sizeof(float));

    _ThetaSeries = new float[_wide];
    memset(_ThetaSeries,0,_wide*sizeof(float));

    _RAboveSquare = new int[size];
    memset(_RAboveSquare,0,size*sizeof(int));

    _RBelowSquare = new int[size];
    memset(_RBelowSquare,0,size*sizeof(int));

    _ThetaAboveSquare = new int[size];
    memset(_ThetaAboveSquare,0,size*sizeof(int));

    _ThetaBelowSquare = new int[size];
    memset(_ThetaBelowSquare,0,size*sizeof(int));

    _RCublicCoeff = new float[4*size];      // 一个像素点的四个系数连续排列
    memset(_RCublicCoeff,0,4*size*sizeof(float));

    _ThetaCublicCoeff = new float[4*size];  // 一个像素点的四个系数连续排列
    memset(_ThetaCublicCoeff,0,4*size*sizeof(float));

    float radius = 0.0;
    float theta = 0.0;
    const float skip = 0.5*(_wide-1);

    for (int i = 0; i <_num;++i)
    {
       _ThetaSeries[i] = i*PI/_num;
    }

    for (int i = 0; i<_wide;++i)
    {
        _RSeries[i] = i-skip;
    }

    for (int i = 0; i< _wide; ++i)
    {
        for (int j = 0; j < _wide; ++j)
        {
            int index = i*_wide+j;
            radius = GetRadius(j-skip,i-skip);
            theta = GetTheta(j-skip,i-skip);

            if (radius <= _RSeries[_wide-1])
            {
                _RAbove[index] = GetAboveIndex(radius,_RSeries,_wide);
            }
            else
            {
                _RAbove[index] = _wide-1;
            }
            _InterpRAbove[index] = _RSeries[_RAbove[index]]-radius;

            _RAboveSquare[index] = GetAboveSquareIndex(_RAbove[index],_wide);

            for (int k = 0;k<4;++k)
            {
                _RCublicCoeff[4*index+k] = SFun(1-k+_InterpRAbove[index]);
            }

            if (_RSeries[0] < radius)
            {
                _RBelow[index] = GetBelowIndex(radius,_RSeries,_wide);
            }
            else
            {
                _RBelow[index] = 0;
            }
            _InterpRBelow[index] = 1.0-_InterpRAbove[index];
            _RBelowSquare[index] = GetBelowSquareIndex(_RBelow[index]);


            if (theta < _ThetaSeries[_num-1])
            {
                _ThetaAbove[index] = GetAboveIndex(theta,_ThetaSeries,_num);
            }
            else
            {
                _ThetaAbove[index] = _num -1;
            }
            _InterpThetaAbove[index] = (_ThetaSeries[_ThetaAbove[index]]-theta)*_num/PI;
            _ThetaAboveSquare[index] = GetAboveSquareIndex(_ThetaAbove[index],_num);

            for (int k = 0; k <4;++k)
            {
                _ThetaCublicCoeff[4*index+k] = SFun(1-k+_InterpThetaAbove[index]);
            }

            if (_ThetaSeries[0] < theta)
            {
                _ThetaBelow[index] = GetBelowIndex(theta,_ThetaSeries,_num);
            }
            else
            {
                _ThetaBelow[index] = 0;
            }
            _InterpThetaBelow[index] = 1.0 -_InterpThetaAbove[index];
            _ThetaBelowSquare[index] = GetBelowSquareIndex(_ThetaBelow[index]);

        }
    }
}

uchar Hifu3d::GetPixelData(int high,int radius,int angle)
{
    int index = angle*_wide*_high + high*_wide+radius;
    if (index < _wide*_high*_num)
    {
        return _pData[index];
    }
    else
    {
        return 0;
    }
}

uchar Hifu3d::GetMaskData(int high,int radius, int angle)
{
    int index = angle*_wide*_high+high*_wide+radius;
    if (index < _wide*_high*_num)
    {
        return _pMask[index];
    }
    else
    {
        return 0;
    }
}

// 将彩色图像转换成灰度图像
QImage Hifu3d::ToGray(QImage image )
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    }
    return ret;
}

uchar* Hifu3d::GetImage(int index)
{
    int size = _wide*_wide;
    uchar* pixelData = new uchar[size];
    memset(pixelData,0,size*sizeof(uchar));
    if (index < 0 || index > _wide)
    {
        return pixelData;
    }

    int imageIndex = 0;
    int pixel = 0;
    for (int j = 0; j <_wide; ++j)
    {
        for (int k = 0; k < _wide; ++k)
        {
            imageIndex = j*_wide + k;
            pixel = (_RCublicCoeff[4*imageIndex]*GetPixelData(index,_RBelowSquare[imageIndex],_ThetaBelowSquare[imageIndex])
                     +_RCublicCoeff[4*imageIndex+1]*GetPixelData(index,_RBelow[imageIndex],_ThetaBelowSquare[imageIndex])
                     +_RCublicCoeff[4*imageIndex+2]*GetPixelData(index,_RAbove[imageIndex],_ThetaBelowSquare[imageIndex])
                     +_RCublicCoeff[4*imageIndex+3]*GetPixelData(index,_RAboveSquare[imageIndex],_ThetaBelowSquare[imageIndex]))*_ThetaCublicCoeff[4*imageIndex]
                    +(_RCublicCoeff[4*imageIndex]*GetPixelData(index,_RBelowSquare[imageIndex],_ThetaBelow[imageIndex])
                     +_RCublicCoeff[4*imageIndex+1]*GetPixelData(index,_RBelow[imageIndex],_ThetaBelow[imageIndex])
                     +_RCublicCoeff[4*imageIndex+2]*GetPixelData(index,_RAbove[imageIndex],_ThetaBelow[imageIndex])
                     +_RCublicCoeff[4*imageIndex+3]*GetPixelData(index,_RAboveSquare[imageIndex],_ThetaBelow[imageIndex]))*_ThetaCublicCoeff[4*imageIndex+1]
                     +(_RCublicCoeff[4*imageIndex]*GetPixelData(index,_RBelowSquare[imageIndex],_ThetaAbove[imageIndex])
                     +_RCublicCoeff[4*imageIndex+1]*GetPixelData(index,_RBelow[imageIndex],_ThetaAbove[imageIndex])
                     +_RCublicCoeff[4*imageIndex+2]*GetPixelData(index,_RAbove[imageIndex],_ThetaAbove[imageIndex])
                     +_RCublicCoeff[4*imageIndex+3]*GetPixelData(index,_RAboveSquare[imageIndex],_ThetaAbove[imageIndex]))*_ThetaCublicCoeff[4*imageIndex+2]
                     +(_RCublicCoeff[4*imageIndex]*GetPixelData(index,_RBelowSquare[imageIndex],_ThetaAboveSquare[imageIndex])
                     +_RCublicCoeff[4*imageIndex+1]*GetPixelData(index,_RBelow[imageIndex],_ThetaAboveSquare[imageIndex])
                     +_RCublicCoeff[4*imageIndex+2]*GetPixelData(index,_RAbove[imageIndex],_ThetaAboveSquare[imageIndex])
                     +_RCublicCoeff[4*imageIndex+3]*GetPixelData(index,_RAboveSquare[imageIndex],_ThetaAboveSquare[imageIndex]))*_ThetaCublicCoeff[4*imageIndex+3];
            if (pixel < 0)
            {
                pixelData[imageIndex] = 0;
            }
            else if(pixel > 255)
            {
                pixelData[imageIndex] = 255;
            }
            else
            {
                pixelData[imageIndex] = pixel;
            }
         }
    }

    return pixelData;
}

bool Hifu3d::RadiToParallelByCublic()
{
    int size = _wide*_wide*_high;
    _pImagesCublic = new int[size];
    memset(_pImagesCublic,0,size*sizeof(int));
    int totalIndex = 0;
    int imageIndex = 0;
    int pixel = 0;
    for (int i = 0; i < _high; ++i)
    {
        for (int j = 0; j <_wide; ++j)
        {
            for (int k = 0; k < _wide; ++k)
            {
                totalIndex = i*_wide*_wide + j * _wide +k;
                imageIndex = j*_wide + k;
                pixel = (_RCublicCoeff[4*imageIndex]*GetPixelData(i,_RBelowSquare[imageIndex],_ThetaBelowSquare[imageIndex])
                        +_RCublicCoeff[4*imageIndex+1]*GetPixelData(i,_RBelow[imageIndex],_ThetaBelowSquare[imageIndex])
                        +_RCublicCoeff[4*imageIndex+2]*GetPixelData(i,_RAbove[imageIndex],_ThetaBelowSquare[imageIndex])
                        +_RCublicCoeff[4*imageIndex+3]*GetPixelData(i,_RAboveSquare[imageIndex],_ThetaBelowSquare[imageIndex]))*_ThetaCublicCoeff[4*imageIndex]
                        +(_RCublicCoeff[4*imageIndex]*GetPixelData(i,_RBelowSquare[imageIndex],_ThetaBelow[imageIndex])
                        +_RCublicCoeff[4*imageIndex+1]*GetPixelData(i,_RBelow[imageIndex],_ThetaBelow[imageIndex])
                        +_RCublicCoeff[4*imageIndex+2]*GetPixelData(i,_RAbove[imageIndex],_ThetaBelow[imageIndex])
                        +_RCublicCoeff[4*imageIndex+3]*GetPixelData(i,_RAboveSquare[imageIndex],_ThetaBelow[imageIndex]))*_ThetaCublicCoeff[4*imageIndex+1]
                        +(_RCublicCoeff[4*imageIndex]*GetPixelData(i,_RBelowSquare[imageIndex],_ThetaAbove[imageIndex])
                        +_RCublicCoeff[4*imageIndex+1]*GetPixelData(i,_RBelow[imageIndex],_ThetaAbove[imageIndex])
                        +_RCublicCoeff[4*imageIndex+2]*GetPixelData(i,_RAbove[imageIndex],_ThetaAbove[imageIndex])
                        +_RCublicCoeff[4*imageIndex+3]*GetPixelData(i,_RAboveSquare[imageIndex],_ThetaAbove[imageIndex]))*_ThetaCublicCoeff[4*imageIndex+2]
                        +(_RCublicCoeff[4*imageIndex]*GetPixelData(i,_RBelowSquare[imageIndex],_ThetaAboveSquare[imageIndex])
                        +_RCublicCoeff[4*imageIndex+1]*GetPixelData(i,_RBelow[imageIndex],_ThetaAboveSquare[imageIndex])
                        +_RCublicCoeff[4*imageIndex+2]*GetPixelData(i,_RAbove[imageIndex],_ThetaAboveSquare[imageIndex])
                        +_RCublicCoeff[4*imageIndex+3]*GetPixelData(i,_RAboveSquare[imageIndex],_ThetaAboveSquare[imageIndex]))*_ThetaCublicCoeff[4*imageIndex+3];
                if (pixel < 0)
                {
                    _pImagesCublic[totalIndex] = 0;
                }
                else
                {
                    _pImagesCublic[totalIndex] = pixel;
                }
            }
        }
    }

    FILE* file = fopen("D:\\cublic.bin","wb");
    if (file == NULL)
    {
        return false;
    }
    fwrite(_pImagesCublic,1,_wide*_wide*_high*sizeof(int),file);

    fclose(file);
    return true;
}

bool Hifu3d::RadiToParallelByLine()
{
    int size = _wide*_wide*_high;
    if(_pMaskImages==NULL)
    {
        _pMaskImages = new uchar[size];
    }
    memset(_pMaskImages,0,size*sizeof(uchar));
    int totalIndex = 0;
    int imageIndex = 0;
    for (int i = 0; i < _high; ++i)
    {
        for (int j = 0; j <_wide; ++j)
        {
            for (int k = 0; k < _wide; ++k)
            {
                totalIndex = i*_wide*_wide + j * _wide +k;
                imageIndex = j*_wide + k;
                _pMaskImages[totalIndex] = _InterpRAbove[imageIndex]*_InterpThetaAbove[imageIndex]*GetMaskData(i,_RAbove[imageIndex],_ThetaAbove[imageIndex])
                                      +_InterpRAbove[imageIndex]*_InterpThetaBelow[imageIndex]*GetMaskData(i,_RAbove[imageIndex],_ThetaBelow[imageIndex])
                                      +_InterpRBelow[imageIndex]*_InterpThetaAbove[imageIndex]*GetMaskData(i,_RBelow[imageIndex],_ThetaAbove[imageIndex])
                                      +_InterpRBelow[imageIndex]*_InterpThetaBelow[imageIndex]*GetMaskData(i,_RBelow[imageIndex],_ThetaBelow[imageIndex]);
            }
        }
    }

//    FILE* file = fopen("D:\\testdata.bin","wb");
//    if (file == NULL)
//    {
//        return false;
//    }
//    fwrite(_pMaskImages,1,_wide*_wide*_high,file);

//    fclose(file);

    return true;
}


