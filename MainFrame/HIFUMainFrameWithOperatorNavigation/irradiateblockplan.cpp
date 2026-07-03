#include "irradiateblockplan.h"
#include <QtCore/qmath.h>

#include "qdebug.h"

#define PI 3.14159265

BlockPlan::BlockPlan()
{
}

void BlockPlan::setBlocks(const QQueue<Irradiate_Block>& blocks)
{
    m_blocks.clear();
    for (QQueue<Irradiate_Block>::const_iterator iter = blocks.begin();
         iter != blocks.end(); ++iter)
    {
        m_blocks.push_back(*iter);
    }
}

bool BlockPlan::getNextBlock(const QMap<QString,bool>& complete,
                             Irradiate_Block& block)
{
    if (m_blocks.empty())
    {
        return false;
    }

    for (QQueue<Irradiate_Block>::iterator iter = m_blocks.begin();
         iter != m_blocks.end();++iter)
    {
        // 按顺序找到了未sonication的点
        if (complete.find(iter->id)==complete.end())
        {
            block = *iter;
            return true;
        }
    }

    return false;
}

// 根据fibroid直径和深度GenerateTreatment盘偏转Angle
void BlockPlan::generateBlocks(int size,int distance)
{
    // Negative forward, positive back，Negative left, positive right
    Irradiate_Block block;
    double radius;
    switch (size)
    {
        case 3:
            m_blocks.clear();
            radius = 277.0;
            block.id = "1";
            block.leftRight = 0;
            block.frontBack = -1.0*qAtan(6.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "2";
            block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = qAtan(3.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "3";
            block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = qAtan(3.0/radius)*180.0/PI;
            m_blocks.push_back(block);
        break;

    case 4:
            m_blocks.clear();
            radius = 282.0;
            block.id = "1";
            block.leftRight = 0;
            block.frontBack = -1.0*qAtan(6.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "2";
            block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = qAtan(3.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "3";
            block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = qAtan(3.0/radius)*180.0/PI;
            m_blocks.push_back(block);
        break;

    case 5:
        m_blocks.clear();
        radius = 277.0;
        block.id = "1";
        block.leftRight = qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        block.id = "2";
        block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "3";
        block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "4";
        block.leftRight = -1.0*qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        block.id = "5";
        block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "6";
        block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "7";
        block.leftRight = 0.0;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        break;

    case 6:
            m_blocks.clear();
            radius = 277.0;
            block.id = "1";
            block.leftRight = qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = 0.0;
            m_blocks.push_back(block);
            block.id = "2";
            block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "3";
            block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "4";
            block.leftRight = -1.0*qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = 0.0;
            m_blocks.push_back(block);
            block.id = "5";
            block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = qAtan(9.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "6";
            block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
            block.frontBack = qAtan(9.0/radius)*180.0/PI;
            m_blocks.push_back(block);
            block.id = "7";
            block.leftRight = 0.0;
            block.frontBack = 0.0;
            m_blocks.push_back(block);
        break;

    case 7:
        m_blocks.clear();
        radius = 277.0;
        block.id = "1";
        block.leftRight = qAtan(9.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        block.id = "2";
        block.leftRight = qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "3";
        block.leftRight = 0.0;
        block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "4";
        block.leftRight = -1.0*qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "5";
        block.leftRight = -1.0*qAtan(9.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        block.id = "6";
        block.leftRight = -1.0*qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "7";
        block.leftRight = 0.0;
        block.frontBack = qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "8";
        block.leftRight = qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(9.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "9";
        block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        block.id = "10";
        block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = 0.0;
        m_blocks.push_back(block);
        break;

    case 8:
        m_blocks.clear();
        radius = 277.0;
        block.id = "1";
        block.leftRight = 0.0;
        block.frontBack = qAtan(12.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "2";
        block.leftRight = qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(12.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "3";
        block.leftRight = qAtan(12.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(12/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "4";
        block.leftRight = qAtan(9.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(3.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "5";
        block.leftRight = qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(6.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "6";
        block.leftRight = qAtan(3.0*sqrt(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(15.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "7";
        block.leftRight = 0.0;
        block.frontBack = -1.0*qAtan(24.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "8";
        block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(15/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "9";
        block.leftRight = -1.0*qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = -1.0*qAtan(6.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "10";
        block.leftRight = -1.0*qAtan(9.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(3.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "11";
        block.leftRight = -1.0*qAtan(12.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(12.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "12";
        block.leftRight = -1.0*qAtan(6.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(12.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "13";
        block.leftRight = 0;
        block.frontBack = -1.0*qAtan(6.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "14";
        block.leftRight = qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(3.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        block.id = "15";
        block.leftRight = -1.0*qAtan(3.0*sqrtf(3.0)/radius)*180.0/PI;
        block.frontBack = qAtan(3.0/radius)*180.0/PI;
        m_blocks.push_back(block);
        break;

    default:
        break;
    }

}

BlockPlan::~BlockPlan()
{

}
