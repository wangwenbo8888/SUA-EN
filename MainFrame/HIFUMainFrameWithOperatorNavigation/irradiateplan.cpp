#include "irradiateplan.h"
#include "qloggingcategory.h"
#include "constant.h"
#include <QBrush>
#include <QMessageBox>
#include "qstandarditemmodel.h"

#include "time.h"

#define DELTA 0.000001
#define PI    3.1415926
#define RADIUSSQUARE 0.00000081    // 圈间隔1.8mm，一半0.9mm的平方

Q_LOGGING_CATEGORY(IRRADIATEPLAN,"IRRADIATEPLAN")

// TODO 转换成实际公式换算
int powerToVoltage(int power)
{
    if (power>100)
    {
        return 5;
    }
    else
    {
        return 3;
    }
}

// TODO 转换成实际公式换算
int energyToVoltage(int energy)
{
    if (energy>100)
    {
        return 5;
    }
    else
    {
        return 3;
    }
}

IrradiatePlan::IrradiatePlan(int focusCount,
              const QQueue<int>& pulseCounts,
              const QQueue<double>& x,
              const QQueue<double>& y,
              const QQueue<double>& z,
              const QQueue<Circle>& circles,
              const QQueue<QString>& idBorns,
              const QQueue<double>& voltages,
              const QQueue<int>& onTimes,
              const QQueue<int>& offTimes,
              const QQueue<int>& coolingTimes,
              Irradiate_model model)
{
    _plan.clear();
    _layer = Layer_end;   // 初始化层
    // _angle = -100;        // 初始化Angle

    size_t size = x.size();
    for (size_t i = 0; i<size; ++i)
    {
        Irradiate_Spot spot;
        spot.focusCount = focusCount;
        spot.pulseCount = pulseCounts[i];
        spot.coolingTime = coolingTimes[i];
        spot.voltage = voltages[i];
        spot.onTime = onTimes[i];
        spot.offTime = offTimes[i];
        spot.position.x = x[i];
        spot.position.y = y[i];
        spot.position.z = z[i];
        spot.circle = circles[i];  // 半径的平方
        spot.idBorn = idBorns[i];
        spot.isIrradiated = false;
        spot.usAngle = getAngle(x[i],y[i]);
        spot.spotAngle = spot.angle(QPointF(0.0,0.0),QPointF(x[i],y[i]));
        spot.layer = getLayer(z[i]);
        _plan.push_back(spot);
    }

    sortSpots(model);
    _displayPlan.clear();
    //printfSpots();
}

IrradiatePlan::~IrradiatePlan()
{
}

// 更新TreatmentPlan，将未sonication的点全部Delete，Add进新的sonication point
//void IrradiatePlan::refreshPlan(int angle,
//                                const QQueue<double>& x,
//                                const QQueue<double>& y,
//                                const QQueue<double>& z,
//                                const QQueue<QString>& idBorn,
//                                QMap<QString,bool>& flag)
//{
//    // Delete当前显示的Plan
//    _displayPlan.clear();

//    // DeleteOverall Plan中已经sonication完的Plan
//    Irradiate_Spot spot = *_plan.begin();
//    int index = _plan.size();
//    QQueue<Irradiate_Spot>::iterator iter = _plan.begin();
//    if (flag.empty())
//    {
//        // NonesonicationComplete点，全部Delete
//        _plan.clear();
//    }
//    else
//    {
//        for (;iter != _plan.end(); ++iter)
//        {
//            if (flag.find(iter->id)==flag.end())
//            {
//                // Delete未Complete的sonication point
//                _plan.erase(iter);
//                iter = _plan.begin();
//            }
//        }
//    }

//    for (int i = 0; i < x.size(); ++i)
//    {
//        spot.id = QString::number(++index);
//        spot.usAngle = angle;
//        spot.position.x = x[i];
//        spot.position.y = y[i];
//        spot.position.z = z[i];
//        spot.idBorn = idBorn[i];
//        spot.layer = getLayer(z[i]);
//        spot.isIrradiated = false;
//        _plan.push_back(spot);
//    }

//    // qDebug()<<"add spot success !";
//}

