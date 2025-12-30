#include "optxx.h"

using namespace optxx;

OPTXX_API OptionMatchContext::OptionMatchContext(int argc, char** argv, void* userData) noexcept: _argc(argc), _argv(argv), _userData(userData) {

}

OPTXX_API OptionMatchContext::~OptionMatchContext() {
}

OPTXX_API CompiledOptionMap::CompiledOptionMap(
	peff::Alloc* alloc,
	FallbackOptionCallback fallbackOptionCallback,
	RequireOptionArgCallback requireOptionArgCallback) noexcept
	: arglessOptions(alloc),
	singleArgOptions(alloc),
	customOptions(alloc),
	fallbackOptionCallback(fallbackOptionCallback),
	requireOptionArgCallback(requireOptionArgCallback) {
}

OPTXX_API bool optxx::buildOptionMap(
	CompiledOptionMap& optionMapOut,
	const ArglessOptionMap& arglessOptions,
	const SingleArgOptionMap& singleArgOptions,
	const CustomOptionMap& customOptions) noexcept {
	for (const auto& i : arglessOptions) {
		if (!optionMapOut.arglessOptions.insert(std::string_view(i.name), &i)) {
			return false;
		}
	}

	for (const auto& i : singleArgOptions) {
		if (!optionMapOut.singleArgOptions.insert(std::string_view(i.name), &i)) {
			return false;
		}
	}

	for (const auto& i : customOptions) {
		if (!optionMapOut.customOptions.insert(std::string_view(i.name), &i)) {
			return false;
		}
	}

	return true;
}

OPTXX_API int optxx::matchArgs(const CompiledOptionMap& optionMap, int argc, char** argv, void* userData) {
	OptionMatchContext matchContext = { argc, argv, userData };
	std::string_view opt;
	for (int i = 1; i < argc; ++i) {
		opt = std::string_view(argv[i]);
		if (auto it = optionMap.arglessOptions.find(opt); it != optionMap.arglessOptions.end()) {
			if (int result = it.value()->callback(matchContext, argv[i]); result) {
				return result;
			}

			continue;
		}

		if (auto it = optionMap.singleArgOptions.find(opt); it != optionMap.singleArgOptions.end()) {
			const char* opt = argv[i];
			if (++i == argc) {
				optionMap.requireOptionArgCallback(matchContext, *it.value());
				return EINVAL;
			}

			if (int result = it.value()->callback(matchContext, opt, argv[i]); result) {
				return result;
			}

			continue;
		}

		if (auto it = optionMap.customOptions.find(opt); it != optionMap.customOptions.end()) {
			if (int result = it.value()->callback(matchContext, argv[i]); result) {
				return result;
			}

			continue;
		}

		if (int result = optionMap.fallbackOptionCallback(matchContext, opt); result) {
			return result;
		}
	}

	return 0;
}