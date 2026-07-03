#ifndef BASE3D
#define BASE3D

#include <QObject>
#include <QBitArray>
#include <QStack>

struct Int16Triple
{
public:
    int X;
    int Y;
    int Z;
    Int16Triple(int x, int y, int z)
    {
        X = x;
        Y = y;
        Z = z;
    }
    Int16Triple()
    {
        X = 0;
        Y = 0;
        Z = 0;
    }
};

class BitMap3d
{
public:
    static const short WHITE = 3000;
    static const short BLACK = -1021;
    short *data;
    int width;
    int height;
    int depth;
    int action_set_count;
    int action_get_count;
    BitMap3d(int iwidth, int iheight, int idepth, unsigned char v)
    {
        width = iwidth;
        height = iheight;
        depth = idepth;
        data = new short[width * height*depth];
        action_set_count = 0;
        action_get_count = 0;
        for (int i = 0; i < width * height*depth; i++)
            data[i] = v;
    }
    BitMap3d(short *idata, int iwidth, int iheight, int idepth)
    {
        data = idata;
        width = iwidth;
        height = iheight;
        depth = idepth;
    }
    void SetPixel(int x, int y, int z, short v)
    {
        action_set_count++;
        data[x + y * width + z*width*height] = v;
    }
    short GetPixel(int x, int y, int z)
    {
        action_get_count++;
        return data[x + y * width + z*width*height];
    }
    void ResetVisitCount()
    {
        action_get_count = 0;
        action_set_count = 0;
    }

    void ReadBitmap(QString path)
    {
        /*FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read);
            fs.Read(data, 0, width*height*depth);
            fs.Close();*/
    }
    void SaveRaw(QString path)
    {
        /*FileStream fs = new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write);
            fs.Write(data, 0, data.Length);
            fs.Close();*/
    }
};

class FlagMap3d
{
public:
    int action_set_count;
    int action_get_count;
    int width;
    int height;
    int depth;
    short* outimagedata;
    QBitArray flags;
    FlagMap3d(int iwidth, int iheight, int idepth,short* outdata)
    {
        width = iwidth;
        height = iheight;
        depth = idepth;
        action_get_count = 0;
        action_set_count = 0;
        flags.resize(width * height*depth);
        outimagedata=outdata;
        for (int i = 0; i < flags.size(); i++)
        {
            flags.clearBit(i);
            outimagedata[i]=0;
        }
    }
    void SetFlagOn(int x, int y, int z, bool v)
    {
        flags.setBit(x + y*width + z*width*height, v);
        //flags[x + y * width] = v;
        if(v)
            outimagedata[x + y*width + z*width*height]=1;
        else
            outimagedata[x + y*width + z*width*height]=0;
        action_set_count++;
    }
    bool GetFlagOn(int x, int y, int z)
    {
        action_get_count++;
        return flags.at(x + y * width + z*width*height);
    }

    int GetCount()
    {
        int sum = 0;
        for (int i = 0; i < flags.size(); i++)
        {
            if (flags.at(i))
                sum++;
        }
        return sum;
    }
};

template<class T>
class Container3d
{
public:
    int action_push_count;
    int action_pop_count;
    int max_contain_count;
    Container3d()
    {
        max_contain_count = 0;
        action_pop_count = 0;
        action_push_count = 0;
    }
    virtual void Push(T v)
    {
    }
    virtual T Pop()
    {
        T t;
        return t;
    }
    virtual bool Empty()
    {
        return false;
    }
};

template <class T>
class Container3d_Stack :Container3d<T>
{
public:
    QStack<T> stack;
    Container3d_Stack()
    {
        stack.clear();
    }
    void Push(T v)
    {
        //action_push_count++;
//        if (stack.count() > max_contain_count)
//            max_contain_count = stack.count();
        stack.push(v);
    }
    T Pop()
    {
       // action_pop_count++;
        T value = stack.pop();
        return value;
    }
    bool Empty()
    {
        return stack.count() == 0;
    }
};

struct ResultReport3d
{
    int result_point_count;
    int flag_set_count;
    int flag_get_count;
    int bmp_get_count;
    int bmp_set_count;
    int container_push_count;
    int container_pop_count;
    int container_max_size;
    long time;
    void PrintInfo()
    {

    }

    void OutPutMap(FlagMap3d *flagsmap, QString path)
    {
        /*QImage *img = new QImage(flagsmap->width, flagsmap->height, QImage::Format::Format_RGB888);
            for (int i = 0; i < flagsmap->width; i++)
            {
                for (int j = 0; j < flagsmap->height; j++)
                {
                    uchar s = flagsmap->GetFlagOn(i, j);
                    if (s)
                        img->setPixelColor(i, j, QColor::fromRgb(255, 255, 255));
                    else
                        img->setPixelColor(i, j, QColor::fromRgb(0, 0, 0));
                }
            }
            img->save(path);
            delete img;
            img = NULL;*/
    }
};

#endif // BASE3D

