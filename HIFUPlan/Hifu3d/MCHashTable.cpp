

// wangwenbo cteate 20150405

#include "MCHasTable.h"
#include "qdebug.h"


SMCTriangleNetHashTable::SMCTriangleNetHashTable(int minx, int miny, int width, int height)
{

    this->stx = minx - 1;
    this->sty = miny - 1;
    this->width = width + 2;
    this->height = height + 2;
    mapList.resize(2);
    CurrentLayerIndex= 0;

    mapList.data()[0].resize(this->width*this->height,-1);
    mapList.data()[1].resize(this->width*this->height,-1);
}

void SMCTriangleNetHashTable::SetDefaultValue(int index0_1)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            mapList[index0_1][i+j*width] = -1;
        }
    }
}

void SMCTriangleNetHashTable::IncreaseIndex()
{
    CurrentLayerIndex++;
    SetDefaultValue(0);
    std::vector<int> temp = mapList[0];
    mapList[0] = mapList[1];
    mapList[1] = temp;
}

void SMCTriangleNetHashTable::SetHashValue(int x, int y, int z, int value)
{
    int index0_1 = z - CurrentLayerIndex;
    mapList[index0_1][x - stx + (y - sty)*width] = value;
}

int SMCTriangleNetHashTable::GetHashValue(int x, int y, int z)
{
    int index0_1 = z - CurrentLayerIndex;
    return mapList[index0_1][x - stx + (y - sty)*width];
}