// 更新sonication参数
void IrradiatePlan::refreshIrradiatePara(const double voltage,
                                         const int onTime,
                                         const int offTime,
                                         const int coolingTime,
                                         const int pulseCount,
                                         const QMap<QString,bool>& current,
                                         const QMap<QString,bool>& total)
{


    for (QQueue<Irradiate_Spot>::iterator iter = _displayPlan.begin();
         iter != _displayPlan.end();++iter)
    {
        if (current.find(iter->id)==current.end())
        {
            // 只更新sonicationEnergy参数
            iter->voltage = voltage;
            iter->pulseCount = pulseCount;
            iter->onTime = onTime;
            iter->offTime = offTime;
            iter->coolingTime = coolingTime;
        }
    }

    for (QQueue<Irradiate_Spot>::iterator iter = _plan.begin();
         iter != _plan.end();++iter)
    {
        if (total.find(iter->id)==total.end())
        {
            // 只更新sonicationEnergy参数
            iter->voltage = voltage;
            iter->pulseCount = pulseCount;
            iter->onTime = onTime;
            iter->offTime = offTime;
            iter->coolingTime = coolingTime;
        }
    }
}

QString IrradiatePlan::createNewId(QString id)
{
    int i = 1;
    QMap<QString,bool> temp;
    QQueue<Irradiate_Spot>::iterator iter = _displayPlan.begin();
    for (;iter != _displayPlan.end();++iter)
    {
        temp.insert(iter->id,true);
    }

    QString idNew = id;
    idNew.append("_1");
    while (temp.find(idNew)!=temp.end())
    {
        idNew = id;
        idNew.append("_");
        idNew.append(QString::number(i));
        ++i;
    }

    return idNew;
}

void IrradiatePlan::copySpots(QVector<QString>& idVec)
{
    QQueue<Irradiate_Spot> temp;
    QVector<QString>::iterator iterVec = idVec.begin();
    QQueue<Irradiate_Spot>::iterator iterQueue = _displayPlan.begin();
    QQueue<Irradiate_Spot>::iterator iterDisplayInsert = _displayPlan.end();
    Irradiate_Spot spot;
    for (iterVec = idVec.begin(); iterVec != idVec.end(); ++iterVec)
    {
        for (iterQueue = _displayPlan.begin();iterQueue != _displayPlan.end(); ++iterQueue)
        {
            if (iterQueue->id==*iterVec)
            {
                spot = *iterQueue;
                spot.id = createNewId(spot.id);
                temp.push_back(spot);
                iterDisplayInsert = iterQueue;
                break;
            }
        }
    }

    if (iterDisplayInsert==_displayPlan.end())
    {
        return;
    }

    QQueue<Irradiate_Spot>::iterator iterTotalInsert = _plan.end();
    for (iterQueue = _plan.begin();iterQueue != _plan.end();++iterQueue)
    {
        if (iterDisplayInsert->id==iterQueue->id)
        {
            iterTotalInsert = iterQueue;
            break;
        }
    }

    // 显示PlanAdd新的sonication point
    for (iterQueue = temp.begin(); iterQueue != temp.end(); ++iterQueue)
    {
        iterDisplayInsert = _displayPlan.insert(++iterDisplayInsert,*iterQueue);
    }

    // 整个PlanAdd新的sonication point
    if (iterTotalInsert!=_plan.end())
    {
        for (iterQueue = temp.begin(); iterQueue != temp.end(); ++iterQueue)
        {
            iterQueue->isIrradiated = true;
            iterTotalInsert = _plan.insert(++iterTotalInsert,*iterQueue);
        }
    }
}

void IrradiatePlan::deleteSpots(QVector<QString>& idVec)
{
    // Deletecurrent plan中选中的sonication point
    QVector<QString>::iterator iterSet = idVec.begin();
    QQueue<Irradiate_Spot>::iterator iterQueue = _displayPlan.begin();
    for (iterSet = idVec.begin(); iterSet != idVec.end(); ++iterSet)
    {
        for (iterQueue = _displayPlan.begin();iterQueue != _displayPlan.end(); ++iterQueue)
        {
            if (iterQueue->id==*iterSet)
            {
                iterQueue = _displayPlan.erase(iterQueue);
                break;
            }
        }
    }

    // Delete整个Plan中选中的sonication point
    for (iterSet = idVec.begin(); iterSet != idVec.end(); ++iterSet)
    {
        for (iterQueue = _plan.begin();iterQueue != _plan.end(); ++iterQueue)
        {
            if (iterQueue->id==*iterSet)
            {
                iterQueue = _plan.erase(iterQueue);
                break;
            }
        }
    }
}

