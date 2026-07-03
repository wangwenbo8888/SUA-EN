#ifndef MODELMANAGER
#define MODELMANAGER

#include "QObject"
#include "gl.h"
#include "math.h"

class Mesh;

// 三维显示模型管理

struct model
{
    GLuint* trisIndex;        // 模型三角形索引数组首地址
    GLfloat* colorIndexs;     // 模型颜色索引首地址
    GLfloat* versPos;         // 模型位置数组首地址

    GLuint triSize;           // 模型三角形个数
    GLuint verSize;           // 模型顶点个数
};

class ModelManager : public QObject
{
    Q_OBJECT

public:
    explicit ModelManager(Mesh* pMyoma);
    ~ModelManager();

    void setModelColor(int index);    // 设置辐照点颜色

    GLuint* getTrisIndex();          // 三角形顶点索引数组首地址
    GLfloat* getColorIndexs();       // 三角形顶点颜色数值首地址
    GLfloat* getVersPos();           // 三角形顶点位置首地址

    int getVerSize();
    int getTrisIndexSize();
    int getColorsLen();

private :

    void initScene();                // 初始化可视化窗口场景
    void createPlane(float wide, float heigh,Mesh* spot);
    void createSpot(Mesh* spot);
    void loadWomanModel(Mesh* pHuman);
    void addModel(Mesh* pModel,GLfloat* matrix);

    Mesh* _pMyoma;                   // 肌瘤模型
    GLuint* _trisIndex;
    GLfloat* _colorIndexs;
    GLfloat* _versPos;
    int _headerVerIndex;
    int _headerTrisIndex;

    int _versSize;
    int _trisIndexSize;
    int _colorsLen;


};

#endif // MODELMANAGER

