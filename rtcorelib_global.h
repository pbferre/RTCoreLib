#ifndef RTCORELIB_GLOBAL_H
#define RTCORELIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RTCORELIB_LIBRARY)
#  define RTCORELIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RTCORELIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RTCORELIB_GLOBAL_H
