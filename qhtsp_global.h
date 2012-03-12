#ifndef QHTSP_GLOBAL_H
#define QHTSP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QHTSP_LIBRARY)
#  define QHTSPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QHTSPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QHTSP_GLOBAL_H
