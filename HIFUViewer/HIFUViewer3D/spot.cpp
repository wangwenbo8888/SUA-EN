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

#include "spot.h"
#include <qgl.h>

using namespace qglviewer;

void spot::drawUnirradiate() const
{
    static GLUquadric *quad = gluNewQuadric();

    glPushMatrix();
    glMultMatrixd(frame.matrix());
    glColor4f(0.2549f,0.1843f,0.1216f,0.7843f);
    gluSphere(quad,0.6,10,6);
    gluCylinder(quad, 0.6, 0.6, 8.0, 20, 1);
    glPopMatrix();
}

void spot::drawIrradiating() const
{
    static GLUquadric *quad = gluNewQuadric();

    glPushMatrix();
    glMultMatrixd(frame.matrix());
    glColor4f(0.0f,1.0f,0.0f,0.7843f);
    gluSphere(quad,0.6,10,6);
    gluCylinder(quad, 0.6, 0.6, 8.0, 20, 1);
    glPopMatrix();
}

void spot::drawIrradiated() const
{
    static GLUquadric *quad = gluNewQuadric();

    glPushMatrix();
    glMultMatrixd(frame.matrix());
    glColor4f(0.3882f,0.7216f,0.9412f,0.7843f);
    gluSphere(quad,0.6,10,6);
    gluCylinder(quad, 0.6, 0.6, 8.0, 20, 1);
    glPopMatrix();
}
