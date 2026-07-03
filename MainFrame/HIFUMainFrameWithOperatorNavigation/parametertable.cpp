
#include "parametertable.h"

IrradiatePara::IrradiatePara()
{
    initTable();
}

IrradiatePara::~IrradiatePara()
{
}

int IrradiatePara::setPulseNumber(int number)
{
    QMap<int,Element>::iterator iter =  m_tables.begin();
    for (;iter != m_tables.end(); ++iter)
    {
        iter->pulse_number = number;
    }
    return number;
}

bool IrradiatePara::getElement(int deep,Element& ele)
{
    QMap<int,Element>::iterator iter = m_tables.find(deep);
    if (iter==m_tables.end())
    {
        return false;
    }
    else
    {
        ele = iter.value();
        return true;
    }
}

void IrradiatePara::initTable()
{
    m_tables.clear();
    Element ele;
//    ele.voltage = 7;
//    ele.on_time = 600;
//    ele.off_time = 100;
//    ele.pulse_number = 16;
//    ele.cooling_time = 60;
//    m_tables.insert(2,ele);

    ele.voltage = 12;
    ele.on_time = 400;
    ele.off_time = 100;
    ele.pulse_number = 6;
    ele.cooling_time = 20;
    m_tables.insert(3,ele);

    //ele.deep = 4;
//    ele.voltage = 9;
//    ele.on_time = 500;
//    ele.off_time = 200;
//    ele.pulse_number = 16;
//    ele.cooling_time = 60;
//    m_tables.insert(4,ele);

    // ele.deep = 5;
    ele.voltage = 15;
    ele.on_time = 400;
    ele.off_time = 100;
    ele.pulse_number = 6;
    ele.cooling_time = 20;
    m_tables.insert(5,ele);

    // ele.deep = 6;
//    ele.voltage = 11;
//    ele.on_time = 600;
//    ele.off_time = 200;
//    ele.pulse_number = 16;
//    ele.cooling_time = 100;
//    m_tables.insert(6,ele);

    // ele.deep = 7;
    ele.voltage = 17;
    ele.on_time = 400;
    ele.off_time = 100;
    ele.pulse_number = 6;
    ele.cooling_time = 20;
    m_tables.insert(7,ele);

    // ele.deep = 8;
//    ele.voltage = 12;
//    ele.on_time = 700;
//    ele.off_time = 300;
//    ele.pulse_number = 16;
//    ele.cooling_time = 180;
//    m_tables.insert(8,ele);

//    ele.voltage = 12;
//    ele.on_time = 800;
//    ele.off_time = 200;
//    ele.pulse_number = 16;
//    ele.cooling_time = 180;
//    m_tables.insert(9,ele);

}

