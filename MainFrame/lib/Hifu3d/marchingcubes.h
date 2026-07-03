
// marchingcubes 提取模型表面算法头文件 王文波 20161228

#ifndef MARCHINGCUBESNEW
#define MARCHINGCUBESNEW

#include "hifu3d_global.h"
#include "MCHasTable.h"
#include "mesh.h"
#include "region_growing_data_struct.h"

static char VULF = 1 << 0;
static char VULB = 1 << 1;
static char VLLB = 1 << 2;
static char VLLF = 1 << 3;
static char VURF = 1 << 4;
static char VURB = 1 << 5;
static char VLRB = 1 << 6;
static char VLRF = 1 << 7;

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
};

 static char PointIndexToFlag[8] =
 {
    VULF,
    VULB,
    VLLB,
    VLLF,
    VURF,
    VURB,
    VLRB,
    VLRF
 };

class HIFU3D_EXPORT SMCProcessor
{
public:

    struct OriginalTriangle
    {
        OriginalTriangle()
        {
            P0.x = 0;
            P0.y = 0;
            P0.z = 0;
            P1.x = 0;
            P1.y = 0;
            P1.z = 0;
            P2.x = 0;
            P2.y = 0;
            P2.z = 0;
        }

        OriginalTriangle(int p0x, int p0y, int p0z, int p1x, int p1y, int p1z, int p2x, int p2y, int p2z)
        {
            P0.x = p0x;
            P0.y = p0y;
            P0.z = p0z;
            P1.x = p1x;
            P1.y = p1y;
            P1.z = p1z;
            P2.x = p2x;
            P2.y = p2y;
            P2.z = p2z;
        }

        Int16Triple P0;
        Int16Triple P1;
        Int16Triple P2;
    };

    SMCProcessor(uchar* pData, int wide, int hight, int depth);
    ~SMCProcessor();
    Mesh GenerateSurface();
    Mesh* GetSurface();
    unsigned char GetConfig(Int16Triple* temp, int indexInWidth, int indexInHeight, int indexInDepth);
    int ExtractTriangles(Int16Triple* temp, unsigned char value, int indexInWidth, int indexInHeight, int indexInDepth, OriginalTriangle* result);
    void MergeTriangleIntoMesh(Mesh& mesh, SMCTriangleNetHashTable& hashMap, OriginalTriangle& ot);

private:

    int d;
    int h;
    int w;
    int wh;
    uchar* data;

};


#endif // MARCHINGCUBESNEW

