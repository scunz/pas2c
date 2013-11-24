
#ifndef PASC_INTERFACES_API_HPP
#define PASC_INTERFACES_API_HPP
#pragma once

#include <QtGlobal>

#ifdef BUILD_ALL_IN_ONE_LIB
#define pascInterfaces_EXPORTS
#endif

#ifndef pascInterfaces_EXPORTS
#define PASC_INTERFACES_API   Q_DECL_IMPORT
#else
#define PASC_INTERFACES_API   Q_DECL_EXPORT
#endif

#endif
