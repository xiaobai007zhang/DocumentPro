#ifndef VIEWCHECK_GLOBAL_H
#define VIEWCHECK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIEWCHECK_LIBRARY)
#  define VIEWCHECK_EXPORT Q_DECL_EXPORT
#else
#  define VIEWCHECK_EXPORT Q_DECL_IMPORT
#endif

#endif // VIEWCHECK_GLOBAL_H
