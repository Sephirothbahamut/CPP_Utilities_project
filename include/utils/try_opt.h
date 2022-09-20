#pragma once

#include <optional>
#include <concepts>
#include <functional>
#include <type_traits>

//TODO test cases
//TODO make interface and names better

namespace utils::attempt
	{
	//TODO make optional and non-optional versions
	template <class F>
	auto catching(F&& f) -> std::conditional<std::same_as<decltype(f()), void>, void, std::optional<decltype(f())>>
		{
		if constexpr (std::same_as<decltype(f()), void>)
			{
			try { f(); }
			catch (...) {}
			}
		else
			{
			try { return f(); }
			catch (...) { return std::nullopt; }
			}
		}

	//TODO expose in construct.h
	
	//namespace construct
	//	{
	//
	//
	//	template <typename T, typename ...param_ts>
	//	T construct(param_ts&& ...params) noexcept
	//		{
	//		static_assert(false, "Calling try_construct is equivalent to calling the regular constructor. Just do that :).");
	//		}
	//
	//	template <typename T, typename ...param_ts>
	//	std::optional<T> opt(param_ts&& ...params) noexcept
	//		{
	//		try { return T{std::forward(params...)}; }
	//		catch (const std::exception& e) { callable(e); }
	//		catch (...) { callable(std::exception{"unknown exception"}); }
	//		return std::nullopt;
	//		}
	//
	//	template <typename T, typename ...param_ts>
	//	std::optional<T> opt_else(std::function<void(const std::exception&)> callable, param_ts&& ...params) noexcept
	//		{
	//		try { return T{std::forward(params...)}; }
	//		catch (const std::exception& e) { callable(e); }
	//		catch (...) { callable(std::exception{"unknown exception"}); }
	//		return std::nullopt;
	//		}
	//
	//	template <typename T, typename ...param_ts>
	//	T else(std::function<T(const std::exception&)> callable, param_ts&& ...params) noexcept
	//		{
	//		try { return T{std::forward(params...)}; }
	//		catch (const std::exception& e) { callable(e); }
	//		catch (...) { return callable(std::exception{"unknown exception"}); }
	//		}
	//	}
	}