#ifndef HIFUALGOPROCESSOR_H
#define HIFUALGOPROCESSOR_H

#include "hifualgoprocessor_global.h"
#include "SeededGrow/seededgrow3d.h"
#include "SurfaceReconstruction/smc_adsmcprocessor.h"

class HIFUALGOPROCESSORSHARED_EXPORT HIFUAlgoProcessor
{

public:
    HIFUAlgoProcessor();

    void SeededGrow3dProcess(short* data, int width, int heigt, int depth, int seedx, int seedy, int seedz, int thresholdmin, int thresholdmax,uchar* outimage);

    void DataPostProcess(int width, int heigt, int depth,uchar* mask);

    void SurfaceRenderingProcess(short* data, int width, int heigt, int depth, double spacingX,double spacingY,double spacingZ);
};

#endif // HIFUALGOPROCESSOR_H
