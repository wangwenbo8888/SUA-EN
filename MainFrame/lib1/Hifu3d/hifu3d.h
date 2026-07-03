#ifndef HIFU3D_H_
#define HIFU3D_H_

#include <QGraphicsScene>

#include <QLoggingCategory>

#include "constant.h"
#include "hifu3d_global.h"

class Mesh;
class DrawScene;

class HIFU3D_EXPORT Hifu3d : public QGraphicsScene
{
    Q_OBJECT
public :

    Hifu3d(QObject *parent=0);
    ~Hifu3d();

    // 宽和高一定要取偶数
    bool LoadImages(const QString& path,int focusX,int focusY,int wide, int high,int num);
    bool LoadImages(const QMap<int,DrawScene*>& scenes,int focusX,int focusY,int wide, int high,int num);

    // 放射状图像重建成平形状图像
    bool RadiToParallelByLine();

    void setMask(uchar* data);

    Mesh* getMesh(uchar* mask);

    bool RadiToParallelByCublic();

    uchar* GetImage(int index);

private :

    inline float GetRadius(float x, float y);
    inline float GetTheta(float x, float y);

    inline float sign(float x);
    inline uchar GetPixelData(int high,int radius,int angle);
    inline uchar GetMaskData(int high,int radius, int angle);
    int GetAboveIndex(float value, float* data, int size);
    int GetBelowIndex(float value, float* data, int size);

    int GetAboveSquareIndex(int index, int size);
    int GetBelowSquareIndex(int index);

    QImage ToGray(QImage image);

    // 计算图像插值系数
    void CalcInterpCoeff();

    float SFun(float f);

    int*  _RAbove;
    int*  _RBelow;
    int*  _ThetaAbove;
    int*  _ThetaBelow;

    int* _RAboveSquare;
    int* _RBelowSquare;
    int* _ThetaAboveSquare;
    int* _ThetaBelowSquare;

    float* _RCublicCoeff;      // 一个像素点的四个系数连续排列
    float* _ThetaCublicCoeff;  // 一个像素点的四个系数连续排列

    float* _RSeries;
    float* _ThetaSeries;
    float* _InterpRAbove;
    float* _InterpRBelow;
    float* _InterpThetaAbove;
    float* _InterpThetaBelow;

    uchar* _pMask;        // 放射状mask数据
    uchar* _pMaskImages;  // 平行状mask数据
    uchar* _pData;        // 放射状图像数据
    uchar* _pImages;
    int*   _pImagesCublic;
    int _wide;     // 自然焦点附近截取图像的宽度
    int _high;     // 自然焦点附近街区图像的高度
    int _num;
    int _focusX;   // 自然焦点在图像上的X坐标
    int _focusY;   // 自然焦点在图像上的Y坐标

};

#endif // HIFU3D_H_
