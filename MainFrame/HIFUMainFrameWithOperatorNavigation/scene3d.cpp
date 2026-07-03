#include "Scene3d.h"
#include "mesh.h"
#include <QMouseEvent>
#include "modelmanager.h"
#include <glu.h>
#include <qDir>

#define PI 3.14159265

Scene3d::Scene3d(QMap<QString,Spot_3d_Status>& status,QWidget *parent)
: _status(status)
  , QOpenGLWidget(parent)
{
    ///< 官方文档有这样设置，具体还没细细看，但是意思吧，就是告诉渲染属性，使用版本等；有空可以深入研究，光看效果不一定能看出什么区别!
    ///< 这个有些是放到main.cpp中去了，通过new widget.setFormat(format)那样去设置，不清楚...我觉得本质是一样，都是给该widget设置属性。
    this->setGeometry(0,0,486,328);

    _pSpot = new Mesh;
    _pSpot->ReadStl("../resources/spot.stl");
    createSpotsModel();
    setHumanModel();
    setMyomaModel();

    rotationX = 0.0f;
    rotationY = 0.0f;
    rotationZ = 0.0f;
    zoom = -63.0;
    scal = 1.0;
}

Scene3d::~Scene3d()
{
    if (_pSpot!=NULL)
    {
        delete _pSpot;
        _pSpot = NULL;
    }

    if (_pHumanModel!=NULL)
    {
        delete _pHumanModel;
        _pHumanModel = NULL;
    }

    if (_pMyoma!=NULL)
    {
        delete _pMyoma;
        _pMyoma = NULL;
    }

    destorySpots();
}

void Scene3d::addSpotNew(GLfloat* matrix,Spot_3d_Status& pStatus)
{
    Mesh* mesh = new Mesh;
    std::vector<point3d>& vers = _pSpot->m_vVertices;
    std::vector<Triangle>& tris = _pSpot->m_vTriangles;
    size_t verSize = vers.size();
    point3d temp;
    for (size_t i = 0; i < verSize; ++i)
    {
        temp.x = matrix[0]*vers[i].x+matrix[1]*vers[i].y+matrix[2]*vers[i].z+matrix[3];
        temp.y = matrix[4]*vers[i].x+matrix[5]*vers[i].y+matrix[6]*vers[i].z+matrix[7];
        temp.z = matrix[8]*vers[i].x+matrix[9]*vers[i].y+matrix[10]*vers[i].z+matrix[11];
        mesh->m_vVertices.push_back(temp);
    }

    mesh->m_vTriangles = tris;
    mesh->CalcTrisNormal();
    pStatus.pMesh = mesh;

    pStatus.r = pStatus.color.red()/255.0;
    pStatus.g = pStatus.color.green()/255.0;
    pStatus.b = pStatus.color.blue()/255.0;
}

void Scene3d::setSpotColor(QString id,QColor color)
{
    QMap<QString,Spot_3d_Status>::iterator iter = _status.find(id);
    if (iter != _status.end())
    {
        iter.value().color = color;
        iter.value().r = iter.value().color.red()/255.0;
        iter.value().g = iter.value().color.green()/255.0;
        iter.value().b = iter.value().color.blue()/255.0;

        update();
    }
}

void Scene3d::setHumanModel()
{

    _pHumanModel = new Mesh;
    _pHumanModel->ReadStl("../resources/humanmodel.stl");
    _pHumanModel->CalcTrisNormal();

}

void Scene3d::setMyomaModel()
{
    _pMyoma = new Mesh;
    QDir dir;
    QString path=dir.currentPath();
    path.append("/Tumor3d.stl");
    qDebug()<<"tumor path is "<<path;
    _pMyoma->ReadStl(path.toStdString().c_str());
    _pMyoma->CalcTrisNormal();
}

void Scene3d::destorySpots()
{
   QMap<QString,Spot_3d_Status>::iterator iter = _status.begin();
   for (; iter != _status.end(); ++iter)
   {
       delete iter.value().pMesh;
       iter.value().pMesh = NULL;
   }

   update();
}

