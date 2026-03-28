#include "optxx.h"

using namespace optxx;

OPTXX_API OptionMatchContext::OptionMatchContext(int argc, char** argv, void* user_data) noexcept: _argc(argc), _argv(argv), _user_data(user_data) {

}

OPTXX_API OptionMatchContext::~OptionMatchContext() {
}

OPTXX_API CompiledOptionMap::CompiledOptionMap(
	peff::Alloc* alloc,
	FallbackOptionCallback fallback_opt_callback,
	RequireOptionArgCallback require_opt_arg_callback) noexcept
	: argless_opts(alloc),
	single_arg_opts(alloc),
	custom_opts(alloc),
	fallback_opt_callback(fallback_opt_callback),
	require_opt_arg_callback(require_opt_arg_callback) {
}

OPTXX_API bool optxx::build_option_map(
	CompiledOptionMap& opt_map_out,
	const ArglessOptionMap& argless_opts,
	const SingleArgOptionMap& single_arg_opts,
	const CustomOptionMap& custom_opts) noexcept {
	for (const auto& i : argless_opts) {
		if (!opt_map_out.argless_opts.insert(std::string_view(i.name), &i)) {
			return false;
		}
	}

	for (const auto& i : single_arg_opts) {
		if (!opt_map_out.single_arg_opts.insert(std::string_view(i.name), &i)) {
			return false;
		}
	}

	for (const auto& i : custom_opts) {
		if (!opt_map_out.custom_opts.insert(std::string_view(i.name), &i)) {
			return false;
		}
	}

	return true;
}

OPTXX_API int optxx::do_match_args(const CompiledOptionMap& opt_map, int argc, char** argv, void* user_data) {
	OptionMatchContext match_context = { argc, argv, user_data };
	std::string_view opt;
	for (int i = 1; i < argc; ++i) {
		opt = std::string_view(argv[i]);
		if (auto it = opt_map.argless_opts.find(opt); it != opt_map.argless_opts.end()) {
			if (int result = it.value()->callback(match_context, argv[i]); result) {
				return result;
			}

			continue;
		}

		if (auto it = opt_map.single_arg_opts.find(opt); it != opt_map.single_arg_opts.end()) {
			const char* opt = argv[i];
			if (++i == argc) {
				opt_map.require_opt_arg_callback(match_context, *it.value());
				return EINVAL;
			}

			if (int result = it.value()->callback(match_context, opt, argv[i]); result) {
				return result;
			}

			continue;
		}

		if (auto it = opt_map.custom_opts.find(opt); it != opt_map.custom_opts.end()) {
			if (int result = it.value()->callback(match_context, argv[i]); result) {
				return result;
			}

			continue;
		}

		if (int result = opt_map.fallback_opt_callback(match_context, opt); result) {
			return result;
		}
	}

	return 0;
}
