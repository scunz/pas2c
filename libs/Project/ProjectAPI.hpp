
#ifndef PASC_PROJECT_API_HPP
#define PASC_PROJECT_API_HPP
#pragma once

#include <QtGlobal>

#ifdef BUILD_ALL_IN_ONE_LIB
#define pascProject_EXPORTS
#endif

#ifndef pascProject_EXPORTS
#define PASC_PROJECT_API   Q_DECL_IMPORT
#else
#define PASC_PROJECT_API   Q_DECL_EXPORT
#endif

#endif