void IrradiatePlan::itemChanged(QStandardItem* item)
{
    int row = item->row();
    int column = item->column();
    QQueue<Irradiate_Spot>::iterator iter = _displayPlan.begin()+row;
    int value = item->text().toInt();
    if (column==3)  //Power
    {
        iter->voltage = powerToVoltage(value);
    }
    else if (column == 4)
    {
        iter->voltage = energyToVoltage(value);
    }
}

void IrradiatePlan::showNextPlan()
{
    QQueue<Irradiate_Spot> plan;
    this->getNextPlan(plan);
}

int IrradiatePlan::getAngle(double x,double y)
{
    if (fabs(y)<DELTA)
    {
        return 0;
    }
    else if (fabs(x)<DELTA)
    {
        return -90;
    }
    else
    {
        // 四舍五入Angle
        float angle = atan(y/x)*180/PI;
        if (angle < 0.0)
        {
            return int(-0.5+angle);
        }
        else
        {
            return int(0.5+angle);
        }
    }
}

Circle IrradiatePlan::getCircle(double x,double y)
{
    double temp = x*x+y*y;
    if (temp <= RADIUSSQUARE)
    {
        return Circle_center;
    }
    else if (temp <= 9*RADIUSSQUARE && temp > RADIUSSQUARE)
    {
        return Circle_one;
    }
    else if (temp <= 25*RADIUSSQUARE && temp > 9*RADIUSSQUARE)
    {
        return Circle_two;
    }
    else if (temp <= 49*RADIUSSQUARE && temp > 25*RADIUSSQUARE)
    {
        return Circle_three;
    }
    else
    {
        return Circle_end;
    }

}

Layer IrradiatePlan::getLayer(double z)
{
    if (z>0.145&&z<0.155)
    {
        // LowerTreatment平面
        return Layer_down;

    }
    else if (z<0.145&&z>0.135)
    {
        // MiddleTreatment平面
        return Layer_middle;

    }
    else if(z<0.135&&z>0.125)
    {
        // UpperTreatment平面
        return Layer_up;
    }
}

void IrradiatePlan::sortSpots(Irradiate_model model)
{
    QQueue<Irradiate_Spot>::iterator iter;
    // 冒泡排序
    if (model == Model_inner_to_outer)
    {
        iter = _plan.begin();
        for (;iter != _plan.end();++iter)
        {
            QQueue<Irradiate_Spot>::iterator iterTemp = _plan.begin();
            for (;iterTemp <= iter;++iterTemp)
            {
                if(*iter<*iterTemp)
                {
                    Irradiate_Spot temp = *iterTemp;
                    *iterTemp = *iter;
                    *iter = temp;
                }
            }
        }
    }
    else
    {
        iter = _plan.begin();
        for (;iter != _plan.end();++iter)
        {
            QQueue<Irradiate_Spot>::iterator iterTemp = _plan.begin();
            for (;iterTemp <= iter;++iterTemp)
            {
                if(*iter>*iterTemp)
                {
                    Irradiate_Spot temp = *iterTemp;
                    *iterTemp = *iter;
                    *iter = temp;
                }
            }
        }
    }


    // 给id赋值
    iter = _plan.begin();
    int id = 1;
    for (;iter != _plan.end();++iter)
    {
        iter->id = QString::number(id);
        ++id;
    }
}

QQueue<Irradiate_Spot> IrradiatePlan::getTotalPlan()
{
    return _plan;
}

int IrradiatePlan::getDisplaySize()
{
    return _displayPlan.size();
}

bool IrradiatePlan::getCoolingTime(int& second)
{
    if (_displayPlan.empty()||_displayPlan.size()<=0||_displayPlan.size()>1000)
    {
        return false;
    }
    second = _displayPlan.begin()->coolingTime;
    return true;
}

bool IrradiatePlan::getDisplayAngle(int& angle)
{
    if (_displayPlan.empty())
    {
        return false;
    }

    angle = _displayPlan.begin()->usAngle;
    return true;
}

