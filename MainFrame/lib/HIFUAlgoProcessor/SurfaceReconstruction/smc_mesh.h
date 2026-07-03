#ifndef SMC_MESH
#define SMC_MESH

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iosfwd>
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>

using namespace std;
namespace mymesh {
enum FaceState
{
    STATE_UNKNOWN =1,
    STATE_INSIDE =2,
    STATE_OUTSIDE =3,
    STATE_SAME=4,
    STATE_OPPOSITE =5,

    STATE_UP=6,
    STATE_DOWN =7,
    STATE_ON =8,
    STATE_NONE =9,
};

typedef pair<int, int> edge;

class OrderEdge
{
public:
    bool operator()(const edge& e1, const edge& e2) const
    {
        if (e1.first < e2.first)
        {
            return true;
        }
        else if (e1.first > e2.first)
        {
            return false;
        }
        else if (e1.second < e2.second)
        {
            return true;
        }
        else
            return false;
    }
};

typedef set<edge, OrderEdge> edgeset;

struct Point3d
{
    float X;
    float Y;
    float Z;
    Point3d(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
    Point3d()
    {
        X = 0;
        Y = 0;
        Z = 0;
    }

    double Magnitude() const
    {
        return sqrt(X*X + Y*Y + Z*Z);
    }

    double MagnitudeSquared() const
    {
        return	X*X + Y*Y + Z*Z;
    }

    void Normalize()
    {
        double mag = Magnitude();
        if (mag == 0.0)
            return;
        double invMag = 1.0 / mag;
        X *= invMag;
        Y *= invMag;
        Z *= invMag;
    }

    Point3d VectorCross(const Point3d &b)
    {
        Point3d res = Point3d(Y*b.Z - Z*b.Y, Z*b.X - X*b.Z, X*b.Y - Y*b.X);
        //res.Normalize();
        return res;
    }

    bool IsVectorZero()
    {
        return ((fabs(X) < 1e-8f) && (fabs(Y) < 1e-8f) && (fabs(Z) < 1e-8f));
    }

    double VectorDot(const Point3d& b)
    {
        return X*b.X + Y*b.Y + Z*b.Z;
    }

    Point3d operator +(const Point3d& p) const
    {
        return Point3d(X + p.X, Y + p.Y, Z + p.Z);
    }

    Point3d operator -(const Point3d& p) const
    {
        return Point3d(X - p.X, Y - p.Y, Z - p.Z);
    }

    Point3d operator += (const Point3d& p)
    {
        X += p.X;
        Y += p.Y;
        Z += p.Z;
        return Point3d(X, Y, Z);
    }

    Point3d operator /=(float factor)
    {
        X /= factor;
        Y /= factor;
        Z /= factor;
        return Point3d(X, Y, Z);
    }

    Point3d operator *(const Point3d& p)    // 叉乘
    {
        return Point3d(X*p.X, Y*p.Y, Z*p.Z);
    }

    double MultiToDouble(const Point3d& p1) const
    {
        return p1.X*X + p1.Y*Y + p1.Z*Z;
    }

    Point3d operator *(float factor)
    {
        return Point3d(X*factor, Y*factor, Z*factor);
    }

    Point3d operator *=(float factor)
    {
        X *= factor;
        Y *= factor;
        Z *= factor;

        return Point3d(X, Y, Z);
    }

    Point3d operator /(float factor)
    {
        return Point3d(X / factor, Y / factor, Z / factor);
    }

    inline double & operator[](int index)
    {
        return reinterpret_cast<double*>(this)[index];
    }
    inline const double & operator[](int index) const
    {
        return reinterpret_cast<const double*>(this)[index];
    }
};

struct Triangle
{
    int P0Index;
    int P1Index;
    int P2Index;
    Triangle(int p0index, int p1index, int p2index)
    {
        P0Index = p0index;
        P1Index = p1index;
        P2Index = p2index;
    }

