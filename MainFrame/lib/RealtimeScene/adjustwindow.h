#ifndef ADJUSTWINDOW_H_
#define ADJUSTWINDOW_H_

class AdjustWindow
{
public:
    AdjustWindow();
    ~AdjustWindow();

    void resetWindow(int deltaWW,int deltaWC);

    unsigned char* getTable(){return colorTable;}

    void resumeWindow();

private:
    void initColorTable();
    void adjust();
    unsigned char colorTable[256];
    int m_iWW;
    int m_iWC;

};

#endif // ADJUSTWINDOW_H_

