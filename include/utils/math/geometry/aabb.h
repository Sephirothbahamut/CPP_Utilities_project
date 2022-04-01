#pragma once

#include "../vec2.h"

namespace utils::math::geometry
	{
	struct axis_aligned_bounding_box
		{
#pragma region Variables
		float ll;
		float up;
		float rr;
		float dw;
#pragma endregion Variables

#pragma region Accessors
		// Corners
		float get_up()  const noexcept { return up; }
		float get_dw()  const noexcept { return dw; }
		float get_rr()  const noexcept { return rr; }
		float get_ll()  const noexcept { return ll; }
		void  set_up(float v) noexcept { up = v; }
		void  set_dw(float v) noexcept { dw = v; }
		void  set_rr(float v) noexcept { rr = v; }
		void  set_ll(float v) noexcept { ll = v; }

		vec2f get_ul()      const noexcept { return {ll, up}; }
		vec2f get_ur()      const noexcept { return {rr, up}; }
		vec2f get_dr()      const noexcept { return {rr, dw}; }
		vec2f get_dl()      const noexcept { return {ll, dw}; }
		void  set_ul(vec2f value) noexcept { ll = value.x; up = value.y; }
		void  set_ur(vec2f value) noexcept { rr = value.x; up = value.y; }
		void  set_dr(vec2f value) noexcept { rr = value.x; dw = value.y; }
		void  set_dl(vec2f value) noexcept { ll = value.x; dw = value.y; }

		// Pos-size
		float get_x()      const noexcept { return ll; }
		float get_y()      const noexcept { return up; }
		float get_h()      const noexcept { return dw - up; }
		float get_w()      const noexcept { return rr - ll; }
		vec2f get_s()      const noexcept { return {w, h}; }
		vec2f get_p()      const noexcept { return ul; }
		void  set_x(float value) noexcept { float tmp = w; ll = value; w = tmp; }
		void  set_y(float value) noexcept { float tmp = h; up = value; h = tmp; }
		void  set_h(float value) noexcept { dw = up + value; }
		void  set_w(float value) noexcept { rr = ll + value; }
		void  set_s(vec2f value) noexcept { w = value.x; h = value.y; }
		void  set_p(vec2f value) noexcept { x = value.x; y = value.y; }
#pragma endregion Accessors

#pragma region Properties
		// Aliases
		__declspec(property(get = get_up, put = set_up)) float top;
		__declspec(property(get = get_dw, put = set_dw)) float down;
		__declspec(property(get = get_dw, put = set_dw)) float bottom;
		__declspec(property(get = get_ll, put = set_ll)) float left;
		__declspec(property(get = get_rr, put = set_rr)) float right;
		__declspec(property(get = get_ll, put = set_ll)) float x1;
		__declspec(property(get = get_rr, put = set_rr)) float x2;
		__declspec(property(get = get_up, put = set_up)) float y1;
		__declspec(property(get = get_dw, put = set_dw)) float y2;

		// Corners
		__declspec(property(get = get_ul, put = set_ul)) vec2f ul;
		__declspec(property(get = get_ul, put = set_ul)) vec2f up_left;
		__declspec(property(get = get_ul, put = set_ul)) vec2f top_left;
		__declspec(property(get = get_ur, put = set_ur)) vec2f ur;
		__declspec(property(get = get_ur, put = set_ur)) vec2f up_right;
		__declspec(property(get = get_ur, put = set_ur)) vec2f top_right;
		__declspec(property(get = get_dr, put = set_dr)) vec2f dr;
		__declspec(property(get = get_dr, put = set_dr)) vec2f down_right;
		__declspec(property(get = get_dr, put = set_dr)) vec2f bottom_right;
		__declspec(property(get = get_dl, put = set_dl)) vec2f dl;
		__declspec(property(get = get_dl, put = set_dl)) vec2f down_left;
		__declspec(property(get = get_dl, put = set_dl)) vec2f bottom_left;

		// Pos-size
		__declspec(property(get = get_x, put = set_x)) float x;
		__declspec(property(get = get_y, put = set_y)) float y;
		__declspec(property(get = get_h, put = set_h)) float h;
		__declspec(property(get = get_h, put = set_h)) float height;
		__declspec(property(get = get_w, put = set_w)) float w;
		__declspec(property(get = get_w, put = set_w)) float width;
		__declspec(property(get = get_s, put = set_s)) vec2f s;
		__declspec(property(get = get_s, put = set_s)) vec2f size;
		__declspec(property(get = get_p, put = set_p)) vec2f p;
		__declspec(property(get = get_p, put = set_p)) vec2f pos;
		__declspec(property(get = get_p, put = set_p)) vec2f position;
#pragma endregion Properties

		bool contains(vec2f point) const noexcept { return point.x >= ll && point.x <= rr && point.y >= up && point.y <= dw; }
		};
	using aabb = axis_aligned_bounding_box;
	}