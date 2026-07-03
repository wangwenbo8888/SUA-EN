/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.0.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "frameTransform.h"

#include <manipulatedFrame.h>

#define PI 3.14159265

using namespace std;
using namespace qglviewer; // Vec

Viewer::Viewer()
{

    for (int i = 0; i < 111; ++i)
    {
        if (i%4==0)
        {
           _showList.append(true);
        }
        else
        {
           _showList.append(false);
        }
    }


    float radius = 0.0;
    float theta = 0.0;
    for (int layer = 0; layer < 3; ++layer)
    {
        spot* o = new spot();
        o->frame.setPosition(Vec(0.0,0.0,-14.0+10.0*layer));
        _spots.append(o);
        radius = 1.8;
        for (int i =0; i < 6; ++i)
        {
            theta = i*2.0*PI/6.0;
            spot* o = new spot();
            o->frame.setPosition(Vec(radius*cos(theta),radius*sin(theta),-14.0+10.0*layer));
            _spots.append(o);
        }
        radius = 3.6;
        for (int i =0; i < 12; ++i)
        {
            theta = i*2.0*PI/12.0;
            spot* o = new spot();
            o->frame.setPosition(Vec(radius*cos(theta),radius*sin(theta),-14.0+10.0*layer));
            _spots.append(o);
        }

        radius = 5.4;
        for (int i =0; i < 18; ++i)
        {
            theta = i*2.0*PI/18.0;
            spot* o = new spot();
            o->frame.setPosition(Vec(radius*cos(theta),radius*sin(theta),-14.0+10.0*layer));
            _spots.append(o);
        }

    }

    for (int i = 0; i < 3; ++i)
    {
        plane* p = new plane();
        p->frame.setPosition(Vec(0.0,0.0,-10.0+10*i));
        _planes.append(p);
    }
}

void Viewer::init()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Light0 is the default ambient light
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

    light1 = new ManipulatedFrame();
    light2 = new ManipulatedFrame();
    setMouseTracking(true);

    light1->setPosition(0.5, 0.5, 0);
    // Align z axis with -position direction : look at scene center
    light1->setOrientation(Quaternion(Vec(0, 0, 1), -light1->position()));

    light2->setPosition(-0.5, 0.5, 0);

    restoreStateFromFile();

    setSceneRadius(20);
    showEntireScene();
    // setAxisIsDrawn();
}

void Viewer::draw()
{
    return;
  // Draws line sets (red, green, blue) with different origins, but with a
  // common end point, located on a circle in the XY plane.

  float pos[4] = {1.0, 0.5, 1.0, 0.0};
  // Directionnal light
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  pos[3] = 1.0;
  // Spot light
  Vec pos1 = light1->position();
  pos[0] = float(pos1.x);
  pos[1] = float(pos1.y);
  pos[2] = float(pos1.z);
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,
            light1->inverseTransformOf(Vec(0, 0, 1)));

  // Point light
  Vec pos2 = light2->position();
  pos[0] = float(pos2.x);
  pos[1] = float(pos2.y);
  pos[2] = float(pos2.z);
  glLightfv(GL_LIGHT2, GL_POSITION, pos);

//  for (int i = 0; i < int(_spots.size()); i++)
//  {
//      if (_showList.at(i))
//      {
//          if ((count+i)%3==0)
//          {
//              _spots.at(i)->drawUnirradiate();
//          }
//          else if ((count+i)%3==1)
//          {
//              _spots.at(i)->drawIrradiating();
//          }
//          else
//          {
//              _spots.at(i)->drawIrradiated();
//          }

//      }
//  }

//  for (int i = 0; i < int(_planes.size()); i++)
//  {
//     _planes.at(i)->draw();
//  }
  // Here, the camera position in world coord. system  is camera()->position().
  // The world origin position in camera frame can be obtained from
  // camera()->cameraCoordinatesOf(Vec(0.0, 0.0, 0.0))
}

//QString Viewer::helpString() const {
//  QString text("<h2>F r a m e T r a n s f o r m</h2>");
//  text += "This example illustrates how easy it is to switch between the "
//          "camera and ";
//  text += "the world coordinate systems using the "
//          "<i>camera()->cameraCoordinatesOf()</i> ";
//  text += "and <i>camera::worldCoordinatesOf()</i> functions.<br><br>";
//  text += "You can create your own hierarchy of local coordinates systems and "
//          "each of ";
//  text += "them can be manipulated with the mouse (see the "
//          "<i>manipulatedFrame</i> and <i>luxo</i> examples). ";
//  text += "Standard functions allow you to convert from any local frame to any "
//          "other, ";
//  text += "the world/camera conversion presented here simply being an "
//          "illustration.<br><br>";
//  text += "See <i>examples/frameTransform.html</i> for an explanation of the "
//          "meaning of these weird lines.";
//  return text;
//}
