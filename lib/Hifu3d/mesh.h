
#ifndef MESH_H_
#define MESH_H_

// create by wangwenbo 20150404

#include <vector>
#include <string>
#include "vector3d.h"

#include <set>
#include "hifu3d_global.h"

using namespace std;

typedef vector3d point3d;

vector3d trinorm(const point3d& p0, const point3d& p1, const point3d& p2);
#if 0
struct point3d
{
    point3d(float fx, float fy,float fz)
    {
        x = fx;
        y = fy;
        z = fz;
    }

    point3d()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    ~point3d()
    {}

    point3d operator +(const point3d& p)
    {
        return point3d(x+p.x,y+p.y,z+p.z);
    }

    point3d operator -(const point3d& p)
    {
        return point3d(x-p.x,y-p.y,z-p.z);
    }

    void operator +=(const point3d& p)
    {
        x = x + p.x;
        y = y + p.y;
        z = z + p.z;
    }

    void operator -=(const point3d& p)
    {
        x = x - p.x;
        y = y - p.y;
        z = z - p.z;
    }

    float x;
    float y;
    float z;

};
#endif

struct Triangle
{
    Triangle(int i0, int i1, int i2)
    {
        p0 = i0;
        p1 = i1;
        p2 = i2;
    }

    Triangle()
    {
        p0 = 0;
        p1 = 0;
        p2 = 0;
    }

    ~Triangle()
    {
    }

    bool hasVertex(int index)
    {
        return (p0 == index || p1 == index || p2 == index);
    }

    void changeVertex(int from,int to)
    {
        if (from == p0)
        {
            p0 = to;
        }
        else if (from == p1)
        {
            p1 = to;
        }
        else if (from == p2)
        {
            p2 = to;
        }
        else
        {
        }
    }

    int operator[](int i) const
    {
        if (0==i)
        {
            return p0;
        }
        else if (1==i)
        {
            return p1;
        }
        else
        {
            return p2;
        }

    }

    int indexof(int v_) const
    {
        return (p0 == v_) ? 0 :
            (p1 == v_) ? 1 :
            (p2 == v_) ? 2 : -1;
    }

    int p0;
    int p1;
    int p2;
};

class HIFU3D_EXPORT Mesh
{
public:

    Mesh();
    ~Mesh();

    bool ReadStl(const string& sFile);
    bool WriteStl(const string& sFile);

    void MoveCenter();

    int AddVertex(const point3d& p);
    int AddFace(const Triangle& tri);

    bool CalcAdjVerPerVer();
    bool CalcAdjTriPerVer();

    void InitPerVertexVertexAdj();
    void InitPerVertexTriangleAdj();
    void ClearPerVertexVertexAdj();
    void ClearPerVertexTriangleAdj();
    bool GetIsPerVertexVertexInfoEnabled();
    bool GetIsPerVertexTriangleInfoEnabled();

    bool CalcVerNomal();

    bool CalcSingleVerNormal(int index);


    void ReverseTris();

    float CalcTriArea(int index);

    float max_x(int i);
    void NormalCorrect();

    void Normalize();

    void RemoveUnavilable(const QVector<bool>& vFlags);

    void CalcTrisNormal();
    void CalcTriNormal(int index);

    point3d CalcTriNormal(int i0,int i1,int i2);

    int PointIndexInTri(int iTri,const point3d& p);

    std::vector<point3d> m_vVertices;
    std::vector<vector3d> m_vVerNormal;
    std::vector<Triangle> m_vTriangles;
    std::vector<point3d> m_vTriNormal;
    std::vector<set<int> > m_vAdjVerPerVer;
    std::vector<set<int> > m_vAdjTriPerVer;

    std::vector<std::vector<long> > AdjInfos;

    bool IsPerVertexVertexInfoEnabled;
    bool IsPerVertexTriangleInfoEnabled;

};

#endif // !MESH_H_


