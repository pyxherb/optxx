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
		char **const _argv;
		int _cur_arg = 0;
		void *const _user_data;

	public:
		OPTXX_API OptionMatchContext(int argc, char **argv, void *user_data) noexcept;
		OPTXX_API ~OptionMatchContext();

		OPTXX_FORCEINLINE void *get_user_data() const noexcept {
			return _user_data;
		}
	};

	struct SingleArgOption;

	typedef int (*ArglessOptionCallback)(const OptionMatchContext &match_context, const std::string_view &option_str);
	typedef int (*SingleArgOptionCallback)(const OptionMatchContext &match_context, const std::string_view &option_str, const std::string_view &arg);
	typedef int (*CustomOptionCallback)(OptionMatchContext &match_context, const std::string_view &option_str);
	typedef int (*FallbackOptionCallback)(OptionMatchContext &match_context, const std::string_view &option_str);
	typedef void (*RequireOptionArgCallback)(const OptionMatchContext &match_context, const SingleArgOption &option);

	struct ArglessOption {
		const char *name;
		ArglessOptionCallback callback;
	};

	struct SingleArgOption {
		const char *name;
		SingleArgOptionCallback callback;
	};

	struct CustomOption {
		const char *name;
		CustomOptionCallback callback;
	};

	using ArglessOptionMap = std::initializer_list<ArglessOption>;
	using SingleArgOptionMap = std::initializer_list<SingleArgOption>;
	using CustomOptionMap = std::initializer_list<CustomOption>;

	struct CompiledOptionMap {
		peff::HashMap<std::string_view, const ArglessOption *> argless_opts;
		peff::HashMap<std::string_view, const SingleArgOption *> single_arg_opts;
		peff::HashMap<std::string_view, const CustomOption *> custom_opts;
		FallbackOptionCallback fallback_opt_callback;
		RequireOptionArgCallback require_opt_arg_callback;

		OPTXX_API CompiledOptionMap(
			peff::Alloc *alloc,
			FallbackOptionCallback fallback_opt_callback,
			RequireOptionArgCallback require_opt_arg_callback) noexcept;
	};

	[[nodiscard]] OPTXX_API bool build_option_map(
		CompiledOptionMap &opt_map_out,
		const ArglessOptionMap &argless_opts,
		const SingleArgOptionMap &single_arg_opts,
		const CustomOptionMap &custom_opts) noexcept;

	[[nodiscard]] OPTXX_API int do_match_args(const CompiledOptionMap &opt_map, int argc, char **argv, void *user_data);

	template <typename MatchData>
	[[nodiscard]] OPTXX_FORCEINLINE int match_args(const CompiledOptionMap &opt_map, int argc, char **argv, MatchData *user_data) {
		static_assert(!std::is_polymorphic_v<MatchData>, "The type of user data must not be polymorphic");
		return do_match_args(opt_map, argc, argv, (void *)user_data);
	}
}

#endif
