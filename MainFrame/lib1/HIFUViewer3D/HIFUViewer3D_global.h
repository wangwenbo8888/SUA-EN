#ifndef HIFU3D_GLOBAL_H
#define HIFU3D_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HIFUVIEWER3D_LIBRARY)
#  define HIFUVIEWER3D_EXPORT Q_DECL_EXPORT
#else
#  define HIFUVIEWER3D_EXPORT Q_DECL_IMPORT
#endif

#endif // HIFU3D_GLOBAL_H
