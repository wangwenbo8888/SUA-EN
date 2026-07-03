
#include "adjustwindow.h"
#include "qdebug.h"

AdjustWindow::AdjustWindow()
{
    m_iWC = 128;
    m_iWW = 255;
    initColorTable();
}

AdjustWindow::~AdjustWindow()
{
}

void AdjustWindow::resumeWindow()
{
    m_iWC = 128;
    m_iWW = 255;
    adjust();
}

void AdjustWindow::resetWindow(int deltaWW,int deltaWC)
{
    int tempWW = m_iWW - deltaWW;
    int tempWC = m_iWC - deltaWC;
    if (tempWW<255&&tempWW > 1 )
    {
        m_iWW = tempWW;
    }

    if (tempWC < 255 && tempWC>1)
    {
        m_iWC = tempWC;
    }

    m_iWC = tempWC;
    m_iWW = tempWW;
    adjust();
}

void AdjustWindow::initColorTable()
{
    for (int i = 0; i < 256;++i)
    {
        colorTable[i] = i;
    }
}

void AdjustWindow::adjust()
{
    if (m_iWW>255 || m_iWW<1)
    {
        return;
    }

    if (m_iWC>255 || m_iWC<1)
    {
        return;
    }

    initColorTable();
    int min = (2*m_iWC - m_iWW)/2.0 + 0.5;
    int max = (2*m_iWC + m_iWW)/2.0 + 0.5;

    for (int i = 0; i < min;++i)
    {
        colorTable[i] = 0;
    }

    for (int i = min; i <= max; i++)
    {
        colorTable[i] = (colorTable[i] - min)*255.0/(double)(max - min);
    }

    for (int i = max +1;i<256;++i)
    {
        colorTable[i] = 0;
    }

}
