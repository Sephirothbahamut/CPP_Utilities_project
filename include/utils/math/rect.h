#pragma once

#include "../console/colour.h"
#include "vec2.h"

namespace utils::math
	{
	template <typename T = float>
	struct rect
		{
#pragma region Variables
		T rr;
		T up;
		T ll;
		T dw;
#pragma endregion Variables

#pragma region Accessors
		// Corners
		T    get_up() const noexcept { return up; }
		T    get_dw() const noexcept { return dw; }
		T    get_rr() const noexcept { return rr; }
		T    get_ll() const noexcept { return ll; }
		void set_up(T v)    noexcept { up = v; }
		void set_dw(T v)    noexcept { dw = v; }
		void set_rr(T v)    noexcept { rr = v; }
		void set_ll(T v)    noexcept { ll = v; }

		vec2<T> get_ul()        const noexcept { return {ll, up}; }
		vec2<T> get_ur()        const noexcept { return {rr, up}; }
		vec2<T> get_dr()        const noexcept { return {rr, dw}; }
		vec2<T> get_dl()        const noexcept { return {ll, dw}; }
		void    set_ul(vec2<T> value) noexcept { ll = value.x; up = value.y; }
		void    set_ur(vec2<T> value) noexcept { rr = value.x; up = value.y; }
		void    set_dr(vec2<T> value) noexcept { rr = value.x; dw = value.y; }
		void    set_dl(vec2<T> value) noexcept { ll = value.x; dw = value.y; }

		// Pos-size
		T       get_x()        const noexcept { return ll; }
		T       get_y()        const noexcept { return up; }
		T       get_h()        const noexcept { return dw - up; }
		T       get_w()        const noexcept { return rr - ll; }
		vec2<T> get_s()        const noexcept { return {w, h}; }
		vec2<T> get_p()        const noexcept { return ul; }
		void    set_x(T       value) noexcept { T tmp = w; ll = value; w = tmp; }
		void    set_y(T       value) noexcept { T tmp = h; up = value; h = tmp; }
		void    set_h(T       value) noexcept { dw = up + value; }
		void    set_w(T       value) noexcept { rr = ll + value; }
		void    set_s(vec2<T> value) noexcept { w = value.x; h = value.y; }
		void    set_p(vec2<T> value) noexcept { x = value.x; y = value.y; }
#pragma endregion Accessors

#pragma region Properties
		// Aliases
		__declspec(property(get = get_up, put = set_up)) T top;
		__declspec(property(get = get_dw, put = set_dw)) T down;
		__declspec(property(get = get_dw, put = set_dw)) T bottom;
		__declspec(property(get = get_ll, put = set_ll)) T left;
		__declspec(property(get = get_rr, put = set_rr)) T right;
		__declspec(property(get = get_ll, put = set_ll)) T x1;
		__declspec(property(get = get_rr, put = set_rr)) T x2;
		__declspec(property(get = get_up, put = set_up)) T y1;
		__declspec(property(get = get_dw, put = set_dw)) T y2;

		// Corners
		__declspec(property(get = get_ul, put = set_ul)) vec2<T> ul;
		__declspec(property(get = get_ul, put = set_ul)) vec2<T> up_left;
		__declspec(property(get = get_ul, put = set_ul)) vec2<T> top_left;
		__declspec(property(get = get_ur, put = set_ur)) vec2<T> ur;
		__declspec(property(get = get_ur, put = set_ur)) vec2<T> up_right;
		__declspec(property(get = get_ur, put = set_ur)) vec2<T> top_right;
		__declspec(property(get = get_dr, put = set_dr)) vec2<T> dr;
		__declspec(property(get = get_dr, put = set_dr)) vec2<T> down_right;
		__declspec(property(get = get_dr, put = set_dr)) vec2<T> bottom_right;
		__declspec(property(get = get_dl, put = set_dl)) vec2<T> dl;
		__declspec(property(get = get_dl, put = set_dl)) vec2<T> down_left;
		__declspec(property(get = get_dl, put = set_dl)) vec2<T> bottom_left;

		// Pos-size
		__declspec(property(get = get_x, put = set_x)) T x;
		__declspec(property(get = get_y, put = set_y)) T y;
		__declspec(property(get = get_h, put = set_h)) T h;
		__declspec(property(get = get_h, put = set_h)) T height;
		__declspec(property(get = get_w, put = set_w)) T w;
		__declspec(property(get = get_w, put = set_w)) T width;
		__declspec(property(get = get_s, put = set_s)) vec2<T> s;
		__declspec(property(get = get_s, put = set_s)) vec2<T> size;
		__declspec(property(get = get_p, put = set_p)) vec2<T> p;
		__declspec(property(get = get_p, put = set_p)) vec2<T> pos;
		__declspec(property(get = get_p, put = set_p)) vec2<T> position;
#pragma endregion Properties

		bool contains(vec2<T> point) const noexcept { return point.x >= ll && point.x <= rr && point.y >= up && point.y <= dw; }
		};
	}



namespace utils::output
	{
	namespace typeless
		{
		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::rect<T>& aabb)
			{
			namespace ucc = utils::console::colour;

			os  << ucc::brace << "(" 
				<< ucc::type << "left"  << ucc::separ << ": " << ucc::value << aabb.ll << ucc::separ << ", "
				<< ucc::type << "up"    << ucc::separ << ": " << ucc::value << aabb.up << ucc::separ << ", "
				<< ucc::type << "right" << ucc::separ << ": " << ucc::value << aabb.rr << ucc::separ << ", "
				<< ucc::type << "down"  << ucc::separ << ": " << ucc::value << aabb.dw << ucc::separ
				<< ucc::brace << ")";
			return os;
			}
		}

	template <typename T>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::rect<T>& aabb)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "rect" << ucc::brace << "<" << ucc::type << typeid(T).name() << ucc::brace << ">";
		return utils::output::typeless::operator<<(os, aabb);
		}
	}