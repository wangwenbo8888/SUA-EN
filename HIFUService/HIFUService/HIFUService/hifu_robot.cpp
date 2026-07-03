
#include <aclapi.h>

#include "include/hifu_robot.h"

HIFURobot::HIFURobot()
{
}

HIFURobot::~HIFURobot()
{
}

bool HIFURobot::setPos(int axis, int pos)
{
    HWND hDeCode = ::FindWindow(NULL, DECODE_PROGRAMM);
    if (hDeCode != NULL)
    {
        ::PostMessage(hDeCode, WM_USER+777, (WPARAM)axis, (LPARAM)pos);
        return true;
    }
    else
    {
        return false;
    }
}

int HIFURobot::getPos(int axis)
{
    return 0;
}