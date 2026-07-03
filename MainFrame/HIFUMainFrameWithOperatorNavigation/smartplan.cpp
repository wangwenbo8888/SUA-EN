
#include "smartplan.h"


SmartPlan::SmartPlan(int angle,const SPOT_DISTRIBUTE& dist)
{
    // initAngleArray(angle,dist);
    initAngleArrayNew(angle,dist);
}

SmartPlan::~SmartPlan()
{
}

// 角度对应层数,默认初始角都是-90度
void SmartPlan::initAngleArrayNew(int angle,const SPOT_DISTRIBUTE& dist)
{
    m_eDensity = dist.eDensity;
    _mapAngleDist.clear();

    for(int i = 0; i < 180; ++i)
    {
        SPOT_DISTRIBUTE temp;
        temp.isLayerDown   = dist.isLayerDown;
        temp.isLayerUp     = dist.isLayerUp;
        temp.isLayerMiddle = dist.isLayerMiddle;
        bool isInsert = false;
        if (i%60==0&&dist.isCircleOne)
        {
            temp.setCircleOne(true);
            isInsert = true;
        }

        if (i%30==0&&dist.isCircleTwo)
        {
            temp.setCircleTwo(true);
            isInsert = true;
        }

        if (i%20==0&&dist.isCircleThree)
        {
            temp.setCircleThree(true);
            isInsert = true;
        }

        if (i%15==0&&dist.isCircleFour)
        {
            temp.setCircleFour(true);
            isInsert = true;
        }

        if (i%12==0&&dist.isCircleFive)
        {
            temp.setCircleFive(true);
            isInsert = true;
        }

        if (isInsert)
        {
            _mapAngleDist.insert(angle+i,temp);
        }
    }

    QMap<int,SPOT_DISTRIBUTE>::iterator iter = _mapAngleDist.find(0);
    if (iter == _mapAngleDist.end())
    {
        SPOT_DISTRIBUTE temp;
        temp.isLayerDown   = dist.isLayerDown;
        temp.isLayerUp     = dist.isLayerUp;
        temp.isLayerMiddle = dist.isLayerMiddle;
        temp.setCircleOne(false);
        temp.setCircleTwo(false);
        temp.setCircleThree(false);
        temp.setCircleFour(false);
        temp.setCircleFive(false);
        temp.setCenter(true);
        _mapAngleDist.insert(0,temp);
    }
    else
    {
        iter.value().setCenter(true);
    }
}
// 相对于初始点位置辐照点分布为
// 相对角度      0     20  30 40 60    80 90 100 120 140 150 160
// 点个数        7     2   2  2  6     2  2  2   6    2   2  2
// 半径       0,1,2,3  3   2  3 1,2,3  3  2  3  1,2,3 3   2  3
void SmartPlan::initAngleArray(int angle,const SPOT_DISTRIBUTE& dist)
{
    _mapAngleDist.clear();
    SPOT_DISTRIBUTE temp = dist;
    for (int i = 0; i < 3; ++i) // 0 4 8
    {
//        _arrayAngle[i*4] = angle+60*i;
        temp.setCenter(false);
//        _arrayStruct[i*4] = temp;
        _mapAngleDist.insert(angle+60*i,temp);
    }

    for (int i = 0;i < 3;++i)  // 1 5 9
    {
//        _arrayAngle[1+i*4] = angle+20+60*i;
        temp.setCenter(false);
        temp.setCircleOne(false);
        temp.setCircleTwo(false);
        if (dist.isCircleThree)
        {
            temp.setCircleThree(true);
        }
        else
        {
            temp.setCircleThree(false);
        }
//        _arrayStruct[1+i*4] = temp;
        _mapAngleDist.insert(angle+20+60*i,temp);
    }

    for (int i = 0;i < 3;++i)  // 3 7 11
    {
//        _arrayAngle[3+i*4] = angle+40+60*i;
        temp.setCenter(false);
        temp.setCircleOne(false);
        temp.setCircleTwo(false);
        if (dist.isCircleThree)
        {
            temp.setCircleThree(true);
        }
        else
        {
            temp.setCircleThree(false);
        }
//        _arrayStruct[3+i*4] = temp;
        _mapAngleDist.insert(angle+40+60*i,temp);
    }

    for (int i = 0; i < 3; ++i)  // 2 6 10
    {
//        _arrayAngle[2+i*4] = angle+30+60*i;
        temp.setCenter(false);
        temp.setCircleOne(false);
        if (dist.isCircleTwo)
        {
            temp.setCircleTwo(true);
        }

        temp.setCircleThree(false);
//        _arrayStruct[2+i*4] = temp;
        _mapAngleDist.insert(angle+30+60*i,temp);
    }

    QMap<int,SPOT_DISTRIBUTE>::iterator iter = _mapAngleDist.find(0);
    if (iter == _mapAngleDist.end())
    {
        temp.setCircleOne(false);
        temp.setCircleTwo(false);
        temp.setCircleThree(false);
        temp.setCenter(true);
        _mapAngleDist.insert(0,temp);
    }
    else
    {
        iter.value().setCenter(true);
    }
}
