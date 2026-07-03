

// wangwenbo create 20150405

#ifndef MCHASHTABLE_H_
#define MCHASHTABLE_H_


#include <QVector>

class SMCTriangleNetHashTable
{
public:

    SMCTriangleNetHashTable(int minx, int miny, int width, int height);
    void SetDefaultValue(int index0_1);

    void IncreaseIndex();

    void SetHashValue(int x, int y, int z, int value);

    int GetHashValue(int x, int y, int z);

    std::vector<std::vector<int> > mapList;

    int CurrentLayerIndex;

private:
    int stx;
    int sty;
    int width;
    int height;
};

#endif
