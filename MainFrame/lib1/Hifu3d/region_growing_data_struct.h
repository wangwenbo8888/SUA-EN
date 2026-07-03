#ifndef SEED_DATA_STRUCT
#define SEED_DATA_STRUCT

#include <QString>
#include "hifu3d_global.h"

const char WHITE = 255;
const char BLACK = 0;

 struct Int16Triple
 {
     Int16Triple(int x, int y,int z)
     {
        this->x = x;
        this->y = y;
        this->z = z;
    }

     Int16Triple()
     {
         this->x = 0;
         this->y = 0;
         this->z = 0;
     }

    int x;
    int y;
    int z;
};

class HIFU3D_EXPORT BitMap3d
{
public:
        BitMap3d(int width, int height,int depth, char v);

        BitMap3d(uchar* data, int width, int height,int depth);

        ~BitMap3d();

        void SetPixel(int x, int y,int z,char v);


        uchar GetPixel(int x, int y,int z);

        void ReadRaw(QString path);

        void SaveRaw(QString path);

        uchar* data;
        int width;
        int height;
        int depth;
};

class HIFU3D_EXPORT FlagMap3d
{
public:
    int width;
    int height;
    int depth;
    char* flags;

    FlagMap3d(int width, int height,int depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;
        flags = new char[width * height*depth];
        memset(flags,0,width*height*depth);
    }

    ~FlagMap3d()
    {
        delete []flags;
    }

    void SetFlagOn(int x, int y, int z,bool v)
    {
        flags[x + y * width+z*width*height] = v;
    }

    bool GetFlagOn(int x, int y,int z)
    {
        return flags[x + y * width+z*width*height];
    }

};

#endif // SEED_DATA_STRUCT

