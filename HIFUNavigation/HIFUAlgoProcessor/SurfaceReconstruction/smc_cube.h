#ifndef SMC_CUBE
#define SMC_CUBE

#include <QObject>
#include "base3d.h"

static uchar VULF = 1 << 0;
static uchar VULB = 1 << 1;
static uchar VLLB = 1 << 2;
static uchar VLLF = 1 << 3;
static uchar VURF = 1 << 4;
static uchar VURB = 1 << 5;
static uchar VLRB = 1 << 6;
static uchar VLRF = 1 << 7;
//以上为体素为实点的位标记
static Int16Triple PointIndexToPointDelta[8] =
{
    Int16Triple(0, 1, 1),
    Int16Triple(0, 1, 0),
    Int16Triple(0, 0, 0),
    Int16Triple(0, 0, 1),
    Int16Triple(1, 1, 1),
    Int16Triple(1, 1, 0),
    Int16Triple(1, 0, 0),
    Int16Triple(1, 0, 1)
};//体元内每个体素相对基准体素坐标的偏移
static uchar PointIndexToFlag[8] =
{
    VULF,
    VULB,
    VLLB,
    VLLF,
    VURF,
    VURB,
    VLRB,
    VLRF
};//每个体素对应的位标记
static int EdgeIndexToEdgeVertexIndex[12][2] =
{
    { 0,1 },{ 1,2 },
    { 2,3 },{ 3,0 },
    { 4,5 },{ 5,6 },
    { 6,7 },{ 7,4 },
    { 0,4 },{ 1,5 },
    { 2,6 },{ 3,7 }
};//每个边对应的两顶点体素的索引

class Cube
{
public:
    int CellIndexX;
    int CellIndexY;
    int CellIndexZ;

    Int16Triple cubeImageIndices[8]{
        Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0),
                Int16Triple(0, 0, 0)
    };//用于存储8个体素的坐标

    Cube(int cellIndexX, int cellIndexY, int cellIndexZ)
    {
        CellIndexX = cellIndexX;
        CellIndexY = cellIndexY;
        CellIndexZ = cellIndexZ;
        for (int i = 0; i < 8; i++)
        {
            cubeImageIndices[i].X = cellIndexX + PointIndexToPointDelta[i].X;
            cubeImageIndices[i].Y = cellIndexY + PointIndexToPointDelta[i].Y;
            cubeImageIndices[i].Z = cellIndexZ + PointIndexToPointDelta[i].Z;
        }
    }//使用基准体素坐标初始化Cube
};

#endif // SMC_CUBE

