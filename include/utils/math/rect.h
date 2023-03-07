#pragma once

#include "vec2.h"

namespace utils::math
	{
	template <typename T = float>
	struct rect
		{
#pragma region Variables
		T ll{0};
		T up{0};
		T rr{0};
		T dw{0};
#pragma endregion Variables

#pragma region Proxies
	#pragma region Position
		class p_proxy;
		class x_proxy
			{
			template <typename T>
			friend struct rect;
			friend class p_proxy;

			public:
				operator T() const noexcept { return r.ll; }
				x_proxy& operator=(const T& new_value) noexcept 
					{
					T previous_width{r.width()};
					r.ll = new_value; 
					r.width() = previous_width; 
					return *this; 
					}

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

				x_proxy& operator+=(const T& delta) noexcept { return (*this) = operator+(delta); }
				x_proxy& operator-=(const T& delta) noexcept { return (*this) = operator-(delta); }

			private:
				x_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class y_proxy
			{
			template <typename T>
			friend struct rect;
			friend class p_proxy;

			public:
				operator T() const noexcept { return r.up; }
				y_proxy& operator=(const T& new_value) noexcept
					{
					T previous_height{r.height()};
					r.up = new_value;
					r.height() = previous_height;
					return *this;
					}

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

				y_proxy& operator+=(const T& delta) noexcept { return (*this) = operator+(delta); }
				y_proxy& operator-=(const T& delta) noexcept { return (*this) = operator-(delta); }

			private:
				y_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class p_proxy
			{
			template <typename T>
			friend struct rect;

			public:
				x_proxy x;
				y_proxy y;

				operator vec2<T>() const noexcept { return {r.ul()}; }
				p_proxy& operator=(const vec2<T>& new_value) noexcept
					{
					x = new_value.x;
					y = new_value.y;
					return *this;
					}

				vec2<T> operator+(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) + delta; }
				vec2<T> operator-(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) - delta; }

				p_proxy& operator+=(const vec2<T>& delta) noexcept { return (*this) = operator+(delta); }
				p_proxy& operator-=(const vec2<T>& delta) noexcept { return (*this) = operator-(delta); }

			private:
				p_proxy(rect<T>& r) : x{r}, y{r}, r{r} {}
				rect<T>& r;
			};

		class const_p_proxy;
		class const_x_proxy
			{
			template <typename T>
			friend struct rect;
			friend class p_proxy;

			public:
				operator T() const noexcept { return r.ll; }

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

			private:
				const_x_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_y_proxy
			{
			template <typename T>
			friend struct rect;
			friend class const_p_proxy;

			public:
				operator T() const noexcept { return r.up; }

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

			private:
				const_y_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_p_proxy
			{
			template <typename T>
			friend struct rect;

			public:
				x_proxy x;
				y_proxy y;

				operator vec2<T>() const noexcept { return {r.ul()}; }

				vec2<T> operator+(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) + delta; }
				vec2<T> operator-(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) - delta; }

			private:
				const_p_proxy(const rect<T>& r) : x{r}, y{r}, r{r} {}
				const rect<T>& r;
			};

	#pragma endregion Position
	#pragma region Size
		class s_proxy;
		class w_proxy
			{
			template <typename T>
			friend struct rect;
			friend class s_proxy;

			public:
				operator T() const noexcept { return r.rr - r.ll; }
				w_proxy& operator=(const T& new_value) noexcept
					{
					r.rr = r.ll + new_value;
					return *this;
					}

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

				w_proxy& operator+=(const T& delta) noexcept { return (*this) = operator+(delta); }
				w_proxy& operator-=(const T& delta) noexcept { return (*this) = operator-(delta); }

			private:
				w_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class h_proxy
			{
			template <typename T>
			friend struct rect;
			friend class s_proxy;

			public:
				operator T() const noexcept { return r.dw - r.up; }
				h_proxy& operator=(const T& new_value) noexcept
					{
					r.dw = r.up + new_value;
					return *this;
					}

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

				h_proxy& operator+=(const T& delta) noexcept { return (*this) = operator+(delta); }
				h_proxy& operator-=(const T& delta) noexcept { return (*this) = operator-(delta); }

			private:
				h_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class s_proxy
			{
			template <typename T>
			friend struct rect;

			public:
				w_proxy w;
				h_proxy h;

				operator vec2<T>() const noexcept { return r.dr() - r.ul(); }
				s_proxy& operator=(const vec2<T>& new_value) noexcept
					{
					w = new_value.x;
					h = new_value.y;
					return *this;
					}

				vec2<T> operator+(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) + delta; }
				vec2<T> operator-(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) - delta; }

				s_proxy& operator+=(const vec2<T>& delta) noexcept { return (*this) = operator+(delta); }
				s_proxy& operator-=(const vec2<T>& delta) noexcept { return (*this) = operator-(delta); }

			private:
				s_proxy(rect<T>& r) : w{r}, h{r}, r{r} {}
				rect<T>& r;
			};
		class const_s_proxy;
		class const_w_proxy
			{
			template <typename T>
			friend struct rect;
			friend class const_s_proxy;

			public:
				operator T() const noexcept { return r.rr - r.ll; }
				const_w_proxy& operator=(const T& new_value) noexcept
					{
					r.rr = r.ll + new_value;
					return *this;
					}

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

			private:
				const_w_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_h_proxy
			{
			template <typename T>
			friend struct rect;
			friend class const_s_proxy;

			public:
				operator T() const noexcept { return r.dw - r.up; }

				T operator+(const T& delta) const noexcept { return static_cast<T>(*this) + delta; }
				T operator-(const T& delta) const noexcept { return static_cast<T>(*this) - delta; }

			private:
				const_h_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_s_proxy
			{
			template <typename T>
			friend struct rect;

			public:
				const_w_proxy w;
				const_h_proxy h;

				operator vec2<T>() const noexcept { return r.dr() - r.ul(); }

				vec2<T> operator+(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) + delta; }
				vec2<T> operator-(const vec2<T>& delta) const noexcept { return static_cast<vec2<T>>(*this) - delta; }

			private:
				const_s_proxy(const rect<T>& r) : w{r}, h{r}, r{r} {}
				const rect<T>& r;
			};
	#pragma endregion Size
#pragma endregion Proxies

		bool operator==(const rect& other) const noexcept = default;

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

		const vec2   <T> get_ul()           const noexcept { return {ll, up}; }
		const vec2   <T> get_ur()           const noexcept { return {rr, up}; }
		const vec2   <T> get_dr()           const noexcept { return {rr, dw}; }
		const vec2   <T> get_dl()           const noexcept { return {ll, dw}; }
		      vecref2<T> get_ul()                 noexcept { return {ll, up}; }
		      vecref2<T> get_ur()                 noexcept { return {rr, up}; }
		      vecref2<T> get_dr()                 noexcept { return {rr, dw}; }
		      vecref2<T> get_dl()                 noexcept { return {ll, dw}; }
		      void       set_ul(vecref2<T> value) noexcept { ll = value.x; up = value.y; }
		      void       set_ur(vecref2<T> value) noexcept { rr = value.x; up = value.y; }
		      void       set_dr(vecref2<T> value) noexcept { rr = value.x; dw = value.y; }
		      void       set_dl(vecref2<T> value) noexcept { ll = value.x; dw = value.y; }

		// Pos-size
		const_x_proxy  get_x ()        const noexcept { return {*this}; }
		const_y_proxy  get_y ()        const noexcept { return {*this}; }
		const_w_proxy  get_w ()        const noexcept { return {*this}; }
		const_h_proxy  get_h ()        const noexcept { return {*this}; }
		const_p_proxy  get_p ()        const noexcept { return {*this}; }
		const_s_proxy  get_s ()        const noexcept { return {*this}; }
		      x_proxy  get_x ()              noexcept { return {*this}; }
		      y_proxy  get_y ()              noexcept { return {*this}; }
		      w_proxy  get_w ()              noexcept { return {*this}; }
		      h_proxy  get_h ()              noexcept { return {*this}; }
		      p_proxy  get_p ()              noexcept { return {*this}; }
		      s_proxy  get_s ()              noexcept { return {*this}; }
		      void     set_x (T       value) noexcept { T tmp = w; ll = value; w = tmp; }
		      void     set_y (T       value) noexcept { T tmp = h; up = value; h = tmp; }
		      void     set_h (T       value) noexcept { dw = up + value; }
		      void     set_w (T       value) noexcept { rr = ll + value; }
		      void     set_s (vec2<T> value) noexcept { w = value.x; h = value.y; }
		      void     set_p (vec2<T> value) noexcept 
			{
			vec2<T> prev_s{get_s()};
			x = value.x; y = value.y; 
			s = prev_s;
			}

		// Center
		T       get_center_x()        const noexcept { return ll + (w() / T{2}); }
		T       get_center_y()        const noexcept { return up + (h() / T{2}); }
		vec2<T> get_center  ()        const noexcept { return {get_center_x(), get_center_y()}; }
		//TODO void    set_center_x(T value) { auto previous_w{get_w()}; }
		//TODO void    set_center_x(T value)
		//TODO set_center() 
#pragma endregion Accessors

#pragma region Properties
		// Aliases
		__declspec(property(get = get_up, put = set_up)) T top;
		__declspec(property(get = get_dw, put = set_dw)) T down;
		__declspec(property(get = get_dw, put = set_dw)) T bottom;
		__declspec(property(get = get_ll, put = set_ll)) T left;
		__declspec(property(get = get_rr, put = set_rr)) T right;

		// Corners
		const vec2   <T> ul          () const noexcept { return  get_ul() ; }
		const vec2   <T> up_left     () const noexcept { return  get_ul() ; }
		const vec2   <T> top_left    () const noexcept { return  get_ul() ; }
		const vec2   <T> ur          () const noexcept { return  get_ur() ; }
		const vec2   <T> up_right    () const noexcept { return  get_ur() ; }
		const vec2   <T> top_right   () const noexcept { return  get_ur() ; }
		const vec2   <T> dr          () const noexcept { return  get_dr() ; }
		const vec2   <T> down_right  () const noexcept { return  get_dr() ; }
		const vec2   <T> bottom_right() const noexcept { return  get_dr() ; }
		const vec2   <T> dl          () const noexcept { return  get_dl() ; }
		const vec2   <T> down_left   () const noexcept { return  get_dl() ; }
		const vec2   <T> bottom_left () const noexcept { return  get_dl() ; }
		      vecref2<T> ul          ()       noexcept { return {get_ul()}; }
		      vecref2<T> up_left     ()       noexcept { return {get_ul()}; }
		      vecref2<T> top_left    ()       noexcept { return {get_ul()}; }
		      vecref2<T> ur          ()       noexcept { return {get_ur()}; }
		      vecref2<T> up_right    ()       noexcept { return {get_ur()}; }
		      vecref2<T> top_right   ()       noexcept { return {get_ur()}; }
		      vecref2<T> dr          ()       noexcept { return {get_dr()}; }
		      vecref2<T> down_right  ()       noexcept { return {get_dr()}; }
		      vecref2<T> bottom_right()       noexcept { return {get_dr()}; }
		      vecref2<T> dl          ()       noexcept { return {get_dl()}; }
		      vecref2<T> down_left   ()       noexcept { return {get_dl()}; }
		      vecref2<T> bottom_left ()       noexcept { return {get_dl()}; }

		// Pos-size
		const_x_proxy  x           () const noexcept { return get_x (); }
		const_y_proxy  y           () const noexcept { return get_y (); }
		const_h_proxy  h           () const noexcept { return get_h (); }
		const_h_proxy  height      () const noexcept { return get_h (); }
		const_w_proxy  w           () const noexcept { return get_w (); }
		const_w_proxy  width       () const noexcept { return get_w (); }
		const_s_proxy  s           () const noexcept { return get_s (); }
		const_s_proxy  size        () const noexcept { return get_s (); }
		const_p_proxy  p           () const noexcept { return get_p (); }
		const_p_proxy  pos         () const noexcept { return get_p (); }
		const_p_proxy  position    () const noexcept { return get_p (); }
		      x_proxy  x           ()       noexcept { return get_x (); }
		      y_proxy  y           ()       noexcept { return get_y (); }
		      h_proxy  h           ()       noexcept { return get_h (); }
		      h_proxy  height      ()       noexcept { return get_h (); }
		      w_proxy  w           ()       noexcept { return get_w (); }
		      w_proxy  width       ()       noexcept { return get_w (); }
		      s_proxy  s           ()       noexcept { return get_s (); }
		      s_proxy  size        ()       noexcept { return get_s (); }
		      p_proxy  p           ()       noexcept { return get_p (); }
		      p_proxy  pos         ()       noexcept { return get_p (); }
		      p_proxy  position    ()       noexcept { return get_p (); }

		// Center
		__declspec(property(get = get_center)) vec2<T> center;

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

	//TODO understand why not working
	template <typename T> 
	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::x_proxy& proxy) { return os << static_cast<T>(proxy); }
	template <typename T> 
	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::y_proxy& proxy) { return os << static_cast<T>(proxy); }
	template <typename T> 
	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::p_proxy& proxy) { return os << static_cast<utils::math::vec2<T>>(proxy); }
	template <typename T> 
	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::w_proxy& proxy) { return os << static_cast<T>(proxy); }
	template <typename T> 
	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::h_proxy& proxy) { return os << static_cast<T>(proxy); }
	template <typename T> 
	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::s_proxy& proxy) { return os << static_cast<utils::math::vec2<T>>(proxy); }
	}