    int& operator[](int i)
    {
        return (i == 0) ? P0Index : (i == 1) ? P1Index : P2Index;
    }
};

/// <summary>
/// Struct PointAttachmentInfo
/// </summary>
struct PointAttachmentInfo
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="PointAttachmentInfo" /> struct.
    /// </summary>
    PointAttachmentInfo()
    {
        VertexAdjacencyList = NULL;
        TriangleAdjacencyList = NULL;
    }
    /// <summary>
    /// Finalizes an instance of the <see cref="PointAttachmentInfo" /> class.
    /// </summary>
    ~PointAttachmentInfo()
    {
        ClearVertexAdj();
        ClearTriangleAdj();
    }
    /// <summary>
    /// Clears the vertex adj.
    /// </summary>
    void ClearVertexAdj()
    {
        if (VertexAdjacencyList != NULL)
        {
            delete VertexAdjacencyList;
            VertexAdjacencyList = NULL;
        }
    }
    /// <summary>
    /// Clears the triangle adj.
    /// </summary>
    void ClearTriangleAdj()
    {
        if (TriangleAdjacencyList != NULL)
        {
            delete TriangleAdjacencyList;
            VertexAdjacencyList = NULL;
        }
    }
    /// <summary>
    /// The vertex adjacency list
    /// </summary>
    std::vector<long>* VertexAdjacencyList;
    /// <summary>
    /// The triangle adjacency list
    /// </summary>
    std::vector<long>* TriangleAdjacencyList;
};

struct Range
{
    unsigned int s;//开始位置
    unsigned int e;//结束位置
};

class Mesh
{
public:
    vector<Point3d> Vertices;
    vector<Triangle> Faces;
    vector<Point3d> m_vTriNormal;
    vector<Point3d> m_vAvgPosPerVer;
    vector<PointAttachmentInfo> AdjInfos;

    vector<Point3d> m_vMeshNormal;

    vector<unsigned int> m_vAdjTriPerVer;//mesh中点的邻接面集合
    vector<Range> m_vTriPerVerIndex;//点的邻接面在m_vAdjTriPerVer中的索引

    vector<unsigned int> m_vAdjVerPerVer;//mesh中点的邻接点集合
    vector<Range> m_vVerPerVerIndex;//点的邻接点在m_vAdjVerPerVer中的索引

    bool IsPerVertexVertexInfoEnabled;

    Mesh()
    {
        Vertices.clear();
        Faces.clear();
        m_vAvgPosPerVer.clear();
        IsPerVertexVertexInfoEnabled = false;
    }
    int AddVertex(Point3d toAdd)
    {
        int index = Vertices.size();
        Vertices.push_back(toAdd);
        return index;
    }
    int AddFace(Triangle tri)
    {
        int index = Faces.size();
        Faces.push_back(tri);
        return index;
    }
    void Clear()
    {
        Vertices.clear();
        Faces.clear();
    }

    //STL文件输出
    Point3d CalcTriNormal(int i0, int i1, int i2)
    {
        Point3d& p0 = Vertices[i0];
        Point3d& p1 = Vertices[i1];
        Point3d& p2 = Vertices[i2];
        Point3d normal;

        float v1x = p1.X - p0.X;
        float v1y = p1.Y - p0.Y;
        float v1z = p1.Z - p0.Z;
        float v2x = p2.X - p0.X;
        float v2y = p2.Y - p0.Y;
        float v2z = p2.X - p0.Z;
        normal.X = v1y*v2z - v1z*v2y;
        normal.Y = v1z*v2x - v1x*v2z;
        normal.Z = v1x*v2y - v1y*v2x;
        float len = (float)sqrt(normal.X*normal.X + normal.Y*normal.Y + normal.Z*normal.Z);
        if (fabs(len)<0.0000001)
        {
            return normal;
        }
        else
        {
            normal.X /= len;
            normal.Y /= len;
            normal.Z /= len;
            return normal;
        }
    }
    void CalcTrisNormal()
    {
        int iSize = static_cast<int>(Faces.size());
        m_vTriNormal.clear();
        for (int i = 0; i < iSize; ++i)
        {
            m_vTriNormal.push_back(CalcTriNormal(Faces[i].P0Index,
                                                 Faces[i].P1Index,
                                                 Faces[i].P2Index));
        }
    }
    bool WriteStl(std::string path)
    {
        if (path.empty())
        {
            return false;
        }

        FILE* f = NULL;
        f = fopen(path.c_str(), "wb");
        if (!f)
        {
            return false;
        }

        char header[80];
        memset(header, ' ', 80);
        fwrite(header, 80, 1, f);

        int nfaces = Faces.size();
        fwrite(&nfaces, 4, 1, f);

        if (m_vTriNormal.empty())
        {
            CalcTrisNormal();
        }

        for (size_t i = 0; i < Faces.size(); i++) {
            float fbuf[12];
            Point3d tn = m_vTriNormal[i];
            fbuf[0] = tn.X;
            fbuf[1] = tn.Y;
            fbuf[2] = tn.Z;
            fbuf[3] = Vertices[Faces[i].P0Index].X;
            fbuf[4] = Vertices[Faces[i].P0Index].Y;
            fbuf[5] = Vertices[Faces[i].P0Index].Z;
            fbuf[6] = Vertices[Faces[i].P1Index].X;
            fbuf[7] = Vertices[Faces[i].P1Index].Y;
            fbuf[8] = Vertices[Faces[i].P1Index].Z;
            fbuf[9] = Vertices[Faces[i].P2Index].X;
            fbuf[10] = Vertices[Faces[i].P2Index].Y;
            fbuf[11] = Vertices[Faces[i].P2Index].Z;
            /*if (need_swap) {
                for (int j = 0; j < 12; j++)
                swap_float(fbuf[j]);
                }*/
            fwrite(fbuf, 48, 1, f);
            unsigned char att[2] = { 0, 0 };
            fwrite(att, 2, 1, f);
        }

        fclose(f);

        return true;
    }

