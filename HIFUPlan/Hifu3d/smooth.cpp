
#include "smooth.h"
#include "qdebug.h"

Smooth::Smooth(Mesh* m)
{
        this->mesh=m;
        m->InitPerVertexVertexAdj();
}

Smooth::~Smooth()
{
        this->mesh=NULL;
}

void Smooth::Taubin(int iterationTime,float lambda,float mu)
{
    vector3d* tempList= new vector3d[mesh->m_vVertices.size()];
    for(int c=0;c<iterationTime;c++)
    {
        for(size_t i=0;i<mesh->m_vVertices.size();i++)
        {
            tempList[i]=GetSmoothedVertex_Taubin_Step(i,lambda);
        }

        for(size_t i=0;i<mesh->m_vVertices.size();i++)
        {
            mesh->m_vVertices[i]=tempList[i];
        }

        for(size_t i=0;i<mesh->m_vVertices.size();i++)
        {
            tempList[i]=GetSmoothedVertex_Taubin_Step(i,mu);
        }

        for(size_t i=0;i<mesh->m_vVertices.size();i++)
        {
            mesh->m_vVertices[i]=tempList[i];
        }
    }

    delete[] tempList;
}

vector3d Smooth::GetSmoothedVertex_Taubin_Step(size_t index,float lambda)
{
    float dx=0,dy=0,dz=0;
    std::vector<long>& adjm_vVertices = this->mesh->AdjInfos[index];
    vector3d& p = mesh->m_vVertices[index];

    if(adjm_vVertices.size()==0)
    {
        qDebug()<<"adjm_vVertices NULL !";
        return mesh->m_vVertices[index];
    }

    for(size_t i=0;i<adjm_vVertices.size();i++)
    {
        vector3d& t=mesh->m_vVertices[adjm_vVertices[i]];
        dx+=(t.x-p.x);
        dy+=(t.y-p.y);
        dz+=(t.z-p.z);
    }

    dx/=adjm_vVertices.size();
    dy/=adjm_vVertices.size();
    dz/=adjm_vVertices.size();

    float newx=lambda*dx+p.x;
    float newy=lambda*dy+p.y;
    float newz=lambda*dz+p.z;

    return vector3d(newx,newy,newz);
}

