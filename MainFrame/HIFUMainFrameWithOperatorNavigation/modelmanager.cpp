#include "modelmanager.h"
#include "mesh.h"
#include "qdebug.h"

const float PI = 3.1415926;

ModelManager::ModelManager(Mesh* pMyoma)
{
    _trisIndex=NULL;
    _colorIndexs=NULL;
    _versPos=NULL;

    _headerVerIndex = 0;    // 位置数组中当前第一个点对应位置索引
    _headerTrisIndex = 0;

    _pMyoma = pMyoma;
    initScene();
}

ModelManager::~ModelManager()
{
    _pMyoma = NULL;  // 资源由传入者释放

    if (_trisIndex!=NULL)
    {
        delete []_trisIndex;
       _trisIndex = NULL;
    }

    if (_colorIndexs!=NULL)
    {
        delete []_colorIndexs;
        _colorIndexs = NULL;
    }

    if (_versPos!=NULL)
    {
        delete []_versPos;
        _versPos = NULL;
    }
}

GLuint* ModelManager::getTrisIndex()          // 三角形顶点索引数组首地址
{
    return _trisIndex;
}

GLfloat* ModelManager::getColorIndexs()       // 三角形顶点颜色数值首地址
{
    return _colorIndexs;
}

GLfloat* ModelManager::getVersPos()           // 三角形顶点位置首地址
{
    return _versPos;
}
// 初始化可视化窗口各模型
// 一个肌瘤模型，三个治疗层面模型，一个人体模型，111个治疗点模型
void ModelManager::initScene()
{
    Mesh* pPlane = new Mesh;
    Mesh* pWoman = new Mesh;
    Mesh* pSpot = new Mesh;
    createPlane(16.0,16.0,pPlane);
    loadWomanModel(pWoman);
    createSpot(pSpot);

//    _versSize = _pMyoma->m_vVertices.size() + pPlane->m_vVertices.size()*3
//                + pWoman->m_vVertices.size() + 111*pSpot->m_vVertices.size();
//    _versSize *= 3;
//    _trisIndexSize = _pMyoma->m_vTriangles.size() + pPlane->m_vTriangles.size()*3
//                + pWoman->m_vTriangles.size() + 111*pSpot->m_vTriangles.size();
//    _trisIndexSize*=3;
//    _colorsLen = 4*_versSize/3;

  _versSize = pPlane->m_vVertices.size()*3+111*pSpot->m_vVertices.size();
  _versSize *= 3;
  _trisIndexSize = pPlane->m_vTriangles.size()*3+111*pSpot->m_vTriangles.size();
  _trisIndexSize*=3;
  _colorsLen = 4*_versSize/3;

    _trisIndex = new GLuint[_trisIndexSize] ;
    _colorIndexs = new GLfloat[_colorsLen];
    _versPos = new GLfloat[_versSize];
    GLfloat matrix[12];
#if 0

    matrix[0] = 0.8f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.8f;
    matrix[7] = -6.0f;
    matrix[8] = 0.0f;
    matrix[9] = -0.8f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;

    addModel(_pMyoma,matrix);


    // x,z互换
    matrix[0] = 0.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.1f;
    matrix[3] = -8.0f;
    matrix[4] = 0.0f;
    matrix[5] = -0.1f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.1f;
    matrix[9] = 0.0f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;
    addModel(pWoman,matrix);
#endif

    // 添加下面平面
    matrix[0] = 0.8f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.8f;
    matrix[7] = -10.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.8f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;
    addModel(pPlane,matrix);

    // 添加中间平面
    matrix[0] = 0.8f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.8f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.8f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;
    addModel(pPlane,matrix);

    // 添加上面平面
    matrix[0] = 0.8f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.8f;
    matrix[7] = 10.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.8f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;

    addModel(pPlane,matrix);

    float radius = 2.0f;
    float theta;
    matrix[0] = 0.8f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.8f;
    matrix[8] = 0.0f;
    matrix[9] = 0.8f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;
    for (int i = 0; i < 3;++i)  // 三层
    {
        matrix[7] = 10.0f*(i-1);
        matrix[3] = 0.0;
        matrix[11] = 0.0;
        addModel(pSpot,matrix); // 添加最中间点
        radius = 2.0;
        for (int j = 0; j < 6; ++j) // 添加内圈6个点
        {
            theta = j*2.0*PI/6.0;
            matrix[3] = radius*cos(theta);
            matrix[11] = radius*sin(theta);
            addModel(pSpot,matrix);
        }

        radius = 4.0;
        for (int j = 0; j < 12; ++j) // 添加内圈6个点
        {
            theta = j*2.0*PI/12.0;
            matrix[3] = radius*cos(theta);
            matrix[11] = radius*sin(theta);
            addModel(pSpot,matrix);
        }

        radius = 6.0;
        for (int j = 0; j < 18; ++j) // 添加内圈6个点
        {
            theta = j*2.0*PI/18.0;
            matrix[3] = radius*cos(theta);
            matrix[11] = radius*sin(theta);
            addModel(pSpot,matrix);
        }
    }

    delete pPlane;
    delete pSpot;
    delete pWoman;
}

