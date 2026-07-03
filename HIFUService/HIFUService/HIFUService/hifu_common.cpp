

#include "include/hifu_common.h"

#include <sstream>
#include <string>

int strToInt(const char* p)
{
    int num;
    std::stringstream ss(p);
    ss >> num;
    return num;
}

int getFileSize(FILE* file)
{
    if (file==NULL)
    {
        return -1;
    }

    FILE* temp = file;

    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    file = temp;

    return size;

}