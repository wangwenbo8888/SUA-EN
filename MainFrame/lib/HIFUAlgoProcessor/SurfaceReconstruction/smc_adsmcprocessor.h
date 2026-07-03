#ifndef SMC_ADSMCPROCESSOR_H
#define SMC_ADSMCPROCESSOR_H

#include <QObject>
#include <vector>
#include "base3d.h"
#include "smc_mesh.h"
#include "smc_table.h"
#include "smc_cube.h"

using namespace std;
using namespace mymesh;

struct OriginalTriangle
{
    Int16Triple P0= Int16Triple(0, 0, 0);
    Int16Triple P1 = Int16Triple(0, 0, 0);
    Int16Triple P2 = Int16Triple(0, 0, 0);
    OriginalTriangle(int p0x, int p0y, int p0z, int p1x, int p1y, int p1z, int p2x, int p2y, int p2z)
    {
        P0.X = p0x;
        P0.Y = p0y;
        P0.Z = p0z;
        P1.X = p1x;
        P1.Y = p1y;
        P1.Z = p1z;
        P2.X = p2x;
        P2.Y = p2y;
        P2.Z = p2z;
    }
    OriginalTriangle()
    {
    }
};

class SMCTriangleNetHashTable
{
public:

    SMCTriangleNetHashTable(int minx, int miny, int width, int height);

    void SetDefaultValue(int index);

    void IncreaseIndex();

    void SetHashValue(int x, int y, int z, int value);

    int GetHashValue(int x, int y, int z);

    int CurrentLayerIndex;
    int stx;
    int sty;
    int width;
    int height;
    vector<vector<int> > maplist;

};

class SMC_AdSMCProcessor : public QObject
{
    Q_OBJECT
public:
    explicit SMC_AdSMCProcessor(QObject *parent = 0);
    SMC_AdSMCProcessor(BitMap3d *bitmap ,QObject *parent);
    SMC_AdSMCProcessor(BitMap3d *bitmap,float fUnitWidth,float fUnitHeight,float fUnitDepth, QObject *parent);
    ~SMC_AdSMCProcessor();

    Mesh GenerateSurface(Mesh *pm);

signals:

public slots:

private:
    BitMap3d *bmp;
    int d;
    int h;
    int w;
    int wh;
    Point3d unitSpace;

    uchar GetConfig(Int16Triple temp[], BitMap3d *flagsMap, int indexInWidth, int indexInHeight, int indexInDepth);
    int ExtractTriangles(Int16Triple temp[], uchar value, int indexInWidth, int indexInHeight, int indexInDepth, OriginalTriangle result[]);
    void MergeTriangleIntoMesh(Mesh *mesh, SMCTriangleNetHashTable *hashMap, OriginalTriangle ot);
};

#endif // SMC_ADSMCPROCESSOR_H
