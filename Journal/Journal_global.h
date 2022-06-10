#ifndef JOURNAL_GLOBAL_H
#define JOURNAL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(JOURNAL_LIBRARY)
#  define JOURNAL_EXPORT Q_DECL_EXPORT
#else
#  define JOURNAL_EXPORT Q_DECL_IMPORT
#endif

#endif // JOURNAL_GLOBAL_H
