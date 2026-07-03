
#include "region_growing_data_struct.h"
#include "qdebug.h"

BitMap3d::BitMap3d(int width, int height,int depth, char v)
{
    this->width = width;
    this->height = height;
    this->depth = depth;
    data = new uchar[width * height * depth];
    memset(data,v,width*height*depth*sizeof(char));
}
BitMap3d::BitMap3d(uchar* data, int width, int height,int depth)
{
    this->data = data;
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->data = new uchar[width*height*depth];
    memcpy(this->data,data,width*height*depth*sizeof(uchar));
}

BitMap3d::~BitMap3d()
{
    if (data!=NULL)
    {
        delete []data;
        data = NULL;
    }
}

void BitMap3d::SetPixel(int x, int y,int z,char v)
{
    data[x + y * width+z*width*height] = v;
}

uchar BitMap3d::GetPixel(int x, int y,int z)
{
    return data[x + y * width+z*width*height];
}

void BitMap3d::ReadRaw(QString path)
{
//    FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read);
//    fs.Read(data, 0, width*height*depth);
//    fs.Close();
    //FILE* file = fopen(path.);
}

void BitMap3d::SaveRaw(QString path)
{
//    FileStream fs = new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write);
//    fs.Write(data, 0, data.Length);
//    fs.Close();
}
