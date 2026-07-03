
#include "region_growing.h"
#include "qdebug.h"

SpanFill3d::SpanFill3d(uchar* data,int width,int height,int depth, uchar up,uchar low )
: bmp(data,width,height,depth)
, flagsMap(FlagMap3d(width, height,depth))
, max(up)
, min(low)
{
    pData = new uchar[width*height*depth];
    memset(pData,0,width*height*depth);
}

SpanFill3d::~SpanFill3d()
{
    if (pData!=NULL)
    {
        delete []pData;
        pData = NULL;
    }
}

void SpanFill3d::ExcuteSpanFill(Int16Triple seed)
{
    qDebug()<<"Enter excute span !";
    Process(seed);
    flagsMap.SetFlagOn(seed.x, seed.y,seed.z, true);
    Span seedspan = Span();
    seedspan.XLeft = seed.x;
    seedspan.XRight = seed.x;
    seedspan.Y = seed.y;
    seedspan.Z = seed.z;
    seedspan.ParentDirection = Non;
    seedspan.Extended = UnRez;
    container.push_back(seedspan);

    qDebug()<<"Enter loop seed !";
    qDebug()<<"container size is "<<container.size();
    while (!container.empty())
    {
        Span span = container.head();
        container.pop_front();

        // #region AllRez
        if (span.Extended == AllRez)
        {
            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)//[spx-spy,y-1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Z - 1 >= 0)//[spx-spy,y,z-1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)//[spx-spy,y,z+1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Y0)
            {
                if (span.Y + 1 < bmp.height)//[spx-spy,y+1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 > 0)//[spx-spy,y,z-1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)//[spx-spy,y,z+1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)//[spx-spy,y-1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)//[spx-spy,y+1,z]
                    CheckRange(span.XLeft, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)//[spx-spy,y,z-1]
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z - 1, Z2);
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(span.XLeft, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(span.XLeft, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            // throw new Exception();
        }
        // #endregion
        // #region UnRez
        if (span.Extended == UnRez)
        {
            int xl = FindXLeft(span.XLeft, span.Y,span.Z);
            int xr = FindXRight(span.XRight, span.Y,span.Z);
            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y + 1, span.Z, Y0);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y + 1, span.Z, Y0);
                }
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Y0)
            {
                if (span.Y + 1 < bmp.height)
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
                if (span.Z + 1 < bmp.depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y, span.Z+1, Z0);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y, span.Z+1, Z0);
                }
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                {
                    if (xl != span.XLeft)
                        CheckRange(xl, span.XLeft, span.Y, span.Z -1, Z2);
                    if (span.XRight != xr)
                        CheckRange(span.XRight, xr, span.Y, span.Z - 1, Z2);
                }
                if (span.Z + 1 < bmp.depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Non)
            {
                if (span.Y + 1 < bmp.height)
                    CheckRange(xl, xr, span.Y + 1, span.Z, Y0);
                if (span.Y - 1 >= 0)
                    CheckRange(xl, xr, span.Y - 1, span.Z, Y2);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(xl, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            // throw new Exception();
        }

        // #endregion
        // #region LeftRequired

        if (span.Extended == LeftRequired)
        {
            int xl = FindXLeft(span.XLeft, span.Y,span.Z);
            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y , span.Z-1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(xl, span.XRight, span.Y , span.Z+1, Z0);
                continue;
            }
            if (span.ParentDirection == Y0)
            {
                if (span.Y - 1 >= 0 && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(xl, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(xl, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0 )
                    CheckRange(xl, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(xl, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(xl, span.XRight, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(xl, span.XRight, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0 && xl != span.XLeft)
                    CheckRange(xl, span.XLeft, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth )
                    CheckRange(xl, span.XRight, span.Y, span.Z + 1, Z0);
                continue;
            }
            // throw new Exception();
        }
        // #endregion
        // #region RightRequired
        if (span.Extended == RightRequired)
        {
            int xr = FindXRight(span.XRight, span.Y,span.Z);

            if (span.ParentDirection == Y2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y - 1,span.Z, Y2);
                if (span.Y + 1 < bmp.height && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y + 1,span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y, span.Z-1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(span.XLeft, xr, span.Y, span.Z+1, Z0);
                continue;
            }

            if (span.ParentDirection == Y0)
            {
                if (span.Y + 1 < bmp.height)
                    CheckRange(span.XLeft, xr, span.Y + 1, span.Z, Y0);
                if (span.Y - 1 >= 0 && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y - 1, span.Z, Y2);
                if (span.Z - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(span.XLeft, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z2)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(span.XLeft, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            if (span.ParentDirection == Z0)
            {
                if (span.Y - 1 >= 0)
                    CheckRange(span.XLeft, xr, span.Y - 1, span.Z, Y2);
                if (span.Y + 1 < bmp.height)
                    CheckRange(span.XLeft, xr, span.Y + 1, span.Z, Y0);
                if (span.Z - 1 >= 0 && span.XRight != xr)
                    CheckRange(span.XRight, xr, span.Y, span.Z - 1, Z2);
                if (span.Z + 1 < bmp.depth)
                    CheckRange(span.XLeft, xr, span.Y, span.Z + 1, Z0);
                continue;
            }
            // throw new Exception();
        }
        // #endregion
    }
}

void SpanFill3d::CheckRange(int xleft, int xright, int y, int z,ParentDirections ptype)
{
    for (int i = xleft; i <= xright; )
    {
        if ((!flagsMap.GetFlagOn(i, y,z)) && IncludePredicate(i, y,z))
        {
            int lb = i;
            int rb = i + 1;
            while (rb <= xright && (!flagsMap.GetFlagOn(rb, y,z)) && IncludePredicate(rb, y,z))
            {
                rb++;
            }
            rb--;

            Span span = Span();
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
                flagsMap.SetFlagOn(j, y,z, true);
                Process(Int16Triple(j, y,z));
            }
            container.push_back(span);

            i = rb + 1;
        }
        else
        {
            i++;
        }
    }
}//区段法的CheckRange 注意与扫描线的CheckRange的不同

int SpanFill3d::FindXRight(int x, int y,int z)
{
    int xright = x + 1;
    while (true)
    {
        if (xright == bmp.width || flagsMap.GetFlagOn(xright, y,z))
        {
            break;
        }
        else
        {
            if (IncludePredicate(xright, y,z))
            {
                Int16Triple t = Int16Triple(xright, y,z);
                flagsMap.SetFlagOn(xright, y,z ,true);
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

int SpanFill3d::FindXLeft(int x, int y,int z)
{
    int xleft = x - 1;
    while (true)
    {
        if (xleft == -1 || flagsMap.GetFlagOn(xleft, y, z))
        {
            break;
        }
        else
        {
            if (IncludePredicate(xleft, y,z))
            {
                Int16Triple t = Int16Triple(xleft, y,z);
                flagsMap.SetFlagOn(xleft, y,z, true);
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

bool SpanFill3d::IncludePredicate(int x, int y, int z)
{
    uchar v = bmp.GetPixel(x, y, z);
    return v > min && v < max;
}

void SpanFill3d::Process(Int16Triple p)
{
    pData[p.z*bmp.width*bmp.height+p.y*bmp.width+p.x] = 255;
}

uchar* SpanFill3d::GetResult()
{
    return pData;
}
