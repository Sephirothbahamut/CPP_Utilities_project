#pragma once

#include <numeric>
#include <numbers>
#include <concepts>
// Reference https://easings.net/

//TODO write test cases


namespace utils::math::easing
	{
	namespace _
		{
		template <std::floating_point T>
		T ease_in_sine(T t)
			{
			return std::sin(1.5707963 * t);
			}

		template <std::floating_point T>
		T ease_out_sine(T t)
			{
			return 1 + std::sin(1.5707963 * (--t));
			}

		template <std::floating_point T>
		T ease_in_out_sine(T t)
			{
			return 0.5 * (1 + std::sin(3.1415926 * (t - 0.5)));
			}

		template <std::floating_point T>
		T ease_in_quad(T t)
			{
			return t * t;
			}

		template <std::floating_point T>
		T ease_out_quad(T t)
			{
			return t * (2 - t);
			}

		template <std::floating_point T>
		T ease_in_out_quad(T t)
			{
			return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
			}

		template <std::floating_point T>
		T ease_in_cubic(T t)
			{
			return t * t * t;
			}

		template <std::floating_point T>
		T ease_out_cubic(T t)
			{
			return 1 + (--t) * t * t;
			}

		template <std::floating_point T>
		T ease_in_out_cubic(T t)
			{
			return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
			}

		template <std::floating_point T>
		T ease_in_quart(T t)
			{
			t *= t;
			return t * t;
			}

		template <std::floating_point T>
		T ease_out_quart(T t)
			{
			t = (--t) * t;
			return 1 - t * t;
			}

		template <std::floating_point T>
		T ease_in_out_quart(T t)
			{
			if (t < 0.5)
				{
				t *= t;
				return 8 * t * t;
				}
			else
				{
				t = (--t) * t;
				return 1 - 8 * t * t;
				}
			}

		template <std::floating_point T>
		T ease_in_quint(T t)
			{
			T t2 = t * t;
			return t * t2 * t2;
			}

		template <std::floating_point T>
		T ease_out_quint(T t)
			{
			T t2 = (--t) * t;
			return 1 + t * t2 * t2;
			}

		template <std::floating_point T>
		T ease_in_out_quint(T t)
			{
			T t2;
			if (t < 0.5)
				{
				t2 = t * t;
				return 16 * t * t2 * t2;
				}
			else
				{
				t2 = (--t) * t;
				return 1 + 16 * t * t2 * t2;
				}
			}

		template <std::floating_point T>
		T ease_in_expo(T t)
			{
			return (std::pow(2, 8 * t) - 1) / 255;
			}

		template <std::floating_point T>
		T ease_out_expo(T t)
			{
			return 1 - std::pow(2, -8 * t);
			}

		template <std::floating_point T>
		T ease_in_out_expo(T t)
			{
			if (t < 0.5)
				{
				return (std::pow(2, 16 * t) - 1) / 510;
				}
			else
				{
				return 1 - 0.5 * std::pow(2, -16 * (t - 0.5));
				}
			}

		template <std::floating_point T>
		T ease_in_circ(T t)
			{
			return 1 - std::sqrt(1 - t);
			}

		template <std::floating_point T>
		T ease_out_circ(T t)
			{
			return std::sqrt(t);
			}

		template <std::floating_point T>
		T ease_in_out_circ(T t)
			{
			if (t < 0.5)
				{
				return (1 - std::sqrt(1 - 2 * t)) * 0.5;
				}
			else
				{
				return (1 + std::sqrt(2 * t - 1)) * 0.5;
				}
			}

		template <std::floating_point T>
		T ease_in_back(T t)
			{
			return t * t * (2.70158 * t - 1.70158);
			}

		template <std::floating_point T>
		T ease_out_back(T t)
			{
			return 1 + (--t) * t * (2.70158 * t + 1.70158);
			}

		template <std::floating_point T>
		T ease_in_out_back(T t)
			{
			if (t < 0.5)
				{
				return t * t * (7 * t - 2.5) * 2;
				}
			else
				{
				return 1 + (--t) * t * 2 * (7 * t + 2.5);
				}
			}

		template <std::floating_point T>
		T ease_in_elastic(T t)
			{
			T t2 = t * t;
			return t2 * t2 * std::sin(t * std::numbers::pi * 4.5);
			}

		template <std::floating_point T>
		T ease_out_elastic(T t)
			{
			T t2 = (t - 1) * (t - 1);
			return 1 - t2 * t2 * std::cos(t * std::numbers::pi * 4.5);
			}

		template <std::floating_point T>
		T ease_in_out_elastic(T t)
			{
			T t2;
			if (t < 0.45)
				{
				t2 = t * t;
				return 8 * t2 * t2 * std::sin(t * std::numbers::pi * 9);
				}
			else if (t < 0.55)
				{
				return 0.5 + 0.75 * std::sin(t * std::numbers::pi * 4);
				}
			else
				{
				t2 = (t - 1) * (t - 1);
				return 1 - 8 * t2 * t2 * std::sin(t * std::numbers::pi * 9);
				}
			}

		template <std::floating_point T>
		T ease_in_bounce(T t)
			{
			return std::pow(2, 6 * (t - 1)) * std::abs(std::sin(t * std::numbers::pi * 3.5));
			}

		template <std::floating_point T>
		T ease_out_bounce(T t)
			{
			return 1 - std::pow(2, -6 * t) * std::abs(std::cos(t * std::numbers::pi * 3.5));
			}

		template <std::floating_point T>
		T ease_in_out_bounce(T t)
			{
			if (t < 0.5)
				{
				return 8 * std::pow(2, 8 * (t - 1)) * std::abs(std::sin(t * std::numbers::pi * 7));
				}
			else
				{
				return 1 - 8 * std::pow(2, -8 * t) * std::abs(std::sin(t * std::numbers::pi * 7));
				}
			}
		}
	enum type { linear, sine, quad, cubic, quart, quint, expo, circ, back, elastic, bounce };
	enum dir { in, out, in_out };

	template <type type, dir dir = in_out, std::floating_point T>
	T ease(T t)
		{
		if constexpr (type == linear) { return t; }
		switch (type)
			{
			case utils::math::easing::linear: return t;
			case utils::math::easing::sine:    if constexpr (dir == dir::in)     { return _::ease_in_sine       (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_sine      (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_sine   (t); }
			case utils::math::easing::quad:    if constexpr (dir == dir::in)     { return _::ease_in_quad       (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_quad      (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_quad   (t); }
			case utils::math::easing::cubic:   if constexpr (dir == dir::in)     { return _::ease_in_cubic      (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_cubic     (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_cubic  (t); }
			case utils::math::easing::quart:   if constexpr (dir == dir::in)     { return _::ease_in_quart      (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_quart     (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_quart  (t); }
			case utils::math::easing::quint:   if constexpr (dir == dir::in)     { return _::ease_in_quint      (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_quint     (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_quint  (t); }
			case utils::math::easing::expo:    if constexpr (dir == dir::in)     { return _::ease_in_expo       (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_expo      (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_expo   (t); }
			case utils::math::easing::circ:    if constexpr (dir == dir::in)     { return _::ease_in_circ       (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_circ      (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_circ   (t); }
			case utils::math::easing::back:    if constexpr (dir == dir::in)     { return _::ease_in_back       (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_back      (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_back   (t); }
			case utils::math::easing::elastic: if constexpr (dir == dir::in)     { return _::ease_in_elastic    (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_elastic   (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_elastic(t); }
			case utils::math::easing::bounce:  if constexpr (dir == dir::in)     { return _::ease_in_bounce     (t); }
			                              else if constexpr (dir == dir::out)    { return _::ease_out_bounce    (t); }
			                              else if constexpr (dir == dir::in_out) { return _::ease_in_out_bounce (t); }
			}
		}

	template <type in, type out, std::floating_point T>
	T ease(T t)
		{
		if constexpr (in == out) { return ease<in, T>(t); }

		if (t < 0.5) { return ease<in,  T>(t); }
		else         { return ease<out, T>(t); }
		}
	}