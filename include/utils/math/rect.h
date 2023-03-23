#pragma once

#include "../memory.h"
#include "vec2.h"
#include "geometry/common/root.h"

namespace utils::math
	{
	template <typename T = float>
	struct rect : geometry::shape_with_vertices_base<rect<T>>
		{
		using value_type = T;
		using nonref_value_type = utils::remove_cvref_t<value_type>;
		
		template <std::convertible_to<value_type> T_oth> requires (!utils::concepts::reference<T>)
		static rect<T> from_possize(utils::math::vec2<T_oth> position, utils::math::vec2<T_oth> size) { return {.ll{position.x}, .up{position.y}, .rr{position.x + size.x}, .dw{position.y + size.y}}; }
		template <std::convertible_to<value_type> T_oth> requires (!utils::concepts::reference<T>)
		static rect<T> from_ul_dr  (utils::math::vec2<T_oth> ul      , utils::math::vec2<T_oth> dr  ) { return {.ll{ul      .x}, .up{ul      .y}, .rr{dr      .x         }, .dw{dr      .y         }}; }

		template <std::same_as<nonref_value_type> T_oth> requires (utils::concepts::reference<T>)
		static rect<T> from_possize(utils::math::vec2<T_oth> position, utils::math::vec2<T_oth> size) { return {.ll{position.x}, .up{position.y}, .rr{position.x + size.x}, .dw{position.y + size.y}}; }
		template <std::same_as<nonref_value_type> T_oth> requires (utils::concepts::reference<T>)
		static rect<T> from_ul_dr  (utils::math::vec2<T_oth> ul      , utils::math::vec2<T_oth> dr  ) { return {.ll{ul      .x}, .up{ul      .y}, .rr{dr      .x         }, .dw{dr      .y         }}; }

#pragma region Variables
		value_type ll{0};
		value_type up{0};
		value_type rr{0};
		value_type dw{0};
#pragma endregion Variables

#pragma region Proxies
	#pragma region Position
		class p_proxy;
		class x_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class p_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.ll; }
				x_proxy& operator=(const nonref_value_type& new_value) noexcept
					{
					T previous_width{r.width()};
					r.ll = new_value; 
					r.width() = previous_width; 
					return *this; 
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				x_proxy& operator+=(const nonref_value_type& delta) noexcept { return (*this) = operator+(delta); }
				x_proxy& operator-=(const nonref_value_type& delta) noexcept { return (*this) = operator-(delta); }

			private:
				x_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class y_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class p_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.up; }
				y_proxy& operator=(const nonref_value_type& new_value) noexcept
					{
					nonref_value_type previous_height{r.height()};
					r.up = new_value;
					r.height() = previous_height;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				y_proxy& operator+=(const nonref_value_type& delta) noexcept { return (*this) = operator+(delta); }
				y_proxy& operator-=(const nonref_value_type& delta) noexcept { return (*this) = operator-(delta); }

			private:
				y_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class p_proxy
			{
			template <typename T2>
			friend struct rect;

			public:
				x_proxy x;
				y_proxy y;

				operator vec2<nonref_value_type>() const noexcept { return {r.ul()}; }
				p_proxy& operator=(const vec2<nonref_value_type>& new_value) noexcept
					{
					x = new_value.x;
					y = new_value.y;
					return *this;
					}

				vec2<nonref_value_type> operator+(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) + delta; }
				vec2<nonref_value_type> operator-(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) - delta; }

				p_proxy& operator+=(const vec2<nonref_value_type>& delta) noexcept { return (*this) = operator+(delta); }
				p_proxy& operator-=(const vec2<nonref_value_type>& delta) noexcept { return (*this) = operator-(delta); }

			private:
				p_proxy(rect<T>& r) : x{r}, y{r}, r{r} {}
				rect<T>& r;
			};

		class const_p_proxy;
		class const_x_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class p_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.ll; }

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

			private:
				const_x_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_y_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class const_p_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.up; }

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

			private:
				const_y_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_p_proxy
			{
			template <typename T2>
			friend struct rect;

			public:
				x_proxy x;
				y_proxy y;

				operator vec2<nonref_value_type>() const noexcept { return {r.ul()}; }

				vec2<nonref_value_type> operator+(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) + delta; }
				vec2<nonref_value_type> operator-(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) - delta; }

			private:
				const_p_proxy(const rect<T>& r) : x{r}, y{r}, r{r} {}
				const rect<T>& r;
			};

	#pragma endregion Position
	#pragma region Size
		class s_proxy;
		class w_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class s_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.rr - r.ll; }
				w_proxy& operator=(const nonref_value_type& new_value) noexcept
					{
					r.rr = r.ll + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				w_proxy& operator+=(const nonref_value_type& delta) noexcept { return (*this) = operator+(delta); }
				w_proxy& operator-=(const nonref_value_type& delta) noexcept { return (*this) = operator-(delta); }

			private:
				w_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class h_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class s_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.dw - r.up; }
				h_proxy& operator=(const nonref_value_type& new_value) noexcept
					{
					r.dw = r.up + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				h_proxy& operator+=(const nonref_value_type& delta) noexcept { return (*this) = operator+(delta); }
				h_proxy& operator-=(const nonref_value_type& delta) noexcept { return (*this) = operator-(delta); }

			private:
				h_proxy(rect<T>& r) : r{r} {}
				rect<T>& r;
			};
		class s_proxy
			{
			template <typename T2>
			friend struct rect;

			public:
				w_proxy w;
				h_proxy h;

				operator vec2<nonref_value_type>() const noexcept { return r.dr() - r.ul(); }
				s_proxy& operator=(const vec2<nonref_value_type>& new_value) noexcept
					{
					w = new_value.x;
					h = new_value.y;
					return *this;
					}

				vec2<nonref_value_type> operator+(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) + delta; }
				vec2<nonref_value_type> operator-(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) - delta; }

				s_proxy& operator+=(const vec2<nonref_value_type>& delta) noexcept { return (*this) = operator+(delta); }
				s_proxy& operator-=(const vec2<nonref_value_type>& delta) noexcept { return (*this) = operator-(delta); }

			private:
				s_proxy(rect<T>& r) : w{r}, h{r}, r{r} {}
				rect<T>& r;
			};
		class const_s_proxy;
		class const_w_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class const_s_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.rr - r.ll; }
				const_w_proxy& operator=(const nonref_value_type& new_value) noexcept
					{
					r.rr = r.ll + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

			private:
				const_w_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_h_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class const_s_proxy;

			public:
				operator nonref_value_type() const noexcept { return r.dw - r.up; }

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

			private:
				const_h_proxy(const rect<T>& r) : r{r} {}
				const rect<T>& r;
			};
		class const_s_proxy
			{
			template <typename T2>
			friend struct rect;

			public:
				const_w_proxy w;
				const_h_proxy h;

				operator vec2<nonref_value_type>() const noexcept { return r.dr() - r.ul(); }

				vec2<nonref_value_type> operator+(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) + delta; }
				vec2<nonref_value_type> operator-(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) - delta; }

			private:
				const_s_proxy(const rect<T>& r) : w{r}, h{r}, r{r} {}
				const rect<T>& r;
			};
	#pragma endregion Size
