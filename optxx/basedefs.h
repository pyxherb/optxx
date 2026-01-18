#pragma once
#ifndef _OPTXX_BASEDEFS_H_
	#define _OPTXX_BASEDEFS_H_

	#include <peff/base/basedefs.h>

	#define OPTXX_DLLEXPORT PEFF_DLLEXPORT
	#define OPTXX_DLLIMPORT PEFF_DLLIMPORT
	#define OPTXX_FORCEINLINE PEFF_FORCEINLINE

	#if defined(_MSC_VER)
		#define OPTXX_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
			apiModifier extern template class name<__VA_ARGS__>;
		#define OPTXX_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
			apiModifier template class name<__VA_ARGS__>;
	#elif defined(__GNUC__) || defined(__clang__)
		#define OPTXX_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
			extern template class apiModifier name<__VA_ARGS__>;
		#define OPTXX_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
			template class name<__VA_ARGS__>;
	#else
		#define OPTXX_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...)
		#define OPTXX_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...)
	#endif

	#if PEFF_DYNAMIC_LINK
		#if IS_OPTXX_BASE_BUILDING
			#define OPTXX_API OPTXX_DLLEXPORT
		#else
			#define OPTXX_API OPTXX_DLLIMPORT
		#endif
	#else
		#define OPTXX_API
	#endif

#endif