    //平滑
    void CalcAvgPosPerVer()
    {
        int iVerSize = Vertices.size();
        int iTriSize = Faces.size();
        m_vAvgPosPerVer.clear();

        vector<int> vCount;   //每个定点的被引用的数量
        for (int i = 0; i < iVerSize; i++)
        {
            vCount.push_back(0);
            m_vAvgPosPerVer.push_back(Point3d());
        }
        for (int i = 0; i < iTriSize; ++i)
        {
            //vCount.replace(Faces[i].P0Index, ++vCount[Faces[i].P0Index]);
            //vCount.replace(Faces[i].P1Index, ++vCount[Faces[i].P1Index]);
            //vCount.replace(Faces[i].P2Index, ++vCount[Faces[i].P2Index]);

            vCount.data()[Faces[i].P0Index] = ++vCount[Faces[i].P0Index];
            vCount.data()[Faces[i].P1Index] = ++vCount[Faces[i].P1Index];
            vCount.data()[Faces[i].P2Index] = ++vCount[Faces[i].P2Index];
        }

        for (int i = 0; i < iTriSize; ++i)
        {
            Triangle& tri = Faces[i];

            m_vAvgPosPerVer[tri.P0Index] = m_vAvgPosPerVer[tri.P0Index] + (Vertices[tri.P1Index] + Vertices[tri.P2Index])*0.5;
            m_vAvgPosPerVer[tri.P0Index] = m_vAvgPosPerVer[tri.P1Index] + (Vertices[tri.P0Index] + Vertices[tri.P2Index])*0.5;
            m_vAvgPosPerVer[tri.P0Index] = m_vAvgPosPerVer[tri.P2Index] + (Vertices[tri.P1Index] + Vertices[tri.P0Index])*0.5;
        }
        for (int i = 0; i < iVerSize; ++i)
        {
            if (0 == vCount[i])
            {
                continue;
            }
            m_vAvgPosPerVer[i].X = m_vAvgPosPerVer[i].X / vCount[i];
            m_vAvgPosPerVer[i].Y = m_vAvgPosPerVer[i].Y / vCount[i];
            m_vAvgPosPerVer[i].Z = m_vAvgPosPerVer[i].Z / vCount[i];
        }
    }

    void InitPerVertexVertexAdj()
    {
        if (IsPerVertexVertexInfoEnabled)
            ClearPerVertexVertexAdj();
        IsPerVertexVertexInfoEnabled = true;
        if (AdjInfos.size() != Vertices.size())
            AdjInfos.resize(Vertices.size());
        size_t vcount = Vertices.size();
        size_t fcount = Faces.size();
        for (size_t i = 0; i < vcount; i++)
        {
            std::vector<long>* vertexAdjacencyList = new (std::nothrow)std::vector<long>();
            if (vertexAdjacencyList == NULL) { return; }
            vertexAdjacencyList->reserve(6);
            AdjInfos[i].VertexAdjacencyList = vertexAdjacencyList;
        }
        for (size_t i = 0; i < fcount; i++)
        {
            Triangle &t = Faces[i];
            std::vector<long> *p0list = AdjInfos[t.P0Index].VertexAdjacencyList;
            std::vector<long> *p1list = AdjInfos[t.P1Index].VertexAdjacencyList;
            std::vector<long> *p2list = AdjInfos[t.P2Index].VertexAdjacencyList;

            if (std::find(p0list->begin(), p0list->end(), t.P1Index) == p0list->end())
                p0list->push_back(t.P1Index);
            if (std::find(p0list->begin(), p0list->end(), t.P2Index) == p0list->end())
                p0list->push_back(t.P2Index);

            if (std::find(p1list->begin(), p1list->end(), t.P0Index) == p1list->end())
                p1list->push_back(t.P0Index);
            if (std::find(p1list->begin(), p1list->end(), t.P2Index) == p1list->end())
                p1list->push_back(t.P2Index);

            if (std::find(p2list->begin(), p2list->end(), t.P0Index) == p2list->end())
                p2list->push_back(t.P0Index);
            if (std::find(p2list->begin(), p2list->end(), t.P1Index) == p2list->end())
                p2list->push_back(t.P1Index);
        }
    }