#pragma endregion Proxies

		bool operator==(const rect& other) const noexcept = default;

#pragma region Accessors
		// Corners
		value_type get_up(                   ) const noexcept { return up; }
		value_type get_dw(                   ) const noexcept { return dw; }
		value_type get_rr(                   ) const noexcept { return rr; }
		value_type get_ll(                   ) const noexcept { return ll; }

		void  set_up(nonref_value_type v) noexcept { up = v; }
		void  set_dw(nonref_value_type v) noexcept { dw = v; }
		void  set_rr(nonref_value_type v) noexcept { rr = v; }
		void  set_ll(nonref_value_type v) noexcept { ll = v; }

		const vec2   <value_type       > get_ul() const noexcept { return {ll, up}; }
		const vec2   <value_type       > get_ur() const noexcept { return {rr, up}; }
		const vec2   <value_type       > get_dr() const noexcept { return {rr, dw}; }
		const vec2   <value_type       > get_dl() const noexcept { return {ll, dw}; }
		      vecref2<nonref_value_type> get_ul()       noexcept { return {ll, up}; }
		      vecref2<nonref_value_type> get_ur()       noexcept { return {rr, up}; }
		      vecref2<nonref_value_type> get_dr()       noexcept { return {rr, dw}; }
		      vecref2<nonref_value_type> get_dl()       noexcept { return {ll, dw}; }
		
		void set_ul(vec2<value_type> value) noexcept { ll = value.x; up = value.y; }
		void set_ur(vec2<value_type> value) noexcept { rr = value.x; up = value.y; }
		void set_dr(vec2<value_type> value) noexcept { rr = value.x; dw = value.y; }
		void set_dl(vec2<value_type> value) noexcept { ll = value.x; dw = value.y; }

		// Pos-size
		const_x_proxy  get_x () const noexcept { return {*this}; }
		const_y_proxy  get_y () const noexcept { return {*this}; }
		const_w_proxy  get_w () const noexcept { return {*this}; }
		const_h_proxy  get_h () const noexcept { return {*this}; }
		const_p_proxy  get_p () const noexcept { return {*this}; }
		const_s_proxy  get_s () const noexcept { return {*this}; }
		      x_proxy  get_x ()       noexcept { return {*this}; }
		      y_proxy  get_y ()       noexcept { return {*this}; }
		      w_proxy  get_w ()       noexcept { return {*this}; }
		      h_proxy  get_h ()       noexcept { return {*this}; }
		      p_proxy  get_p ()       noexcept { return {*this}; }
		      s_proxy  get_s ()       noexcept { return {*this}; }

		      void     set_x (     nonref_value_type  value) noexcept { nonref_value_type tmp = get_w(); ll = value; set_w(tmp); }
		      void     set_y (     nonref_value_type  value) noexcept { nonref_value_type tmp = get_h(); up = value; set_h(tmp); }
		      void     set_h (     nonref_value_type  value) noexcept { dw = up + value; }
		      void     set_w (     nonref_value_type  value) noexcept { rr = ll + value; }
		      void     set_s (vec2<nonref_value_type> value) noexcept { set_w(value.x); set_h(value.y); }
		      void     set_p (vec2<nonref_value_type> value) noexcept 
			{
			vec2<nonref_value_type> prev_s{get_s()};
			set_x(value.x); set_y(value.y); 
			set_s(prev_s);
			}

		// Center
		     nonref_value_type  get_center_x()        const noexcept { return ll + (w() / nonref_value_type{2}); }
		     nonref_value_type  get_center_y()        const noexcept { return up + (h() / nonref_value_type{2}); }
		vec2<nonref_value_type> get_center  ()        const noexcept { return {get_center_x(), get_center_y()}; }
		//TODO void    set_center_x(T value) { auto previous_w{get_w()}; }
		//TODO void    set_center_x(T value)
		//TODO set_center() 
