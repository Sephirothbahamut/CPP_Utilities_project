#pragma once

#include "compiler.h"

#if defined(utils_compiler_msvc)
    #define utils_disable_warnings_begin __pragma(warning( push ))
	#define utils_disable_warnings_end __pragma(warning( pop )) 

	#define utils_disable_warning(msvc_number, gcc_name) utils_disable_warning_msvc(msvc_number)

	#define utils_disable_warning_msvc(msvc_number) __pragma(warning( disable : msvc_number ))
    
    #define utils_disable_warning_gnu(name)
    #define utils_disable_warning_gcc(name)
    #define utils_disable_warning_clang(name)

#elif defined(utils_compiler_gcc) || defined(utils_compiler_clang)
#define utils_disable_warnings_all_begin

	#define utils_pragma_gnu(X) _Pragma(#X)
	#define utils_disable_warnings_begin utils_pragma_gnu(GCC diagnostic push)
	#define utils_disable_warnings_end utils_pragma_gnu(GCC diagnostic pop) 

	#define utils_disable_warning(msvc_number, gcc_name) utils_disable_warning_gnu(gcc_name)

	#define utils_disable_warning_gnu(name) utils_pragma_gnu(GCC diagnostic ignored name)

    #if defined(utils_compiler_clang)
        #define utils_disable_warning_clang(name) utils_pragma_gnu(GCC diagnostic ignored name)
        #define utils_disable_warning_gcc(name)
    #else
        #define utils_disable_warning_gcc(name) utils_pragma_gnu(GCC diagnostic ignored name)
        #define utils_disable_warning_clang(name)
    #endif
    
    #define utils_disable_warning_msvc(msvc_number)
    
#elif
#define utils_disable_warnings_all_begin
    #define utils_disable_warnings_begin
    #define utils_disable_warnings_end

    #define utils_disable_warning(msvc_number, gcc_name)

    #define utils_disable_warning_gnu(name)
    #define utils_disable_warning_gcc(name)
    #define utils_disable_warning_clang(name)
    #define utils_disable_warning_msvc(msvc_number)
#endif