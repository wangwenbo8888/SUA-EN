#ifndef GENERATEOUTLINE_H_
#define GENERATEOUTLINE_H_

#include "drawscene.h"

#include "qloggingcategory.h"

Q_DECLARE_LOGGING_CATEGORY(GENERATEOUTLINE)

class GenerateOutline
{
public :
    GenerateOutline(DrawScene* item0,DrawScene* item1);
    ~GenerateOutline();

    bool getIntersectItem(QGraphicsItem*& item);

private :
    DrawScene* m_scene0;
    DrawScene* m_scene1;

};

#endif // GENERATEOUTLINE_H_

