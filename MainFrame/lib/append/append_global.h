#ifndef APPEND_GLOBAL_H
#define APPEND_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(APPEND_LIBRARY)
#  define APPENDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define APPENDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // APPEND_GLOBAL_H
