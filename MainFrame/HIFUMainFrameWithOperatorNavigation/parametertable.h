#ifndef PARAMETERTABLE
#define PARAMETERTABLE

#include "commondefine.h"
#include <QMap>

class IrradiatePara
{
public:
    IrradiatePara();
    ~IrradiatePara();

    void initTable();

    int setPulseNumber(int number);

    bool getElement(int deep,Element& ele);

private:
    QMap<int,Element> m_tables;

};


#endif // PARAMETERTABLE

