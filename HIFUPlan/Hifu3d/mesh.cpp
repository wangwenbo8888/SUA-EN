
// wangwenbo create 20150404

#include "Mesh.h"

#include <map>
#include <QString>
#include <algorithm>
#include "math.h"
#include "float.h"
#include "qdebug.h"

#define COND_READ(cond, where, len) do { if ((cond) && !fread((void *)&(where), (len), 1, f)) return false; } while (0)
#define FWRITE(ptr, size, nmemb, stream) do { if (fwrite((ptr), (size), (nmemb), (stream)) != (nmemb)) return false; } while (0)

static bool we_are_little_endian()
{
    // The following appears to be legal according to
    // C99 strict-aliasing rules
    int tmp = 1;
    return !!(*(unsigned char *)&tmp);
}

static inline void swap_32(unsigned char *p)
{
    *(unsigned *)p = _byteswap_ulong(*(unsigned *)p);
}

static inline void swap_float(float &x)
{
    swap_32((unsigned char *)&x);
}

static inline void swap_int(int &x)
{
    swap_32((unsigned char *)&x);
}

Mesh::Mesh()
{
    IsPerVertexVertexInfoEnabled = false;
    IsPerVertexTriangleInfoEnabled = false;
    m_vVertices.clear();
    m_vTriangles.clear();
}

Mesh::~Mesh()
{
}

void Mesh::MoveCenter()
{
    size_t versSize = m_vVertices.size();
    float maxX = -10000.0f;
    float maxY = -10000.0f;
    float maxZ = -10000.0f;
    float miniX = 10000.0f;
    float miniY = 10000.0f;
    float miniZ = 10000.0f;

    for (size_t i = 0; i < versSize;++i)
    {
        vector3d& coor = m_vVertices.data()[i];
        if (coor.x > maxX)
        {
            maxX = coor.x;
        }

        if (coor.x < miniX)
        {
           miniX = coor.x;
        }

        if (coor.y > maxY)
        {
            maxY = coor.y;
        }

        if (coor.y < miniY)
        {
           miniY = coor.y;
        }

        if (coor.z > maxZ)
        {
            maxZ = coor.x;
        }

        if (coor.z < miniZ)
        {
           miniZ = coor.x;
        }
    }

    vector3d center(0.5*(maxX+miniX),0.5*(maxY+miniY),0.5*(maxZ+miniZ));
    vector3d size(maxX-miniX,maxY-miniY,maxZ-miniZ);
    float maxSize = size.x > size.y ? size.x:size.y;
    maxSize = maxSize > size.z ? maxSize : size.z;
    maxSize = 40.0/maxSize;

    for (size_t i = 0; i < versSize;++i)
    {
        vector3d& coor = m_vVertices.data()[i];
        coor.x -= center.x;
        coor.x *= maxSize;
        coor.y -= center.y;
        coor.y *= maxSize;
        coor.z -= center.z;
        coor.z *= maxSize;
    }
}

