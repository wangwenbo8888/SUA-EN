#ifndef Scene3d_H_
#define Scene3d_H_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "commondefine.h"

class Mesh;
class ModelManager;

class Scene3d : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:

    Scene3d(QMap<QString,Spot_3d_Status>& status,QWidget *parent = 0);
    ~Scene3d();

    void initializeGL();                ///< 初始化
    void resizeGL(int w, int h);        ///< 当窗口发生变化时重新初始化
    void paintGL();                     ///< 绘制

    void destorySpots();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

private slots :
    void setSpotColor(QString id,QColor color);

private:


    void createSpotsModel();

    void createPlane();    // 创建辐照平面
    void initPlane();
    void setHumanModel();
    void setMyomaModel();
    void setSpots();
    void addSpotNew(GLfloat* matrix,Spot_3d_Status& pStatus);

    Mesh* _pHumanModel;
    Mesh* _pMyoma;
    ModelManager* _pModelManager;

    QMap<QString,Spot_3d_Status>& _status;

    QPoint lastPos;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat zoom;
    GLfloat scal;

    Mesh* _pSpot;
};

#endif // Scene3d_H_
