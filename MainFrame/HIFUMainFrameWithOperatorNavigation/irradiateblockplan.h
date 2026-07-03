#ifndef IRRADIATEBLOCKPLAN_H_
#define IRRADIATEBLOCKPLAN_H_

#include <QQueue>
#include <QMap>
#include "irradiateblock.h"

class BlockPlan
{
public:
    BlockPlan();
    void generateBlocks(int size,int distance);

    // 获取下一个block位置
    bool getNextBlock(const QMap<QString,bool>& complete,Irradiate_Block& block);

    void setBlocks(const QQueue<Irradiate_Block>& blocks);

    QQueue<Irradiate_Block>& getBlocks()
    {
        return m_blocks;
    }
    ~BlockPlan();

private:
    QQueue<Irradiate_Block> m_blocks;

};

#endif // IRRADIATEBLOCKPLAN_H_

