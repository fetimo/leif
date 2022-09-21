#ifndef UK_GLOBAL_H
#define UK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UK_LIBRARY)
#  define UK_EXPORT Q_DECL_EXPORT
#else
#  define UK_EXPORT Q_DECL_IMPORT
#endif

#endif // UK_GLOBAL_H
