
#ifndef PASC_CORE_UTILS_API_HPP
#define PASC_CORE_UTILS_API_HPP
#pragma once

#include <QtGlobal>

#ifdef BUILD_ALL_IN_ONE_LIB
#define pascCoreUtils_EXPORTS
#endif

#ifndef pascCoreUtils_EXPORTS
#define PASC_CORE_UTILS_API     Q_DECL_IMPORT
#else
#define PASC_CORE_UTILS_API     Q_DECL_EXPORT
#endif

#endif
