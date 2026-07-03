#ifndef REGION_GROWING
#define REGION_GROWING

#include "region_growing_data_struct.h"
#include "hifu3d_global.h"
// 区段法区域增长头文件
#include <QQueue>

enum ParentDirections
{
    Y0 = 1,
    Y2 = 2,
    Z0=3,
    Z2=4,
    Non = 5
};

enum ExtendTypes
{
    LeftRequired = 1,
    RightRequired = 2,
    AllRez = 3,
    UnRez = 4
};

struct Span
{
    int XLeft;
    int XRight;
    int Y;
    int Z;
    ExtendTypes Extended;
    ParentDirections ParentDirection;
};

class HIFU3D_EXPORT SpanFill3d
{
public:
    SpanFill3d(uchar* data,int width,int height,int depth, uchar up,uchar low);
    ~SpanFill3d();

    uchar* GetResult();

    void ExcuteSpanFill(Int16Triple seed);

protected:

    BitMap3d bmp;
    FlagMap3d flagsMap;

    QQueue<Span> container;//以Span为单位的Queue或Stack容器

    void CheckRange(int xleft, int xright, int y, int z,ParentDirections ptype);

    int FindXRight(int x, int y,int z);

    int FindXLeft(int x, int y,int z);

private:
    uchar min;
    uchar max;
    uchar* pData;

protected:

    bool IncludePredicate(int x, int y, int z);

    void Process(Int16Triple p);
};

#endif // REGION_GROWING

