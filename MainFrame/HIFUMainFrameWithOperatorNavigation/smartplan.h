#ifndef SMARTPLAN_H_
#define SMARTPLAN_H_

#include "drawscene.h"

class SmartPlan
{
public:
    SmartPlan(int angle,const SPOT_DISTRIBUTE& dist);
    ~SmartPlan();

    int getAngle(size_t i)
    {
        QMap<int,SPOT_DISTRIBUTE>::Iterator iter = _mapAngleDist.begin();
        return (iter+i).key();
    }

    SPOT_DISTRIBUTE getDistribute(size_t i)
    {
        QMap<int,SPOT_DISTRIBUTE>::Iterator iter = _mapAngleDist.begin();
        return (iter+i).value();
    }

    size_t getSize()
    {
        return _mapAngleDist.size();
    }

    DENSITY_TYPE getDensity()
    {
        return m_eDensity;
    }

private:
    void initAngleArray(int angle,const SPOT_DISTRIBUTE& dist);
    void initAngleArrayNew(int angle,const SPOT_DISTRIBUTE& dist);
    QMap<int,SPOT_DISTRIBUTE> _mapAngleDist;
    DENSITY_TYPE m_eDensity;
};

#endif // SMARTPLAN_H_

