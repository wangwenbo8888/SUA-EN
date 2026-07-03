#ifndef PLAN_GLOBAL_H
#define PLAN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAN_LIBRARY)
#  define PLANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLANSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAN_GLOBAL_H