bool Mesh::ReadStl(const string& sFile)
{
    FILE* f = NULL;
    f = fopen(sFile.c_str(),"rb");
    if (f==NULL)
    {
        return false;
    }

    bool need_swap = !we_are_little_endian();

    char header[80];
    COND_READ(true, header, 80);

    int nfacets;
    COND_READ(true, nfacets, 4);
    if (need_swap)
        swap_int(nfacets);

    m_vTriangles.reserve(nfacets);
    m_vVertices;

    map<QString, int> mapTemp;
    map<QString, int>::iterator iter = mapTemp.begin();
    QString sTemp;
    int pointIndex[3];
    int index = 0;
    for (int i = 0; i < nfacets; i++) {
        float fbuf[12];
        COND_READ(true, fbuf, 48);
        if (need_swap) {
            for (int j = 3; j < 12; j++)
                swap_float(fbuf[j]);
        }

        sTemp = QString::number(fbuf[3]);
        sTemp.append(QString::number(fbuf[4]));
        sTemp.append(QString::number(fbuf[5]));
        iter = mapTemp.find(sTemp);
        if (iter!= mapTemp.end())
        {
            pointIndex[0] = iter->second;
        }
        else
        {
            pointIndex[0] = index;
            mapTemp.insert(make_pair(sTemp,index));
            m_vVertices.push_back(point3d(fbuf[3],fbuf[4],fbuf[5]));
            ++index;
        }

        sTemp = QString::number(fbuf[6]);
        sTemp.append(QString::number(fbuf[7]));
        sTemp.append(QString::number(fbuf[8]));
        iter = mapTemp.find(sTemp);
        if (iter!=mapTemp.end())
        {
            pointIndex[1] = iter->second;
        }
        else
        {
            pointIndex[1] = index;
            mapTemp.insert(make_pair(sTemp, index));
            m_vVertices.push_back(point3d(fbuf[6],fbuf[7],fbuf[8]));
            ++index;
        }

        sTemp = QString::number(fbuf[9]);
        sTemp.append(QString::number(fbuf[10]));
        sTemp.append(QString::number(fbuf[11]));

        iter = mapTemp.find(sTemp);
        if (iter!=mapTemp.end())
        {
            pointIndex[2] = iter->second;
        }
        else
        {
            pointIndex[2] = index;
            mapTemp.insert(make_pair(sTemp, index));
            m_vVertices.push_back(point3d(fbuf[9],fbuf[10],fbuf[11]));
            ++index;
        }

        m_vTriangles.push_back(Triangle(pointIndex[0],pointIndex[1],pointIndex[2]));

        unsigned char att[2];
        COND_READ(true, att, 2);
    }

    fclose(f);

    return true;
}

void Mesh::CalcTrisNormal()
{
    int iTriSize = m_vTriangles.size();
    m_vTriNormal.resize(iTriSize);
    for (int i = 0; i < iTriSize;++i)
    {
        Triangle& tri = m_vTriangles.data()[i];
        m_vTriNormal.data()[i] = CalcTriNormal(tri.p0,tri.p1,tri.p2);
    }
}

void Mesh::CalcTriNormal(int index)
{
    Triangle& tri = m_vTriangles.data()[index];
    m_vTriNormal.data()[index] = CalcTriNormal(tri.p0, tri.p1, tri.p2);
}

point3d Mesh::CalcTriNormal(int i0,int i1,int i2)
{
    point3d& p0 = m_vVertices.data()[i0];
    point3d& p1 = m_vVertices.data()[i1];
    point3d& p2 = m_vVertices.data()[i2];

    point3d Normal;
    float v1x = p1.x - p0.x;
    float v1y = p1.y - p0.y;
    float v1z = p1.z - p0.z;
    float v2x = p2.x - p1.x;
    float v2y = p2.y - p1.y;
    float v2z = p2.z - p1.z;
    Normal.x = v1y * v2z - v1z * v2y;
    Normal.y = v1z * v2x - v1x * v2z;
    Normal.z = v1x * v2y - v1y * v2x;
    float len = (float)sqrt(Normal.x * Normal.x + Normal.y * Normal.y + Normal.z * Normal.z);
    if (len == 0)
    {
        return point3d();
    }
    else
    {
        Normal.x /= len;
        Normal.y /= len;
        Normal.z /= len;
        return Normal;
    }

}


bool Mesh::WriteStl(const string& sFile)
{

    FILE* f = NULL;
    f = fopen(sFile.c_str(),"wb");
    if (f == NULL)
    {
        return false;
    }

    bool need_swap = !we_are_little_endian();

    char header[80];
    memset(header, ' ', 80);
    FWRITE(header, 80, 1, f);

    int nfaces = m_vTriangles.size();
    if (need_swap)
        swap_int(nfaces);
    FWRITE(&nfaces, 4, 1, f);
    CalcTrisNormal();

    for (size_t i = 0; i < m_vTriangles.size(); i++) {
        float fbuf[12];
        point3d& tn = m_vTriNormal.data()[i];
        fbuf[0] = tn.x;
        fbuf[1] = tn.y;
        fbuf[2] = tn.z;
        fbuf[3] = m_vVertices[m_vTriangles[i].p0].x;
        fbuf[4] = m_vVertices[m_vTriangles[i].p0].y;
        fbuf[5] = m_vVertices[m_vTriangles[i].p0].z;
        fbuf[6] = m_vVertices[m_vTriangles[i].p1].x;
        fbuf[7] = m_vVertices[m_vTriangles[i].p1].y;
        fbuf[8] = m_vVertices[m_vTriangles[i].p1].z;
        fbuf[9] = m_vVertices[m_vTriangles[i].p2].x;
        fbuf[10] = m_vVertices[m_vTriangles[i].p2].y;
        fbuf[11] = m_vVertices[m_vTriangles[i].p2].z;
        if (need_swap) {
            for (int j = 0; j < 12; j++)
                swap_float(fbuf[j]);
        }
        FWRITE(fbuf, 48, 1, f);
        unsigned char att[2] = { 0, 0 };
        FWRITE(att, 2, 1, f);
    }

    fclose(f);

    return true;
}

