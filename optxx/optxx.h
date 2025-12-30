#pragma once
#ifndef _OPTXX_OPTXX_H_
#define _OPTXX_OPTXX_H_

#include "basedefs.h"
#include <peff/containers/hashmap.h>
#include <peff/containers/string.h>

namespace optxx {
	class OptionMatchContext final {
	private:
		const int _argc;
		char** const _argv;
		int _currentArg = 0;
		void* const _userData;

	public:
		OPTXX_API OptionMatchContext(int argc, char** argv, void* userData) noexcept;
		OPTXX_API ~OptionMatchContext();

		OPTXX_FORCEINLINE void* getUserData() const noexcept {
			return _userData;
		}
	};

	struct SingleArgOption;

	typedef int (*ArglessOptionCallback)(const OptionMatchContext& matchContext, const std::string_view &optionStr);
	typedef int (*SingleArgOptionCallback)(const OptionMatchContext& matchContext, const std::string_view& optionStr, const std::string_view &arg);
	typedef int (*CustomOptionCallback)(OptionMatchContext& matchContext, const std::string_view& optionStr);
	typedef int (*FallbackOptionCallback)(OptionMatchContext& matchContext, const std::string_view& optionStr);
	typedef void (*RequireOptionArgCallback)(const OptionMatchContext& matchContext, const SingleArgOption& option);

	struct ArglessOption {
		const char* name;
		ArglessOptionCallback callback;
	};

	struct SingleArgOption {
		const char* name;
		SingleArgOptionCallback callback;
	};

	struct CustomOption {
		const char* name;
		CustomOptionCallback callback;
	};

	using ArglessOptionMap = std::initializer_list<ArglessOption>;
	using SingleArgOptionMap = std::initializer_list<SingleArgOption>;
	using CustomOptionMap = std::initializer_list<CustomOption>;

	struct CompiledOptionMap {
		peff::HashMap<std::string_view, const ArglessOption*> arglessOptions;
		peff::HashMap<std::string_view, const SingleArgOption*> singleArgOptions;
		peff::HashMap<std::string_view, const CustomOption*> customOptions;
		FallbackOptionCallback fallbackOptionCallback;
		RequireOptionArgCallback requireOptionArgCallback;

		OPTXX_API CompiledOptionMap(
			peff::Alloc* alloc,
			FallbackOptionCallback fallbackOptionCallback,
			RequireOptionArgCallback requireOptionArgCallback) noexcept;
	};

	[[nodiscard]] OPTXX_API bool buildOptionMap(
		CompiledOptionMap& optionMapOut,
		const ArglessOptionMap& arglessOptions,
		const SingleArgOptionMap& singleArgOptions,
		const CustomOptionMap& customOptions) noexcept;

	[[nodiscard]] OPTXX_API int matchArgs(const CompiledOptionMap& optionMap, int argc, char** argv, void* userData);
}

#endif