    void ClearPerVertexVertexAdj()
    {
        if (!IsPerVertexVertexInfoEnabled)
            return;
        for (size_t i = 0; i < Vertices.size(); i++)
        {
            if (AdjInfos[i].VertexAdjacencyList != NULL)
            {
                delete AdjInfos[i].VertexAdjacencyList;
                AdjInfos[i].VertexAdjacencyList = NULL;
            }
        }
        IsPerVertexVertexInfoEnabled = false;
    }

    void HCLaplacin(int iTimes, float factor1, float factor2)
    {
        InitPerVertexVertexAdj();

        vector<Point3d> point_vector;
        for (size_t i = 0; i < Vertices.size(); i++)
        {
            point_vector.push_back(Point3d());
        }
        vector<Point3d> startPoint;
        startPoint.resize(Vertices.size());
        for (size_t i = 0; i<Vertices.size(); i++)
        {
            startPoint[i].X = Vertices[i].X;
            startPoint[i].Y = Vertices[i].Y;
            startPoint[i].Z = Vertices[i].Z;
        }

        for (int c = 0; c < iTimes; c++)
        {
            for (size_t i = 0; i<Vertices.size(); i++)
            {
                float dx = 0, dy = 0, dz = 0;
                std::vector<long>& adV = *(AdjInfos[i].VertexAdjacencyList);
                for (size_t j = 0; j<adV.size(); j++)
                {
                    Point3d& t = Vertices[adV[j]];
                    dx += t.X;
                    dy += t.Y;
                    dz += t.Z;
                }
                dx = dx / adV.size();
                dy = dy / adV.size();
                dz = dz / adV.size();
                point_vector[i].X = dx - (factor1 * startPoint[i].X + (1 - factor1) *Vertices[i].X);
                point_vector[i].Y = dy - (factor1 * startPoint[i].Y + (1 - factor1) *Vertices[i].Y);
                point_vector[i].Z = dz - (factor1 * startPoint[i].Z + (1 - factor1) *Vertices[i].Z);
                startPoint[i].X = dx;
                startPoint[i].Y = dy;
                startPoint[i].Z = dz;
            }
            for (size_t i = 0; i<Vertices.size(); i++)
            {
                Vertices[i].X = point_vector[i].X;
                Vertices[i].Y = point_vector[i].Y;
                Vertices[i].Z = point_vector[i].Z;
            }
            //////////////////Step
            for (size_t i = 0; i<Vertices.size(); i++)
            {
                float dx = 0, dy = 0, dz = 0;
                std::vector<long>& adV = *(AdjInfos[i].VertexAdjacencyList);
                for (size_t j = 0; j<adV.size(); j++)
                {
                    Point3d& t = Vertices[adV[j]];
                    dx += t.X;
                    dy += t.Y;
                    dz += t.Z;
                }
                dx = (1.0f - factor2) * dx / adV.size();
                dy = (1.0f - factor2) * dy / adV.size();
                dz = (1.0f - factor2) * dz / adV.size();
                //smoothed_point.SetX( start_point.GetX() - (smooth_factor * actual_vertex->GetX() + dx) );
                point_vector[i].X = startPoint[i].X - (factor2*Vertices[i].X + dx);
                point_vector[i].Y = startPoint[i].Y - (factor2*Vertices[i].Y + dy);
                point_vector[i].Z = startPoint[i].Z - (factor2*Vertices[i].Z + dz);
            }
            ////
            for (size_t i = 0; i<Vertices.size(); i++)
            {
                Vertices[i].X = point_vector[i].X;
                Vertices[i].Y = point_vector[i].Y;
                Vertices[i].Z = point_vector[i].Z;
            }
        }
    }