int Mesh::AddVertex(const point3d& p)
{
    size_t i = m_vVertices.size();
    m_vVertices.push_back(p);
    return i;
}

int Mesh::AddFace(const Triangle& tri)
{
    size_t i = m_vTriangles.size();
    m_vTriangles.push_back(tri);
    return i;
}

bool Mesh::CalcAdjVerPerVer()
{
    size_t iVerSize = m_vVertices.size();
    size_t iTriSize = m_vTriangles.size();
    vector<int> vCounts;
    vCounts.resize(iVerSize,0);
    for (size_t i = 0; i < iTriSize;++i)
    {
        Triangle& tri = m_vTriangles.data()[i];
        ++vCounts.data()[tri.p0];
        ++vCounts.data()[tri.p1];
        ++vCounts.data()[tri.p2];
    }

    for (size_t i = 0; i < iVerSize; ++i)
    {
        set<int> vTemp;
        //vTemp.reserve(vCounts.data()[i] + 2);
        m_vAdjVerPerVer.push_back(vTemp);
    }

    for (size_t i = 0; i < iTriSize; i++)
    {
        Triangle &t = m_vTriangles.data()[i];
        set<int>& p0list = m_vAdjVerPerVer.data()[t.p0];
        set<int>& p1list = m_vAdjVerPerVer.data()[t.p1];
        set<int>& p2list = m_vAdjVerPerVer.data()[t.p2];

        if (find(p0list.begin(), p0list.end(), t.p1) == p0list.end())
            p0list.insert(t.p1);
         if (find(p0list.begin(), p0list.end(), t.p2) == p0list.end())
            p0list.insert(t.p2);

         if (find(p1list.begin(), p1list.end(), t.p0) == p1list.end())
            p1list.insert(t.p0);
         if (find(p1list.begin(), p1list.end(), t.p2) == p1list.end())
            p1list.insert(t.p2);

         if (find(p2list.begin(), p2list.end(), t.p0) == p2list.end())
            p2list.insert(t.p0);
         if (find(p2list.begin(), p2list.end(), t.p1) == p2list.end())
            p2list.insert(t.p1);

    }

    return true;
}


bool Mesh::CalcAdjTriPerVer()
{
    size_t iVerSize = m_vVertices.size();
    size_t iTriSize = m_vTriangles.size();

    m_vAdjTriPerVer.clear();
    m_vAdjTriPerVer.reserve(iVerSize);
    for (size_t i = 0; i < iVerSize; i++)
    {
        set<int> list;
        m_vAdjTriPerVer.push_back(list);
    }
    for (size_t i = 0; i < iTriSize; i++)
    {
        Triangle& t = m_vTriangles.data()[i];
        set<int>& t0list = m_vAdjTriPerVer.data()[t.p0];
        set<int>& t1list = m_vAdjTriPerVer.data()[t.p1];
        set<int>& t2list = m_vAdjTriPerVer.data()[t.p2];
        t0list.insert(i);
        t1list.insert(i);
        t2list.insert(i);
    }


    return true;
}

bool Mesh::CalcSingleVerNormal(int index)
{

    float sumx = 0;
    float sumy = 0;
    float sumz = 0;
    set<int>& tlist = m_vAdjTriPerVer.data()[index];

    if (tlist.size() != 0)
    {
        set<int>::iterator iter = tlist.begin();
        for (iter = tlist.begin(); iter != tlist.end(); ++iter)
        {
            sumx += m_vTriNormal.data()[*iter].x;
            sumy += m_vTriNormal.data()[*iter].y;
            sumz += m_vTriNormal.data()[*iter].z;
        }

        m_vVerNormal.data()[index] = vector3d(sumx / tlist.size(), sumy / tlist.size(), sumz / tlist.size());//���ڽ��淢����ֵ
    }
    else
    {
        m_vVerNormal.data()[index] = vector3d();
    }
    return true;
}



