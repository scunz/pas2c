
#ifndef PASC_CODE_MODEL_API_HPP
#define PASC_CODE_MODEL_API_HPP
#pragma once

#include <QtGlobal>

#ifndef pascCodeModel_EXPORTS
#define PASC_CODE_MODEL_API     Q_DECL_IMPORT
#define PASC_CODE_MODEL_NO_INTERNALS
#else
#define PASC_CODE_MODEL_API     Q_DECL_EXPORT
#endif

#endif