    double FeatureSize()
    {
        if (Faces.empty())
            return 0.0;
        unsigned int nTri = Faces.size();
        int iTemp = (nTri / 2) + 1;
        int nSamp = iTemp < 333 ? iTemp : 333;
        vector<double> vSamples;
        vSamples.reserve(nSamp * 3);

        for (int i = 0; i < nSamp; ++i)
        {
            static unsigned int randq = 0;
            randq = unsigned(1664525)*randq + unsigned(1013904223);

            int ind = randq%nTri;
            Point3d & p0 = Vertices[Faces[ind].P0Index];
            Point3d & p1 = Vertices[Faces[ind].P1Index];
            Point3d & p2 = Vertices[Faces[ind].P2Index];

            vSamples.push_back(Point3d(p0 - p1).MagnitudeSquared());
            vSamples.push_back(Point3d(p1 - p2).MagnitudeSquared());
            vSamples.push_back(Point3d(p2 - p0).MagnitudeSquared());
        }
        nth_element(vSamples.begin(), vSamples.begin() + vSamples.size() / 2, vSamples.end());
        return sqrt(vSamples[vSamples.size() / 2]);
    }

    void Taubin(int iterationTime,float lambda,float mu)
    {
        InitPerVertexVertexAdj();

        Point3d* tempList=new Point3d[Vertices.size()];
        for(int c=0;c<iterationTime;c++)
        {
            for(size_t i=0;i<Vertices.size();i++)
            {
                tempList[i]=GetSmoothedVertex_Taubin_Step(i,lambda);
            }
            for(size_t i=0;i<Vertices.size();i++)
            {
                Vertices[i]=tempList[i];
            }
            for(size_t i=0;i<Vertices.size();i++)
            {
                tempList[i]=GetSmoothedVertex_Taubin_Step(i,mu);
            }
            for(size_t i=0;i<Vertices.size();i++)
            {
                Vertices[i]=tempList[i];
            }
        }

        delete[] tempList;
    }

    Point3d GetSmoothedVertex_Taubin_Step(size_t index,float lambda)
    {
        float dx=0,dy=0,dz=0;
        std::vector<long>& adjVertices=*(AdjInfos[index].VertexAdjacencyList);
        Point3d& p=Vertices[index];
        if(adjVertices.size()==0)
            return Vertices[index];
        for(size_t i=0;i<adjVertices.size();i++)
        {
            Point3d& t=Vertices[adjVertices[i]];
            dx+=(t.X-p.X);
            dy+=(t.Y-p.Y);
            dz+=(t.Z-p.Z);
        }
        dx/=adjVertices.size();
        dy/=adjVertices.size();
        dz/=adjVertices.size();
        float newx=lambda*dx+p.X;
        float newy=lambda*dy+p.Y;
        float newz=lambda*dz+p.Z;
        return Point3d(newx,newy,newz);
    }