bool Mesh::CalcVerNomal()
{
    if (m_vTriNormal.empty())
    {
        CalcTrisNormal();
    }

    if (m_vAdjTriPerVer.empty())
    {
        CalcAdjTriPerVer();
    }

    size_t iVerSize = m_vVertices.size();
    m_vVerNormal.resize(iVerSize);
    for (size_t i = 0; i < iVerSize; i++)
    {
        float sumx = 0;
        float sumy = 0;
        float sumz = 0;
        set<int>& tlist = m_vAdjTriPerVer.data()[i];

        if (tlist.size() != 0)
        {
            set<int>::iterator iter = tlist.begin();
            for (iter = tlist.begin(); iter != tlist.end(); ++iter)
            {
                sumx += m_vTriNormal.data()[*iter].x;
                sumy += m_vTriNormal.data()[*iter].y;
                sumz += m_vTriNormal.data()[*iter].z;
            }

            m_vVerNormal.data()[i] = vector3d(sumx / tlist.size(), sumy / tlist.size(), sumz / tlist.size());//���ڽ��淢����ֵ
        }
        else
        {
            m_vVerNormal.data()[i] = vector3d();
        }
    }

    return true;
}

void Mesh::ReverseTris()
{
    size_t iTriSize = m_vTriangles.size();
    for (size_t i = 0; i < iTriSize;++i)
    {
        Triangle& tri = m_vTriangles.data()[i];
        int iTemp = tri.p2;
        tri.p2 = tri.p1;
        tri.p1 = iTemp;
    }
}

void Mesh::RemoveUnavilable(const QVector<bool>& vFlags)
{
    size_t iTriSize = m_vTriangles.size();
    int iIndex = 0;
    for (size_t i = 0; i < iTriSize;++i)
    {
        if (!vFlags[i])
        {
            m_vTriangles.data()[iIndex] = m_vTriangles.data()[i];
            ++iIndex;
        }
    }

    m_vTriangles.erase(m_vTriangles.begin() + iIndex, m_vTriangles.end());

    size_t iVerSize = m_vVertices.size();
    vector<int> vTemp;
    vector<bool> vVerFlags;
    vTemp.resize(iVerSize,-1);
    vVerFlags.resize(iVerSize,false);
    iTriSize = m_vTriangles.size();
    for (size_t i = 0; i < iTriSize;++i)
    {
        Triangle& tri = m_vTriangles.data()[i];
        vVerFlags[tri.p0] = true;
        vVerFlags[tri.p1] = true;
        vVerFlags[tri.p2] = true;
    }

    iIndex = 0;
    for (size_t i = 0; i < iVerSize;++i)
    {
        if (vVerFlags[i])
        {
            m_vVertices.data()[iIndex] = m_vVertices.data()[i];
            vTemp.data()[i] = iIndex;
            ++iIndex;
        }
    }
    m_vVertices.erase(m_vVertices.begin()+iIndex,m_vVertices.end());

    for (int i = 0; i < iTriSize;++i)
    {
        Triangle& tri = m_vTriangles.data()[i];
        tri.p0 = vTemp.data()[tri.p0];
        tri.p1 = vTemp.data()[tri.p1];
        tri.p2 = vTemp.data()[tri.p2];
    }
}

float Mesh::max_x(int i)
{
    return max(max(m_vVertices.data()[m_vTriangles.data()[i].p0].x,
        m_vVertices.data()[m_vTriangles.data()[i].p1].x),
        m_vVertices.data()[m_vTriangles.data()[i].p2].x);
}

void swap(int a,int b)
{
    int c = a;
    a = b;
    b = c;
}

vector3d trinorm(const point3d& p0, const point3d& p1, const point3d& p2)
{
    return vector3d(0.5*vector3dCross(p1 - p0, p2 - p0));
}

