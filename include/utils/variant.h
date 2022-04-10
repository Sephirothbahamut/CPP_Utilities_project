#pragma once

#include <variant>

namespace utils
	{
	// https://stackoverflow.com/questions/63482070/how-can-i-code-something-like-a-switch-for-stdvariant
	
	// helper type for the visitor #4
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

	// explicit deduction guide (not needed as of C++20)
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

	// Usage:
	// std::visit(overloaded
	// 	{
	// 	[](std::monostate&) {/*..*/},
	// 	[](a&) {/*..*/},
	// 	[](b&) {/*..*/},
	// 	[](c&) {/*..*/}
	// 	}, var);
	}