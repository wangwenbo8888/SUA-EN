#ifndef IRRADIATEPLAN_H_
#define IRRADIATEPLAN_H_

#include <QQueue>
#include <QString>
#include <QObject>
#include "irradiatespot.h"

#include <QLoggingCategory>
#include "constant.h"

class CurrentPlanModel;
class TotalPlanModel;
class QStandardItem;

Q_DECLARE_LOGGING_CATEGORY(IRRADIATEPLAN)

// Sonication Plan
class IrradiatePlan : public QObject
{
    Q_OBJECT

public:

    IrradiatePlan(int focusCount,
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
                  Irradiate_model model);

    ~IrradiatePlan();

    // 更新TreatmentPlan，将未sonication的点全部Delete，Add进新的sonication point
//    void refreshPlan(int angle,
//                     const QQueue<double>& x,
//                     const QQueue<double>& y,
//                     const QQueue<double>& z,
//                     const QQueue<QString>& idBorn,
//                     QMap<QString,bool>& flag);

    // 更新sonication参数
    void refreshIrradiatePara(const double valtage,
                              const int onTime,
                              const int offTime,
                              const int coolintTime,
                              const int pulseCount,
                              const QMap<QString,bool>& current,
                              const QMap<QString,bool>& total);

    void printfSpots();
    void getNextPlan(QQueue<Irradiate_Spot>& plan);
    void getDisplayPlan(QQueue<Irradiate_Spot>& plan);
    bool getIrradiateSpot(QMap<QString,bool>& flag,Irradiate_Spot& spot);
    bool getDisplayAngle(int& angle);
    bool getCoolingTime(int& second);
    int getDisplaySize();

    void updatePlan(const Irradiate_Spot& spot);

    QQueue<Irradiate_Spot> getTotalPlan();

public slots:
    void showNextPlan();

    void itemChanged(QStandardItem*);

    void copySpots(QVector<QString>& idSet);

    void deleteSpots(QVector<QString>& idSet);

    void refreshDisplayPlan(const QVector<QString>& ids);

private:
    // 根据旧ID创建一个新的sonication pointID
    QString createNewId(QString id);
    int getAngle(double x,double y);
    Layer getLayer(double z);

    Circle getCircle(double x,double y);

    void sortSpots(Irradiate_model model);

    Layer _layer;    // 记录Current Plansonication的层
    // int   _angle;    // 记录Current Plansonication的Angle

    QQueue<Irradiate_Spot> _plan;
    QQueue<Irradiate_Spot> _displayPlan;
};

#endif // IRRADIATEPLAN_H_

