
#ifndef PASC_CODE_MODEL_API_HPP
#define PASC_CODE_MODEL_API_HPP
#pragma once

#include <QtGlobal>

#ifdef BUILD_ALL_IN_ONE_LIB
#define pascCodeModel_EXPORTS
#endif

#ifndef pascCodeModel_EXPORTS
#define PASC_CODE_MODEL_API     Q_DECL_IMPORT
#define PASC_CODE_MODEL_NO_INTERNALS
#else
#define PASC_CODE_MODEL_API     Q_DECL_EXPORT
#endif

#endif
