

// wangwenbo create 20150405

#include "MCHasTable.h"
#include "MarchingCubes.h"
#include "MCTablethin.h"
#include "qdebug.h"

SMCProcessor::SMCProcessor(uchar* pData, int wide, int hight, int depth)
{
    data = pData;

    d = depth;
    h = hight;
    w = wide;
    wh = wide*hight;
}

SMCProcessor::~SMCProcessor()
{
//    if (data!=NULL)
//    {
//        delete []data;
//        data = NULL;
//    }
}

Mesh* SMCProcessor::GetSurface()
{
    Int16Triple temp[8];
    Mesh* pMesh = new Mesh;
    OriginalTriangle tempTriangles[4];
    SMCTriangleNetHashTable hash(0, 0, w, h);

    for (int k = 0; k < d; k++)
    {
        for (int j = 0; j < h ; j++)
        {
            for (int i = 0; i < w; i++)
            {
                unsigned char value = GetConfig(temp, i, j, k);
                if (value == 0 || value == 255)
                    continue;
                int tcount = ExtractTriangles(temp, value, i, j, k, tempTriangles);
                for (int tindex = 0; tindex < tcount; tindex++)
                {
                    MergeTriangleIntoMesh(*pMesh, hash, tempTriangles[tindex]);
                }

            }
        }
        hash.IncreaseIndex();
    }

    qDebug()<<"get mesh success !";
    return pMesh;
}

Mesh SMCProcessor::GenerateSurface()
{
    Int16Triple temp[8];
    Mesh m;
    OriginalTriangle tempTriangles[4];
    SMCTriangleNetHashTable hash(0, 0, w, h);

    for (int k = 0; k < d; k++)
    {
        for (int j = 0; j < h ; j++)
        {
            for (int i = 0; i < w; i++)
            {
                unsigned char value = GetConfig(temp, i, j, k);
                if (value == 0 || value == 255)
                    continue;
                int tcount = ExtractTriangles(temp, value, i, j, k, tempTriangles);
                for (int tindex = 0; tindex < tcount; tindex++)
                {
                    MergeTriangleIntoMesh(m, hash, tempTriangles[tindex]);
                }

            }
        }
        hash.IncreaseIndex();
    }

    return m;
}

unsigned char SMCProcessor::GetConfig(Int16Triple* temp, int indexInWidth, int indexInHeight, int indexInDepth)
{
    unsigned char value = 0;
    for (int pi = 0; pi < 8; pi++)
    {
        temp[pi].x = indexInWidth + PointIndexToPointDelta[pi].x;
        temp[pi].y = indexInHeight + PointIndexToPointDelta[pi].y;
        temp[pi].z = indexInDepth + PointIndexToPointDelta[pi].z;
        if (temp[pi].x < w && temp[pi].x >= 0
            && temp[pi].y < h && temp[pi].y >= 0
            && temp[pi].z < d && temp[pi].z >= 0
            && data[temp[pi].x + w * (temp[pi].y) + wh * (temp[pi].z)] >50)
        {
            value |= PointIndexToFlag[pi];
        }
    }

    return value;
}

int SMCProcessor::ExtractTriangles(Int16Triple* temp, unsigned char value, int indexInWidth, int indexInHeight, int indexInDepth, OriginalTriangle* result)
    {
        int tcount = 0;
        if (TableThin[value][0] != -1)
        {
            int index = 0;
            while (TableThin[value][index] != -1)
            {
                Int16Triple t0 = temp[TableThin[value][index]];
                Int16Triple t1 = temp[TableThin[value][index + 1]];
                Int16Triple t2 = temp[TableThin[value][index + 2]];
                result[tcount] = OriginalTriangle(t0.x, t0.y, t0.z, t1.x, t1.y, t1.z, t2.x, t2.y, t2.z);
                tcount++;
                index += 3;
            }
        }
        return tcount;
    }

void SMCProcessor::MergeTriangleIntoMesh(Mesh& mesh, SMCTriangleNetHashTable& hashMap, OriginalTriangle& ot)
{

    int p0x = ot.P0.x;
    int p0y = ot.P0.y;
    int p0z = ot.P0.z;
    int p1x = ot.P1.x;
    int p1y = ot.P1.y;
    int p1z = ot.P1.z;
    int p2x = ot.P2.x;
    int p2y = ot.P2.y;
    int p2z = ot.P2.z;

    int p0i;
    int p1i;
    int p2i;
    int index = 0;
    index = hashMap.GetHashValue(p0x, p0y, p0z);
    if (index == -1)
    {
        p0i = mesh.AddVertex(point3d(p0x, p0y, p0z));
        hashMap.SetHashValue(p0x, p0y, p0z, p0i);
    }
    else
    {
        p0i = index;
    }

    index = hashMap.GetHashValue(p1x, p1y, p1z);
    if (index == -1)
    {
        p1i = mesh.AddVertex(point3d(p1x, p1y, p1z));
        hashMap.SetHashValue(p1x, p1y, p1z, p1i);
    }
    else
    {
        p1i = index;
    }

    index = hashMap.GetHashValue(p2x, p2y, p2z);
    if (index == -1)
    {
        p2i = mesh.AddVertex(point3d(p2x, p2y, p2z));
        hashMap.SetHashValue(p2x, p2y, p2z, p2i);
    }
    else
    {
        p2i = index;
    }

     mesh.AddFace(Triangle(p0i, p1i, p2i));
}
