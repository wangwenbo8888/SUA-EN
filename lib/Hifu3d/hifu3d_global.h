#ifndef HIFU3D_GLOBAL_H
#define HIFU3D_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HIFU3D_LIBRARY)
#  define HIFU3D_EXPORT Q_DECL_EXPORT
#else
#  define HIFU3D_EXPORT Q_DECL_IMPORT
#endif

#endif // HIFU3D_GLOBAL_H