float Mesh::CalcTriArea(int index)
{
    Triangle& tri = m_vTriangles.data()[index];
    point3d& p0 = m_vVertices.data()[tri.p0];
    point3d& p1 = m_vVertices.data()[tri.p1];
    point3d& p2 = m_vVertices.data()[tri.p2];
    point3d v01 = p0 - p1;
    point3d v21 = p2 - p1;
    point3d cross = vector3dCross(v01,v21);

    return float(0.5*cross.Magnitude());
}


void Mesh::Normalize()
{
    point3d max;
    max[0] = max[1] = max[2] = -FLT_MAX;
    point3d min;
    min[0] = min[1] = min[2] = FLT_MAX;
    float scale;

    int iVerSize = m_vVertices.size();
    for (int i = 0; i < iVerSize;++i)
    {
        point3d& p = m_vVertices.data()[i];
        if (p.x < min.x) min.x = p.x;
        if (p.y < min.y) min.y = p.y;
        if (p.z < min.z) min.z = p.z;
        if (p.x > max.x) max.x = p.x;
        if (p.y > max.y) max.y = p.y;
        if (p.z > max.z) max.z = p.z;
    }

    point3d dimv = max - min;
    if (dimv.x >= dimv.y && dimv.x >= dimv.z) scale = 2.0f / dimv.x;
    else if (dimv.y >= dimv.x && dimv.y >= dimv.z) scale = 2.0f / dimv.y;
    else scale = 2.0f / dimv.z;

    point3d transv = min + max;

    transv *= 0.5f;

    for (int i = 0; i < iVerSize;++i)
    {
        m_vVertices.data()[i] -= transv;
        m_vVertices.data()[i] *= scale;
    }
}

void Mesh::NormalCorrect()
{

    CalcAdjTriPerVer();

    // vFlags.clear();
    QVector<int> vFlags;
    size_t iSize = m_vTriangles.size();
    const unsigned NONE = 0XFFFFFFFFFFFFFFFF;
    vFlags.resize(iSize);

    unsigned cc = 0;
    QVector<int> cc_farthest;
    for (int i = 0; i < iSize; i++) {
        if (vFlags.data()[i] != NONE)
            continue;

        vFlags[i] = cc;
        cc_farthest.push_back(i);
        float farthest_val = max_x(i);

        vector<int> q;
        q.push_back(i);
        while (!q.empty()) {
            int f = q.back();
            q.pop_back();
            for (int j = 0; j < 3; j++) {
                int v0 = m_vTriangles.data()[f][j];
                int v1 = m_vTriangles.data()[f][(j + 1) % 3];
                const set<int> &a = m_vAdjTriPerVer[v0];
                set<int>::iterator iter = a.begin();
                for (iter = a.begin(); iter != a.end(); ++iter) {
                    int f1 = *iter;
                    if (vFlags[f1] != NONE)
                        continue;
                    int i0 = m_vTriangles.data()[f1].indexof(v0);
                    int i1 = m_vTriangles.data()[f1].indexof(v1);
                    if (i0 < 0 || i1 < 0)
                        continue;
                    if (i1 == (i0 + 1) % 3)
                        swap(m_vTriangles.data()[f1][1],
                        m_vTriangles.data()[f1][2]);
                    vFlags[f1] = cc;
                    if (max_x(f1) > farthest_val) {
                        farthest_val = max_x(f1);
                        cc_farthest[cc] = f1;
                    }
                    q.push_back(f1);
                }
            }
        }
        cc++;
    }

    vector<bool> cc_flip(cc, false);
    for (int i = 0; i < cc; i++) {
        int f = cc_farthest[i];
        const point3d &v0 = m_vVertices.data()[m_vTriangles.data()[f][0]];
        const point3d &v1 = m_vVertices.data()[m_vTriangles.data()[f][1]];
        const point3d &v2 = m_vVertices.data()[m_vTriangles.data()[f][2]];
        int j = 0;
        if (v1[0] > v0[0])
        if (v2[0] > v1[0]) j = 2; else j = 1;
        else
        if (v2[0] > v0[0]) j = 2;
        int v = m_vTriangles.data()[f][j];
        const set<int> &a = m_vAdjTriPerVer[v];
        vector3d n;
        set<int>::iterator iter = a.begin();
        for (iter = a.begin(); iter != a.end(); ++iter) {
            int f1 = *iter;
            // Triangle& tri = m_vTriangles.data()[f1];
            const point3d &v0 = m_vVertices.data()[m_vTriangles.data()[f1][0]];
            const point3d &v1 = m_vVertices.data()[m_vTriangles.data()[f1][1]];
            const point3d &v2 = m_vVertices.data()[m_vTriangles.data()[f1][2]];
            n += trinorm(v0, v1, v2);
            //n += CalcTriNormal(tri.p0, tri.p1, tri.p2);
        }
        if (n[0] < 0.0f)
            cc_flip[i] = true;
    }

    for (int i = 0; i < m_vTriangles.size(); i++) {
        if (cc_flip[vFlags[i]])
            swap(m_vTriangles.data()[i][1], m_vTriangles.data()[i][2]);
    }
}

