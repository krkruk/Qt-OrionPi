#ifndef SERIAL_GLOBAL_H
#define SERIAL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SERIAL_LIBRARY)
#  define SERIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SERIALSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SERIAL_GLOBAL_H
