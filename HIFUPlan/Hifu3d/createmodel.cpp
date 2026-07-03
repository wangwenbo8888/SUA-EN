

#include "createmodel.h"
#include <math.h>

CreateModel::CreateModel()
{
}

CreateModel::~CreateModel()
{
}

void CreateModel::createBall(Mesh* pBall)
{
    // 球直径1mm
    if (pBall == NULL)
    {
        pBall = new Mesh;
    }
    else
    {
        pBall->m_vVertices.clear();
        pBall->m_vTriangles.clear();
        pBall->m_vVerNormal.clear();
    }

    float pi=3.1415926;
    float theta=0.0;
    float phi=0.0;
    float radius = 5.0;

    for(int i= 1; i < 20; i++)
    {
        theta =  0.5*pi*i*0.1;
        for(int j= 0; j < 20; j++)
        {
            phi = pi* j *0.1;
            pBall->m_vVertices.push_back(vector3d(radius*sin(theta)*cos(phi),
                                                radius*sin(theta)*sin(phi),
                                                radius*cos(theta)));
        }
    }

    // 加上两端点
    pBall->m_vVertices.push_back(vector3d(0.0,0.0,radius));
    pBall->m_vVertices.push_back(vector3d(0.0,0.0,-radius));

    for (int i = 0 ;i < 18; ++i)
    {
        for (int j = 0; j < 19 ; ++j)
        {
            pBall->m_vTriangles.push_back(Triangle(j+20*i,j+20*(i+1),j+1+20*(i+1)));
            pBall->m_vTriangles.push_back(Triangle(j+20*i,j+1+20*(i+1),j+1+20*i));
        }
        pBall->m_vTriangles.push_back(Triangle(19+20*i,19+20*(i+1),20*(i+1)));
        pBall->m_vTriangles.push_back(Triangle(19+20*i,20*(i+1),20*i));
    }

    for (int i = 0; i < 19; ++i)
    {
        pBall->m_vTriangles.push_back(Triangle(380,i,i+1));
        pBall->m_vTriangles.push_back(Triangle(i+361,i+360,381));
    }
    pBall->m_vTriangles.push_back(Triangle(380,19,0));
    pBall->m_vTriangles.push_back(Triangle(360,379,381));

    if (pBall->m_vVerNormal.empty())
    {
        pBall->CalcVerNomal();
    }
}
void CreateModel::createSpotBlock(Mesh* pBlock)
{
    // block 直径12mm,高10mm
    if (pBlock == NULL)
    {
        pBlock = new Mesh;
    }
    else
    {
        pBlock->m_vVertices.clear();
        pBlock->m_vTriangles.clear();
        pBlock->m_vVerNormal.clear();
    }

    float pi=3.1415926;
    float theta=0.0;
    float radius = 6;
    float height = 10.0;

    for(int i= 0; i < 20; i++)
    {
        theta =  0.1*pi*i;
        pBlock->m_vVertices.push_back(vector3d(radius*cos(theta),
                                               0.5*height,
                                               radius*sin(theta)));
    }

    for(int i= 0; i < 20; i++)
    {
        theta =  0.1*pi*i;
        pBlock->m_vVertices.push_back(vector3d(radius*cos(theta),
                                               -0.5*height,
                                               radius*sin(theta)));
    }

    // 加上两端点
    pBlock->m_vVertices.push_back(vector3d(0.0,0.5*height,0.0));
    pBlock->m_vVertices.push_back(vector3d(0.0,-0.5*height,0.0));

    for (int i = 1 ;i <= 19; ++i)
    {
        pBlock->m_vTriangles.push_back(Triangle(i-1,i,i+20));
        pBlock->m_vTriangles.push_back(Triangle(i+19,i-1,i+20));
    }

    pBlock->m_vTriangles.push_back(Triangle(19,0,20));
    pBlock->m_vTriangles.push_back(Triangle(39,19,20));

    for (int i = 1; i < 20;++i)
    {
        pBlock->m_vTriangles.push_back(Triangle(i-1,40,i));
        pBlock->m_vTriangles.push_back(Triangle(41,i+19,i+20));
    }
    pBlock->m_vTriangles.push_back(Triangle(19,40,0));
    pBlock->m_vTriangles.push_back(Triangle(41,39,20));

    if (pBlock->m_vVerNormal.empty())
    {
        pBlock->CalcVerNomal();
    }
}

void CreateModel::createSpot(Mesh* pSpot)
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