void Mesh::InitPerVertexVertexAdj()
{
    qDebug()<<"Enter init per ver ver adj !";

    if(IsPerVertexVertexInfoEnabled)
        ClearPerVertexVertexAdj();

    IsPerVertexVertexInfoEnabled = true;
    if(AdjInfos.size() != m_vVertices.size())
        AdjInfos.resize(m_vVertices.size());

    size_t vcount= m_vVertices.size();
    size_t fcount= m_vTriangles.size();
    for (size_t i = 0; i < vcount; i++)
    {
        std::vector<long> vertexAdjacencyList;
        vertexAdjacencyList.reserve(6);
        AdjInfos[i]=vertexAdjacencyList;
    }

    for (size_t i = 0; i < fcount; i++)
    {
        Triangle &t = m_vTriangles[i];
        std::vector<long>& p0list= AdjInfos[t.p0];
        std::vector<long>& p1list= AdjInfos[t.p1];
        std::vector<long>& p2list= AdjInfos[t.p2];

        if (std::find(p0list.begin(), p0list.end(), t.p1)==p0list.end())
            p0list.push_back(t.p1);

        if (std::find(p0list.begin(), p0list.end(), t.p2)==p0list.end())
            p0list.push_back(t.p2);

        if (std::find(p1list.begin(), p1list.end(), t.p0)==p1list.end())
            p1list.push_back(t.p0);

        if (std::find(p1list.begin(), p1list.end(), t.p2)==p1list.end())
            p1list.push_back(t.p2);

        if (std::find(p2list.begin(), p2list.end(), t.p0)==p2list.end())
            p2list.push_back(t.p0);

        if (std::find(p2list.begin(), p2list.end(), t.p1)==p2list.end())
            p2list.push_back(t.p1);
    }

    qDebug()<<"init per ver ver adj success !";
}

void Mesh::InitPerVertexTriangleAdj()
{
    if(IsPerVertexTriangleInfoEnabled)
        ClearPerVertexTriangleAdj();
    IsPerVertexTriangleInfoEnabled = true;

    if(AdjInfos.size()!=m_vVertices.size())
        AdjInfos.resize(m_vVertices.size());

    for (size_t i = 0; i < m_vVertices.size(); i++)
    {
        std::vector<long> triangleAdjacencyList;
        triangleAdjacencyList.reserve(6);
        AdjInfos[i]=triangleAdjacencyList;
    }

    for (size_t i = 0; i < m_vTriangles.size(); i++)
    {
        Triangle& t = m_vTriangles[i];
        std::vector<long>& t0list= AdjInfos[t.p0];
        std::vector<long>& t1list= AdjInfos[t.p1];
        std::vector<long>& t2list= AdjInfos[t.p2];
        t0list.push_back(i);
        t1list.push_back(i);
        t2list.push_back(i);
    }
}

void Mesh::ClearPerVertexVertexAdj()
{
    if(!IsPerVertexVertexInfoEnabled)
        return;

    IsPerVertexVertexInfoEnabled = false;
}

void Mesh::ClearPerVertexTriangleAdj()
{
    if(!IsPerVertexTriangleInfoEnabled)
        return;

    IsPerVertexTriangleInfoEnabled = false;
}

bool Mesh::GetIsPerVertexVertexInfoEnabled()
{
    return IsPerVertexVertexInfoEnabled;
}

bool Mesh::GetIsPerVertexTriangleInfoEnabled()
{
    return IsPerVertexTriangleInfoEnabled;
}
