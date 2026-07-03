#ifndef SCENE_GLOBAL_H
#define SCENE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCENE_LIBRARY)
#  define SCENESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCENESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SCENE_GLOBAL_H
