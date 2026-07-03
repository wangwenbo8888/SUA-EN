#include "smc_adsmcprocessor.h"

//*****************************CLASS SMCTriangleNetHashTable**********************

void SMCTriangleNetHashTable::SetDefaultValue(int index)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            maplist[index][j*width + i] = -1;
        }
    }
}

SMCTriangleNetHashTable::SMCTriangleNetHashTable(int minx, int miny, int width, int height)
{
    this->stx = minx - 1;
    this->sty = miny - 1;
    this->width = width + 2;
    this->height = height + 2;
    maplist.resize(2);
    maplist[0].resize(this->width*this->height);
    maplist[1].resize(this->width*this->height);
    SetDefaultValue(0);
    SetDefaultValue(1);
    CurrentLayerIndex = 0;
}

void SMCTriangleNetHashTable::IncreaseIndex()
{
    CurrentLayerIndex++;
    SetDefaultValue(0);
    vector<int> vTemp = maplist[0];
    maplist[0] = maplist[1];
    maplist[1] = vTemp;
}

void SMCTriangleNetHashTable::SetHashValue(int x, int y, int z, int value)
{
    int index = z - CurrentLayerIndex;
    maplist[index][(y - sty)*width + x - stx] = value;
}

int SMCTriangleNetHashTable::GetHashValue(int x, int y, int z)
{
    int index = z - CurrentLayerIndex;
    return maplist[index][(y - sty)*width + x - stx];
}

//*****************************CLASS SMC_AdSMCProcessor**********************

SMC_AdSMCProcessor::SMC_AdSMCProcessor(QObject *parent) : QObject(parent)
{

}

SMC_AdSMCProcessor::SMC_AdSMCProcessor(BitMap3d *bitmap, QObject * parent)
{
    this->bmp = bitmap;
    unitSpace.X = 1;
    unitSpace.Y = 1;
    unitSpace.Z = 1;
}

SMC_AdSMCProcessor::SMC_AdSMCProcessor(BitMap3d * bitmap, float fUnitWidth, float fUnitHeight, float fUnitDepth, QObject * parent)
{
    this->bmp = bitmap;
    unitSpace.X = fUnitWidth;
    unitSpace.Y = fUnitHeight;
    unitSpace.Z = fUnitDepth;
}

SMC_AdSMCProcessor::~SMC_AdSMCProcessor()
{

}

Mesh SMC_AdSMCProcessor::GenerateSurface(Mesh *pm)
{
    d = bmp->depth;
    h = bmp->height;
    w = bmp->width;
    wh = w * h;
    Int16Triple temp[8];
    Mesh *m;
    m = pm;
    OriginalTriangle tempTriangles[4];
    SMCTriangleNetHashTable hash(0, 0, w, h);

    for (int k = 0; k <= d - 1; k++)
    {
        for (int j = 0; j <= h - 1; j++)
        {
            for (int i = 0; i <= w - 1; i++)
            {
                uchar value = GetConfig(temp, bmp, i, j, k);
                if (value == 0 || value == 255)
                    continue;
                int tcount = ExtractTriangles(temp, value, i, j, k, tempTriangles);
                for (int tindex = 0; tindex < tcount; tindex++)
                {
                    MergeTriangleIntoMesh(m, &hash, tempTriangles[tindex]);
                }
            }
        }
        hash.IncreaseIndex();
    }
    return Mesh();
}

uchar SMC_AdSMCProcessor::GetConfig(Int16Triple temp[], BitMap3d *flagsMap, int indexInWidth, int indexInHeight, int indexInDepth)
{
    uchar value = 0;
    for (int pi = 0; pi < 8; pi++)
    {
        temp[pi].X = indexInWidth + PointIndexToPointDelta[pi].X;
        temp[pi].Y = indexInHeight + PointIndexToPointDelta[pi].Y;
        temp[pi].Z = indexInDepth + PointIndexToPointDelta[pi].Z;
        if (temp[pi].X < w && temp[pi].X >= 0
                && temp[pi].Y < h && temp[pi].Y >= 0
                && temp[pi].Z < d && temp[pi].Z >= 0
                && bmp->data[temp[pi].X + w * (temp[pi].Y) + wh * (temp[pi].Z)] >0)
        {
            value |= PointIndexToFlag[pi];
        }
    }
    return value;
}

int SMC_AdSMCProcessor::ExtractTriangles(Int16Triple temp[], uchar value, int indexInWidth, int indexInHeight, int indexInDepth, OriginalTriangle result[])
{
    int tcount = 0;
    if (TabeThin[value][0] != -1)
    {
        int index = 0;
        while (TabeThin[value][index] != -1)
        {
            Int16Triple t0 = temp[TabeThin[value][index]];
            Int16Triple t1 = temp[TabeThin[value][index + 1]];
            Int16Triple t2 = temp[TabeThin[value][index + 2]];
            result[tcount] = OriginalTriangle(t0.X, t0.Y, t0.Z, t1.X, t1.Y, t1.Z, t2.X, t2.Y, t2.Z);
            tcount++;
            index += 3;
        }
    }
    return tcount;
}

void SMC_AdSMCProcessor::MergeTriangleIntoMesh(Mesh *mesh, SMCTriangleNetHashTable *hashMap, OriginalTriangle ot)
{
    int p0x = ot.P0.X;
    int p0y = ot.P0.Y;
    int p0z = ot.P0.Z;
    int p1x = ot.P1.X;
    int p1y = ot.P1.Y;
    int p1z = ot.P1.Z;
    int p2x = ot.P2.X;
    int p2y = ot.P2.Y;
    int p2z = ot.P2.Z;
    int p0i;
    int p1i;
    int p2i;
    int index = 0;
    index = hashMap->GetHashValue(p0x, p0y, p0z);
    if (index == -1)
    {
        p0i = mesh->AddVertex(Point3d(p0x, p0y, p0z)*unitSpace);
        hashMap->SetHashValue(p0x, p0y, p0z, p0i);
    }
    else
    {
        p0i = index;
    }

    index = hashMap->GetHashValue(p1x, p1y, p1z);
    if (index == -1)
    {
        p1i = mesh->AddVertex(Point3d(p1x, p1y, p1z)*unitSpace);
        hashMap->SetHashValue(p1x, p1y, p1z, p1i);
    }
    else
    {
        p1i = index;
    }

    index = hashMap->GetHashValue(p2x, p2y, p2z);
    if (index == -1)
    {
        p2i = mesh->AddVertex(Point3d(p2x, p2y, p2z)*unitSpace);
        hashMap->SetHashValue(p2x, p2y, p2z, p2i);
    }
    else
    {
        p2i = index;
    }

    Triangle t(p0i, p1i, p2i);
    mesh->AddFace(t);
}

