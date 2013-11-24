
#ifndef PASC_LEX_ANAL_API_HPP
#define PASC_LEX_ANAL_API_HPP
#pragma once

#include <QtGlobal>

#ifdef BUILD_ALL_IN_ONE_LIB
#define pascLexAnal_EXPORTS
#endif

#ifndef pascLexAnal_EXPORTS
#define PASC_LEX_ANAL_API   Q_DECL_IMPORT
#define PASC_LEX_ANAL_NO_INTERNALS
#else
#define PASC_LEX_ANAL_API   Q_DECL_EXPORT
#endif

#endif
