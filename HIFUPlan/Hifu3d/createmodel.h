#ifndef CREATEMODEL_H_
#define CREATEMODEL_H_

#include "mesh.h"

class CreateModel
{
public:
    CreateModel();
    ~CreateModel();

    void createBall(Mesh* pBall);

    void createSpot(Mesh* pSpot);

    void createSpotBlock(Mesh* block);

public:

};

#endif // CREATEMODEL_H_