    //计算点的邻接三角形
    void CalcAdjTriPerVer()
    {
        m_vAdjTriPerVer.clear();
        m_vTriPerVerIndex.clear();

        unsigned int nv = Vertices.size();
        unsigned int nf = Faces.size();

        for (size_t i = 0; i < nf * 3; i++)
        {
            m_vAdjTriPerVer.push_back(0);
        }
        for (size_t i = 0; i < nv; i++)
        {
            m_vTriPerVerIndex.push_back(Range());
        }

        vector<int> vNAjacentFaces(nv, 0);
        for (size_t i = 0; i < nv; i++)
        {
            vNAjacentFaces.data()[Faces[i].P0Index]++;
            vNAjacentFaces.data()[Faces[i].P1Index]++;
            vNAjacentFaces.data()[Faces[i].P2Index]++;
        }

        unsigned int buffer = 0;
        for (int i = 0; i < nv; i++)
        {
            m_vTriPerVerIndex[i].s = buffer;
            buffer += vNAjacentFaces.data()[i];
            m_vTriPerVerIndex[i].e = buffer;
        }

        vector<int> tempIndex(nv, 0);
        for (size_t i = 0; i < nf; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                unsigned int verIndex;
                switch (j)
                {
                case 0:
                    verIndex = Faces[i].P0Index;
                    break;
                case 1:
                    verIndex = Faces[i].P1Index;
                    break;
                case 2:
                    verIndex = Faces[i].P2Index;
                    break;
                default:
                    break;
                }
                m_vAdjTriPerVer[m_vTriPerVerIndex[verIndex].s + tempIndex[verIndex]] = i;
                tempIndex[verIndex]++;
            }
        }
    }
    //计算点的邻接点
    void CalcAdjVerPerVer()
    {
        unsigned int nv = Vertices.size();
        unsigned int nf = Faces.size();
        m_vAdjVerPerVer.clear();
        m_vVerPerVerIndex.clear();
        for (size_t i = 0; i < nv * 3; i++)
        {
            m_vAdjVerPerVer.push_back(0);
        }
        for (size_t i = 0; i < nv; i++)
        {
            m_vVerPerVerIndex.push_back(Range());
        }

        if (m_vAdjTriPerVer.empty())
            CalcAdjTriPerVer();

        vector<bool> vState(nv, false);
        vector<int> nearVers;
        nearVers.reserve(10);
        int bufferSize = 0;
        for (size_t i = 0; i < nv; i++)
        {
            for (unsigned int j = m_vTriPerVerIndex[i].s; j < m_vTriPerVerIndex[i].e; j++)
            {
                int i0 = Faces[m_vAdjTriPerVer[j]].P0Index;
                int i1 = Faces[m_vAdjTriPerVer[j]].P1Index;
                int i2 = Faces[m_vAdjTriPerVer[j]].P2Index;
                if (i0 == i)
                {
                    if (!vState[i1])
                    {
                        vState[i1] = true;
                        nearVers.push_back(i1);
                    }
                    if (!vState[i2])
                    {
                        vState[i2] = true;
                        nearVers.push_back(i2);
                    }
                }
                else if (i1 == i)
                {
                    if (!vState[i2])
                    {
                        vState[i2] = true;
                        nearVers.push_back(i2);
                    }
                    if (!vState[i0])
                    {
                        vState[i0] = true;
                        nearVers.push_back(i0);
                    }
                }
                else if (i2 == i)
                {
                    if (!vState[i0])
                    {
                        vState[i0] = true;
                        nearVers.push_back(i0);
                    }
                    if (!vState[i1])
                    {
                        vState[i1] = true;
                        nearVers.push_back(i1);
                    }
                }
            }

            m_vVerPerVerIndex[i].s = m_vAdjVerPerVer.size();
            vector<int>::iterator iter = nearVers.begin();
            for (;iter != nearVers.end(); ++iter)
            {
                m_vAdjVerPerVer.push_back(*iter);
                vState[*iter] = false;
            }
            m_vVerPerVerIndex[i].e = m_vAdjVerPerVer.size();
            nearVers.clear();
        }
    }

    bool CalNeighborTriPerEdge(int triIndex, int edgeIndex, int & neighborTri, int & neighborEdge)
    {
        neighborTri = -1;
        neighborEdge = 0;
        edge Edges;
        if (edgeIndex == 1)
        {
            Edges.first = Faces[triIndex].P0Index;
            Edges.second = Faces[triIndex].P1Index;
        }
        else if (edgeIndex == 1)
        {
            Edges.first = Faces[triIndex].P1Index;
            Edges.second = Faces[triIndex].P2Index;
        }
        else
        {
            Edges.first = Faces[triIndex].P2Index;
            Edges.second = Faces[triIndex].P0Index;
        }
        int size0 = m_vTriPerVerIndex[Edges.first].e - m_vTriPerVerIndex[Edges.first].s;
        int size1 = m_vTriPerVerIndex[Edges.second].e - m_vTriPerVerIndex[Edges.second].s;

        unsigned int s, e;
        if (size0 < size1)
        {
            s = m_vTriPerVerIndex[Edges.first].s;
            e = m_vTriPerVerIndex[Edges.first].e;
        }
        else
        {
            s = m_vTriPerVerIndex[Edges.second].s;
            e = m_vTriPerVerIndex[Edges.second].e;
        }
        for (size_t i = s; i < e; i++)
        {
            unsigned int iTri = m_vAdjTriPerVer[i];
            if (iTri == triIndex)
                continue;

            int & i0 = Faces[iTri].P0Index;
            int & i1 = Faces[iTri].P1Index;
            int & i2 = Faces[iTri].P2Index;

            if ((i0 == Edges.first&&i1 == Edges.second) || (i0 = Edges.second&&i1 == Edges.first))
            {
                neighborTri = iTri;
                neighborEdge = 1;
                return true;
            }
            else if ((i1 == Edges.first&&i2 == Edges.second) || (i1 = Edges.second&&i2 == Edges.first))
            {
                neighborTri = iTri;
                neighborEdge = 2;
                return true;
            }
            else if ((i2 == Edges.first&&i0 == Edges.second) || (i2 = Edges.second&&i0 == Edges.first))
            {
                neighborTri = iTri;
                neighborEdge = 3;
                return true;
            }
        }
        return false;
    }

};
}

#endif // SMC_MESH

