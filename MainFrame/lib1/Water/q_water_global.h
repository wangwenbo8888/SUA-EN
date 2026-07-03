#ifndef Q_WATER_GLOBAL_H
#define Q_WATER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(Q_WATER_LIBRARY)
#  define Q_WATERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define Q_WATERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // Q_WATER_GLOBAL_H
