#ifndef SEEDEDGROW3D_H
#define SEEDEDGROW3D_H

#include <QObject>
#include "base3d.h"

enum ParentDirections3d
{
    Y0 = 1,
    Y2 = 2,
    Z0 = 3,
    Z2 = 4,
    Non = 5
};
enum ExtendTypes3d
{
    LeftRequired = 1,
    RightRequired = 2,
    AllRez = 3,
    UnRez = 4
};
struct Span3d
{
    int XLeft;
    int XRight;
    int Y;
    int Z;
    ExtendTypes3d Extended;
    ParentDirections3d ParentDirection;
};

class SeededGrow3d : public QObject
{
    Q_OBJECT
public:
    explicit SeededGrow3d(QObject *parent = 0);
    ~SeededGrow3d();

    FlagMap3d *flagsMap;
    ResultReport3d report;
    short min;
    short max;

    virtual void ExcuteSpanFill(BitMap3d *data, Int16Triple seed,short* outimage);

signals:

public slots:

protected:
    int count;
    BitMap3d *bmp;
    Container3d_Stack<Span3d> *container;//以Span为单位的Queue或Stack容器

private:
    virtual void CheckRange(int xleft, int xright, int y, int z, ParentDirections3d ptype);
    virtual int FindXRight(int x, int y, int z);
    virtual int FindXLeft(int x, int y, int z);
    virtual bool IncludePredicate(int x, int y, int z);
    virtual void Process(Int16Triple p);
};

#endif // SEEDEDGROW3D_H