void Scene3d::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f,0.2265625f,0.421875f,0.0f);  // #003a6c

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    GLfloat light_position[] = {20.0,20.0,20.0,0.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendEquation(GL_FUNC_ADD);

//    glLightfv(GL_LIGHT1,GL_AMBIENT, lightAmbient);
//    glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffuse);
//    glLightfv(GL_LIGHT1,GL_POSITION,light_position);
//    glEnable(GL_LIGHT1);

    glEnable(GL_LIGHT0);

    // Light1 is a spot light
    glEnable(GL_LIGHT1);
    const GLfloat light_ambient[4] = {0.8f, 0.2f, 0.2f, 1.0};
    const GLfloat light_diffuse[4] = {1.0, 0.4f, 0.4f, 1.0};
    const GLfloat light_specular[4] = {1.0, 0.0, 0.0, 1.0};

    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.5);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

    // Light2 is a classical directionnal light
    glEnable(GL_LIGHT2);
    const GLfloat light_ambient2[4] = {0.2f, 0.2f, 2.0, 1.0};
    const GLfloat light_diffuse2[4] = {0.8f, 0.8f, 1.0, 1.0};
    const GLfloat light_specular2[4] = {0.0, 0.0, 1.0, 1.0};

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);

}

void Scene3d::resizeGL(int w, int h)
{
    /* 2.1 viewport 设定窗口的原点 origin (x, y)、宽度和高度 */
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(GLfloat)w/(GLfloat)h,0.1,1000.0);
    glColor4f(1.0,1.0,1.0,0.5);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

void Scene3d::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.2265625f,0.421875f,0.0f); // 设置背景颜色

    glLoadIdentity();
    glTranslatef(0.0,0.0,zoom);

    glRotatef(rotationX,1.0,0.0,0.0);
    glRotatef(rotationY,0.0,1.0,0.0);
    glRotatef(rotationZ,0.0,0.0,1.0);

    glPushMatrix();

    glScalef(0.15,0.15,0.15);

    std::vector<point3d>& humanVers = _pHumanModel->m_vVertices;
    std::vector<Triangle>& humanTris = _pHumanModel->m_vTriangles;
    std::vector<point3d>& humanNormals = _pHumanModel->m_vTriNormal;
    size_t trisSize = humanTris.size();

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < trisSize ; ++i)
    {
        Triangle& tri = humanTris[i];
        point3d& normal = humanNormals[i];
        glNormal3f(normal.x,normal.y,normal.z);
        glColor4f(0.9333,0.0667,0.0,0.8);
        glVertex3f(humanVers[tri.p0].x,humanVers[tri.p0].y,humanVers[tri.p0].z);

        glColor4f(0.9333,0.0667,0.0,0.8);
        glVertex3f(humanVers[tri.p1].x,humanVers[tri.p1].y,humanVers[tri.p1].z);

        glColor4f(0.9333,0.0667,0.0,0.8);
        glVertex3f(humanVers[tri.p2].x,humanVers[tri.p2].y,humanVers[tri.p2].z);
    }

    glEnd();

    glPopMatrix();

    if (!_pMyoma->m_vVertices.empty())
    {
        std::vector<point3d>& Vers = _pMyoma->m_vVertices;
        std::vector<Triangle>& Tris = _pMyoma->m_vTriangles;
        std::vector<point3d>& Normals = _pMyoma->m_vTriNormal;
        size_t trisSize = Tris.size();

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < trisSize ; ++i)
        {
            Triangle& tri = Tris[i];
            point3d& normal = Normals[i];
            glNormal3f(normal.x,normal.y,normal.z);
            glColor4f(0.0,1.0,0.0,0.5);
            glVertex3f(Vers[tri.p0].x,Vers[tri.p0].y,Vers[tri.p0].z);

            glColor4f(0.0,1.0,0.0,0.5);
            glVertex3f(Vers[tri.p1].x,Vers[tri.p1].y,Vers[tri.p1].z);

            glColor4f(0.0,1.0,0.0,0.5);
            glVertex3f(Vers[tri.p2].x,Vers[tri.p2].y,Vers[tri.p2].z);
        }

        glEnd();
    }


