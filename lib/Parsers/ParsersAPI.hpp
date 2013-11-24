
#ifndef PASC_PARSERS_API_HPP
#define PASC_PARSERS_API_HPP
#pragma once

#include <QtGlobal>

#ifdef BUILD_ALL_IN_ONE_LIB
#define pascParsers_EXPORTS
#endif

#ifndef pascParsers_EXPORTS
#define PASC_PARSERS_API   Q_DECL_IMPORT
#else
#define PASC_PARSERS_API   Q_DECL_EXPORT
#endif

#endif
