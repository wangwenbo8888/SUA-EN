#ifndef HIFUREGIST_GLOBAL_H_
#define HIFUREGIST_GLOBAL_H_

#include <QtCore/qglobal.h>

#if defined(HIFUREGIST_LIBRARY)
#  define HIFUREGIST_EXPORT Q_DECL_EXPORT
#else
#  define HIFUREGIST_EXPORT Q_DECL_IMPORT
#endif

#endif // HIFUREGIST_GLOBAL_H_
