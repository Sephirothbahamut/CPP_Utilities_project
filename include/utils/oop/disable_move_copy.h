#pragma once

namespace utils::oop
	{
	struct non_constructible
		{
		inline non_constructible() = delete;
		};
	struct non_copyable
		{
		inline  non_copyable() {}
		inline ~non_copyable() {}
		inline       non_copyable           (const non_copyable& copy) = delete;
		inline const non_copyable& operator=(const non_copyable& copy) = delete;
		};
	struct non_movable
		{
		inline  non_movable() {}
		inline ~non_movable() {}
		inline       non_movable           (non_movable&& move) noexcept = delete;
		inline const non_movable& operator=(non_movable&& move) noexcept = delete;
		};
	}