//    std::vector<point3d>& vers = _pSpots->m_vVertices;
//    std::vector<Triangle>& tris = _pSpots->m_vTriangles;
//    std::vector<point3d>& normals = _pSpots->m_vTriNormal;
//    size_t triSize = tris.size();

    QMap<QString,Spot_3d_Status>::iterator iter = _status.begin();
    glBegin(GL_TRIANGLES);

    for (; iter != _status.end(); ++iter)
    {
        if (iter.value().pMesh==NULL)
        {
            continue;
        }
        std::vector<point3d>& vers = iter.value().pMesh->m_vVertices;
        std::vector<Triangle>& tris = iter.value().pMesh->m_vTriangles;
        std::vector<point3d>& normals = iter.value().pMesh->m_vTriNormal;
        size_t triSize = tris.size();
        for (size_t i = 0; i < triSize ; ++i)
        {
            Triangle& tri = tris[i];
            point3d& normal = normals[i];
            glNormal3f(normal.x,normal.y,normal.z);
            glColor4f(iter.value().r,iter.value().g,iter.value().b,0.8);
            glVertex3f(vers[tri.p0].x,vers[tri.p0].y,vers[tri.p0].z);

            glColor4f(iter.value().r,iter.value().g,iter.value().b,0.8);
            glVertex3f(vers[tri.p1].x,vers[tri.p1].y,vers[tri.p1].z);

            glColor4f(iter.value().r,iter.value().g,iter.value().b,0.8);
            glVertex3f(vers[tri.p2].x,vers[tri.p2].y,vers[tri.p2].z);
        }
    }
//    for (size_t i = 0; i < triSize ; ++i)
//    {
//        Triangle& tri = tris[i];
//        point3d& normal = normals[i];
//        glNormal3f(normal.x,normal.y,normal.z);
//        glColor4f(0.0,0.2,0.0,0.8);
//        glVertex3f(vers[tri.p0].x,vers[tri.p0].y,vers[tri.p0].z);

//        glColor4f(0.0,0.2,0.0,0.8);
//        glVertex3f(vers[tri.p1].x,vers[tri.p1].y,vers[tri.p1].z);

//        glColor4f(0.0,0.2,0.0,0.8);
//        glVertex3f(vers[tri.p2].x,vers[tri.p2].y,vers[tri.p2].z);
//    }

    glEnd();

}

void Scene3d::createSpotsModel()
{
    GLfloat matrix[12] = {0.0f};
    matrix[0] = 0.8f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.8f;
    matrix[8] = 0.0f;
    matrix[9] = 0.8f;
    matrix[10] = 0.0f;
    matrix[11] = 0.0f;

    qDebug()<<"spot size is "<<_status.size();
    QMap<QString,Spot_3d_Status>::iterator iter = _status.begin();
    for (; iter != _status.end(); ++iter)
    {
        qDebug()<<"x is "<<iter.value().xPos*1000;
        qDebug()<<"y is "<<iter.value().yPos*1000;
        qDebug()<<"z is "<<iter.value().zPos*1000-140.0;

        matrix[3] = -1.0*iter.value().yPos*1000.0;
        matrix[7] = -1.0*(iter.value().zPos*1000.0-140.0);
        matrix[11] = iter.value().xPos*1000.0;
        addSpotNew(matrix,iter.value());
    }
}

void Scene3d::mouseReleaseEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void Scene3d::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void Scene3d::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if(event->buttons() & Qt::LeftButton)
    {
        rotationX += dy*180;
        rotationY += dx*180;

        update();
    }
    else if(event->buttons() & Qt::RightButton)
    {
        rotationX += dy*180;
        rotationY += dx*180;

        update();
    }

    lastPos = event->pos();
}

void Scene3d::wheelEvent(QWheelEvent* event)
{
    GLfloat zValue = event->delta();
    zoom -= zValue*0.01;
    if (zoom>1.0)
    {
        zoom = 1.0;
    }

    update();
}
