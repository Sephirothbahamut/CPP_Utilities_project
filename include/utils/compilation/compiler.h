#pragma once

#if defined(_MSC_VER)
    #define utils_compiler_msvc
#elif defined(__clang__) //note: check clang before gcc because clang defines __GNUC__ as well, not just __clang__
    #define utils_compiler_clang
#elif defined(__GNUC__)
    #define utils_compiler_gcc
#elif
    #define utils_compiler_unknown
#endif

namespace utils::compilation
	{
	enum class compilers { msvc, gcc, clang, unknown };

	inline constexpr compilers compiler
#if defined(utils_compiler_msvc)
		{compilers::msvc};
#elif defined(utils_compiler_clang)
		{compilers::clang};
#elif defined(utils_compiler_gcc)
		{compilers::gcc};
#else
		{compilers::unknown};
#endif
	}