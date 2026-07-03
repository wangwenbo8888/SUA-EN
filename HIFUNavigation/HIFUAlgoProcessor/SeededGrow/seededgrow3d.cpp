#include "seededgrow3d.h"
#include <QDebug>

SeededGrow3d::SeededGrow3d(QObject *parent)
    : QObject(parent),flagsMap(NULL),bmp(NULL),count(0),container(NULL)
{

}

SeededGrow3d::~SeededGrow3d()
{
    if (container!=NULL)
    {
        delete container;
        container = NULL;
    }

    if (flagsMap!=NULL)
    {
        delete flagsMap;
        flagsMap = NULL;
    }
}

void SeededGrow3d::ExcuteSpanFill(BitMap3d * data, Int16Triple seed,uchar* outimage)
{
    container = new Container3d_Stack<Span3d>();
    this->bmp = data;
    bmp->ResetVisitCount();
    flagsMap = new FlagMap3d(bmp->width, bmp->height, bmp->depth,outimage);   
    Process(seed);
    flagsMap->SetFlagOn(seed.X, seed.Y, seed.Z, true);
    Span3d seedspan;
    seedspan.XLeft = seed.X;
    seedspan.XRight = seed.X;
    seedspan.Y = seed.Y;
    seedspan.Z = seed.Z;
    seedspan.ParentDirection = Non;
    seedspan.Extended = UnRez;
    container->Push(seedspan);

    while (!container->Empty())
    {
        Span3d span = container->Pop();
        //*****************AllRez
        if (span.Extended == AllRez)
        {
            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)//[spx-spy,y-1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Z - 1 >= 0)//[spx-spy,y,z-1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < data->depth)//[spx-spy,y,z+1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Y0)
            {
                if (span.Y + 1 < bmp->height)//[spx-spy,y+1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)//[spx-spy,y,z-1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < data->depth)//[spx-spy,y,z+1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)//[spx-spy,y-1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)//[spx-spy,y+1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)//[spx-spy,y,z-1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z - 1, Z2);
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(span.XLeft, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z + 1 < data->depth)
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            //throw Exception();
        }

        //*****************UnRez
        if (span.Extended == UnRez)
        {
            int xl = FindXLeft(span.XLeft, span.Y, span.Z);
            int xr = FindXRight(span.XRight, span.Y, span.Z);
            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y + 1, span.Z, Y0);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y + 1, span.Z, Y0);
                }
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);

                continue;
            }
            if (span.ParentDirection == Y0)
            {
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Y - 1 >= 0)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y - 1, span.Z, Y2);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y - 1, span.Z, Y2);
                }
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y, span.Z + 1, Z0);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y, span.Z + 1, Z0);
                }
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y, span.Z - 1, Z2);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y, span.Z - 1, Z2);
                }
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Non)
            {
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            //throw Exception();
        }

        //*****************LeftRequired
        if (span.Extended == LeftRequired)
        {
            int xl = FindXLeft(span.XLeft, span.Y, span.Z);
            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Y0)
            {
                if (span.Y - 1 >= 0 && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(xl, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0 && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(xl, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            //throw Exception();
        }

        //*****************RightRequired
        if (span.Extended == RightRequired)
        {
            int xr = FindXRight(span.XRight, span.Y, span.Z);

            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(span.XLeft, xr, span.Y, span.Z + 1, Z0);
                continue;
            }

            if (span.ParentDirection == Y0)
            {
                if (span.Y + 1 < bmp->height)
                    CheckRange(span.XLeft, xr, span.Y + 1, span.Z, Y0);
                if (span.Y - 1 >= 0 && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y - 1, span.Z, Y2);
                if (span.Z - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(span.XLeft, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(span.XLeft, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp->height)
                    CheckRange(span.XLeft, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0 && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp->depth)
                    CheckRange(span.XLeft, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            //throw Exception();
        }

    }

////    int b = flagsMap->GetCount();
////#pragma omp parallel for
//    for (int iz = 0; iz < bmp->depth; iz++)
//    {
////#pragma omp parallel for
//        for (int ix = 0; ix < bmp->width; ix++)
//        {
//            for (int iy = 0; iy < bmp->height; iy++)
//            {
//                if (flagsMap->GetFlagOn(ix, iy, iz))
//                {
//                    bmp->SetPixel(ix, iy, iz, 1000);
//                }
//                else
//                    bmp->SetPixel(ix, iy, iz,0);
//            }
//        }
//    }
}

void SeededGrow3d::CheckRange(int xleft, int xright, int y, int z, ParentDirections3d ptype)
{
    for (int i = xleft; i <= xright; )
    {
        if ((!flagsMap->GetFlagOn(i, y, z)) && IncludePredicate(i, y, z))
        {
            int lb = i;
            int rb = i + 1;
            while (rb <= xright && (!flagsMap->GetFlagOn(rb, y, z)) && IncludePredicate(rb, y, z))
            {
                rb++;
            }
            rb--;

            Span3d span;
            span.XLeft = lb;
            span.XRight = rb;
            span.Y = y;
            span.Z = z;
            if (lb == xleft && rb == xright)
            {
                span.Extended = UnRez;
            }
            else if (rb == xright)
            {
                span.Extended = RightRequired;
            }
            else if (lb == xleft)
            {
                span.Extended = LeftRequired;
            }
            else
            {
                span.Extended = AllRez;
            }
            span.ParentDirection = ptype;
            for (int j = lb; j <= rb; j++)
            {
                flagsMap->SetFlagOn(j, y, z, true);
                Process(Int16Triple(j, y, z));
            }
            container->Push(span);

            i = rb + 1;
        }
        else
        {
            i++;
        }
    }
}

int SeededGrow3d::FindXRight(int x, int y, int z)
{
    int xright = x + 1;
    while (true)
    {
        if (xright == bmp->width || flagsMap->GetFlagOn(xright, y, z))
        {
            break;
        }
        else
        {
            if (IncludePredicate(xright, y, z))
            {
                Int16Triple t(xright, y, z);
                flagsMap->SetFlagOn(xright, y, z, true);
                Process(t);
                xright++;
            }
            else
            {
                break;
            }
        }
    }
    return xright - 1;
}

int SeededGrow3d::FindXLeft(int x, int y, int z)
{
    int xleft = x - 1;
    while (true)
    {
        if (xleft == -1 || flagsMap->GetFlagOn(xleft, y, z))
        {
            break;
        }
        else
        {
            if (IncludePredicate(xleft, y, z))
            {
                Int16Triple t(xleft, y, z);
                flagsMap->SetFlagOn(xleft, y, z, true);
                Process(t);
                xleft--;
            }
            else
            {
                break;
            }
        }
    }
    return xleft + 1;
}

bool SeededGrow3d::IncludePredicate(int x, int y, int z)
{
    short v = bmp->GetPixel(x, y, z);

    return v > min && v <= max;
}

void SeededGrow3d::Process(Int16Triple p)
{
    count++;
}

