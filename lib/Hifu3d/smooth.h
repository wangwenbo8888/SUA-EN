#ifndef SMOOTH
#define SMOOTH

#include "mesh.h"
#include "hifu3d_global.h"

class HIFU3D_EXPORT Smooth
{

public:
    Smooth(Mesh* m);
    ~Smooth();

public:

    void Taubin(int iterationTime,float lambda,float mu);

private:
    vector3d GetSmoothedVertex_Taubin_Step(size_t index,float lambda);
private:
    Mesh* mesh;
};

#endif // SMOOTH