void ModelManager::addModel(Mesh* pModel,GLfloat* matrix)
{
    if (pModel==NULL)
    {
        return;
    }

    GLuint* index = _trisIndex+3*_headerTrisIndex;
    GLfloat* color = _colorIndexs+4*_headerVerIndex;
    GLfloat* pos = _versPos+3*_headerVerIndex;

    std::vector<point3d>& vers = pModel->m_vVertices;
    size_t versSize = vers.size();
    for (size_t i = 0; i < versSize; ++i)
    {
        *pos++ = matrix[0]*vers[i].x+matrix[1]*vers[i].y+matrix[2]*vers[i].z+matrix[3];
        *pos++ = matrix[4]*vers[i].x+matrix[5]*vers[i].y+matrix[6]*vers[i].z+matrix[7];
        *pos++ = matrix[8]*vers[i].x+matrix[9]*vers[i].y+matrix[10]*vers[i].z+matrix[11];
    }

    std::vector<vector3d>& normals = pModel->m_vVerNormal;
    for (size_t i = 0; i < versSize;++i)
    {
        *color++ = normals[i].x;
        *color++ = normals[i].y;
        *color++ = normals[i].z;
        *color++ = 1.0f;
    }

    std::vector<Triangle>& tris = pModel->m_vTriangles;
    size_t trisSize = tris.size();
    for (size_t i = 0; i < trisSize; ++i)
    {
        *index++ = tris[i].p0+_headerVerIndex;
        *index++ = tris[i].p1+_headerVerIndex;
        *index++ = tris[i].p2+_headerVerIndex;
    }

    _headerVerIndex  += versSize;
    _headerTrisIndex += trisSize;
}

void ModelManager::createSpot(Mesh* pSpot)
{

    if (pSpot == NULL)
    {
        pSpot = new Mesh;
    }
    else
    {
        pSpot->m_vVertices.clear();
        pSpot->m_vTriangles.clear();
        pSpot->m_vVerNormal.clear();
    }

    float pi=3.1415926;
    float theta=0.0;
    float phi=0.0;
    float radius = 1.5;
    float height = 8.0;

    for(int i= 1; i <= 10; i++)
    {
        theta =  0.5*pi*i*0.1;
        for(int j= 0; j < 20; j++)
        {
            phi = pi* j *0.1;
            pSpot->m_vVertices.push_back(vector3d(radius*sin(theta)*cos(phi),
                                                radius*sin(theta)*sin(phi),
                                                radius*cos(theta)+0.5*height));
        }
    }

    for(int i= 10; i < 20; i++)
    {
        theta =  0.5*pi*i*0.1;
        for(int j= 0; j < 20; j++)
        {
            phi = pi* j *0.1;
            pSpot->m_vVertices.push_back(vector3d(radius*sin(theta)*cos(phi),
                                                radius*sin(theta)*sin(phi),
                                                radius*cos(theta)-0.5*height));

        }
    }
    // 加上两端点
    pSpot->m_vVertices.push_back(vector3d(0.0,0.0,radius+0.5*height));
    pSpot->m_vVertices.push_back(vector3d(0.0,0.0,-radius-0.5*height));

    for (int i = 0 ;i < 19; ++i)
    {
        for (int j = 0; j < 19 ; ++j)
        {
            pSpot->m_vTriangles.push_back(Triangle(j+20*i,j+20*(i+1),j+1+20*(i+1)));
            pSpot->m_vTriangles.push_back(Triangle(j+20*i,j+1+20*(i+1),j+1+20*i));
        }
        pSpot->m_vTriangles.push_back(Triangle(19+20*i,19+20*(i+1),20*(i+1)));
        pSpot->m_vTriangles.push_back(Triangle(19+20*i,20*(i+1),20*i));
    }

    for (int i = 0; i < 19; ++i)
    {
        pSpot->m_vTriangles.push_back(Triangle(400,i,i+1));
        pSpot->m_vTriangles.push_back(Triangle(i+381,i+380,401));
    }
    pSpot->m_vTriangles.push_back(Triangle(400,19,0));
    pSpot->m_vTriangles.push_back(Triangle(380,399,401));

    if (pSpot->m_vVerNormal.empty())
    {
        pSpot->CalcVerNomal();
    }
}

int ModelManager::getVerSize()
{
    return _versSize;
}

int ModelManager::getTrisIndexSize()
{
    return _trisIndexSize;
}

int ModelManager::getColorsLen()
{
    return _colorsLen;
}

// 创建一个宽为wide,高为height，中心在坐标原点的平面
void ModelManager::createPlane(float wide, float height,Mesh* pPlane)
{
    if (pPlane==NULL)
    {
        pPlane = new Mesh;
    }
    else
    {
        pPlane->m_vVertices.clear();
        pPlane->m_vTriangles.clear();
        pPlane->m_vVerNormal.clear();
    }

    pPlane->m_vVertices.push_back(vector3d(0.5*wide,0.5*height,0));
    pPlane->m_vVertices.push_back(vector3d(-0.5*wide,0.5*height,0));
    pPlane->m_vVertices.push_back(vector3d(-0.5*wide,-0.5*height,0));
    pPlane->m_vVertices.push_back(vector3d(0.5*wide,-0.5*height,0));

    pPlane->m_vTriangles.push_back(Triangle(0,1,2));
    pPlane->m_vTriangles.push_back(Triangle(0,2,1));
    pPlane->m_vTriangles.push_back(Triangle(0,3,2));
    pPlane->m_vTriangles.push_back(Triangle(0,2,3));

    if (pPlane->m_vVerNormal.empty())
    {
        size_t versSize = pPlane->m_vVertices.size();
        for (size_t i = 0; i < versSize; ++i)
        {
            pPlane->m_vVerNormal.push_back(vector3d(0.0,1.0,1.0));
        }
    }
}

void ModelManager::loadWomanModel(Mesh* pHuman)
{
    if(pHuman==NULL)
    {
        pHuman = new Mesh;
    }
    else
    {
        pHuman->m_vVertices.clear();
        pHuman->m_vTriangles.clear();
        pHuman->m_vVerNormal.clear();
    }

    pHuman->ReadStl("../resources/humanmodel.stl");
    pHuman->CalcVerNomal();
}