#pragma endregion Accessors

#pragma region Properties
		// Aliases
		__declspec(property(get = get_up, put = set_up)) value_type top;
		__declspec(property(get = get_dw, put = set_dw)) value_type down;
		__declspec(property(get = get_dw, put = set_dw)) value_type bottom;
		__declspec(property(get = get_ll, put = set_ll)) value_type left;
		__declspec(property(get = get_rr, put = set_rr)) value_type right;

		// Corners
		const vec2   <       value_type> ul          () const noexcept { return  get_ul() ; }
		const vec2   <       value_type> up_left     () const noexcept { return  get_ul() ; }
		const vec2   <       value_type> top_left    () const noexcept { return  get_ul() ; }
		const vec2   <       value_type> ur          () const noexcept { return  get_ur() ; }
		const vec2   <       value_type> up_right    () const noexcept { return  get_ur() ; }
		const vec2   <       value_type> top_right   () const noexcept { return  get_ur() ; }
		const vec2   <       value_type> dr          () const noexcept { return  get_dr() ; }
		const vec2   <       value_type> down_right  () const noexcept { return  get_dr() ; }
		const vec2   <       value_type> bottom_right() const noexcept { return  get_dr() ; }
		const vec2   <       value_type> dl          () const noexcept { return  get_dl() ; }
		const vec2   <       value_type> down_left   () const noexcept { return  get_dl() ; }
		const vec2   <       value_type> bottom_left () const noexcept { return  get_dl() ; }
		      vecref2<nonref_value_type> ul          ()       noexcept { return {get_ul()}; }
		      vecref2<nonref_value_type> up_left     ()       noexcept { return {get_ul()}; }
		      vecref2<nonref_value_type> top_left    ()       noexcept { return {get_ul()}; }
		      vecref2<nonref_value_type> ur          ()       noexcept { return {get_ur()}; }
		      vecref2<nonref_value_type> up_right    ()       noexcept { return {get_ur()}; }
		      vecref2<nonref_value_type> top_right   ()       noexcept { return {get_ur()}; }
		      vecref2<nonref_value_type> dr          ()       noexcept { return {get_dr()}; }
		      vecref2<nonref_value_type> down_right  ()       noexcept { return {get_dr()}; }
		      vecref2<nonref_value_type> bottom_right()       noexcept { return {get_dr()}; }
		      vecref2<nonref_value_type> dl          ()       noexcept { return {get_dl()}; }
		      vecref2<nonref_value_type> down_left   ()       noexcept { return {get_dl()}; }
		      vecref2<nonref_value_type> bottom_left ()       noexcept { return {get_dl()}; }

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
		__declspec(property(get = get_center)) vec2<nonref_value_type> center;

#pragma endregion Properties

		bool contains(vec2<nonref_value_type> point) const noexcept { return point.x >= ll && point.x <= rr && point.y >= up && point.y <= dw; }
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