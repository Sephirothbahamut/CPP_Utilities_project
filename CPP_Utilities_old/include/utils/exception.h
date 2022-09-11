#pragma once
#include <optional>

namespace utils
	{
	template <typename T, typename ...Args>
	std::optional<T> try_construct(Args&&... args)
		{
		try { return T{args...}; }
		catch (...) { return std::nullopt; }
		}

	template <class F>
	auto try_catching(F&& f) -> std::optional<decltype(f())> {
		try { return f(); }
		catch (...) { return std::nullopt; }
		}
	}