void IrradiatePlan::updatePlan(const Irradiate_Spot& spot)
{
    for (QQueue<Irradiate_Spot>::iterator iter = _displayPlan.begin();
         iter != _displayPlan.end();++iter)
    {
        if (iter->id==spot.id)
        {
            // 只更新sonicationEnergy参数
            iter->voltage = spot.voltage;
            iter->pulseCount = spot.pulseCount;
//            iter->focusCount = spot.focusCount;
        }
    }

    for (QQueue<Irradiate_Spot>::iterator iter = _plan.begin();
         iter != _plan.end();++iter)
    {
        if (iter->id==spot.id)
        {
            // 只更新sonicationEnergy参数
            iter->voltage = spot.voltage;
            iter->pulseCount = spot.pulseCount;
//            iter->focusCount = spot.focusCount;
        }
    }


}

bool IrradiatePlan::getIrradiateSpot(QMap<QString,bool>& flagMap,Irradiate_Spot& spot)
{
    qDebug()<<"flagMap====="<<flagMap.size();
    qDebug()<<"disp size6====="<<getDisplaySize();
    if (_displayPlan.empty())
    {
        return false;
    }
    qDebug()<<"displaysize====="<<_displayPlan.size();
    for (QQueue<Irradiate_Spot>::iterator iter = _displayPlan.begin();
         iter != _displayPlan.end();++iter)
    {
        qDebug()<<"SSSSSSSSSSSSSSS";
        // 按顺序找到了未sonication的点
        if (flagMap.find(iter->id)==flagMap.end())
        {
            spot = *iter;
            return true;
        }
    }

    return false;
}

void IrradiatePlan::refreshDisplayPlan(const QVector<QString>& ids)
{
    if (_displayPlan.size()!=ids.size())
    {
        qCDebug(IRRADIATEPLAN()) << "refresh size and display size not equal !"
                                 << TRACE_CMH();

        return;
    }

    QQueue<Irradiate_Spot> temp = _displayPlan;
    _displayPlan.clear();
    for (int i = 0; i < ids.size();++i)
    {
        QQueue<Irradiate_Spot>::iterator iter = temp.begin();
        for (;iter != temp.end();++iter)
        {
            if (ids[i]==iter->id)
            {
                _displayPlan.push_back(*iter);
                break;
            }
        }
    }
}

void IrradiatePlan::getDisplayPlan(QQueue<Irradiate_Spot>& plan)
{
    if (_displayPlan.empty())
    {
        return;
    }

    plan.clear();
    for (QQueue<Irradiate_Spot>::iterator iter = _displayPlan.begin();
         iter != _displayPlan.end();++iter)
    {
        plan.push_back(*iter);
    }
}

void IrradiatePlan::getNextPlan(QQueue<Irradiate_Spot>& plan)
{
    if (_plan.empty())
    {
        qDebug()<<"IrradiatePlan _plan empty !";
        return;
    }

    _layer = Layer_end;
    QQueue<Irradiate_Spot>::iterator iter = _plan.begin();
    for (; iter != _plan.end();++iter)
    {
        if (!iter->isIrradiated)
        {
            _layer = iter->layer;
            // _angle = iter->angle;
            break;
        }
    }

    if (_layer==Layer_end)
    {
        qDebug()<<"IrradiatePlan plan over !";
        return;
    }
    plan.clear();

    _displayPlan.clear();

    qDebug()<<"PLAN SIZE======"<<_plan.size();
    iter = _plan.begin();
    for (;iter != _plan.end();++iter)
    {
        if (iter->layer==_layer && !iter->isIrradiated)
        {
            plan.push_back(*iter);
            _displayPlan.push_back(*iter);
            iter->isIrradiated = true;
        }
    }
    qDebug()<<"displan size====="<<_displayPlan.size();
}

void IrradiatePlan::printfSpots()
{
    QQueue<Irradiate_Spot>::iterator iter = _plan.begin();
    for (;iter != _plan.end();++iter)
    {
        qDebug()<<"id is "<< iter->id
                            <<"focus count is "<< iter->focusCount
                             << "pulse count is "<< iter->pulseCount
                             << "cooling time is "<< iter->coolingTime
                             << "voltage is " <<iter->voltage
                             << "time on is "<< iter->onTime
                             << "off time is "<< iter->offTime
                             << "angle is "<< iter->usAngle
                             << "layer is " <<iter->layer
                             << "pos x is "<< iter->position.x
                             << "pos y is "<< iter->position.y
                             << "pos z is "<< iter->position.z
                             <<"is irradiated is "<<iter->isIrradiated
                             << TRACE_CMH();

    }
}

