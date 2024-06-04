#pragma once

#include "../memory.h"
#include "vec2.h"

namespace utils::math
	{
	template <typename T = float>
	struct rect
		{
		using self_t = rect<T>;
		using value_type = T;
		using nonref_value_type = utils::remove_reference_t<value_type>;
		
		template <std::convertible_to<value_type> T_oth> requires (!utils::concepts::reference<value_type>)
		static self_t from_possize(utils::math::vec2<T_oth> position, utils::math::vec2<T_oth> size) { return {.ll{position.x}, .up{position.y}, .rr{position.x + size.x}, .dw{position.y + size.y}}; }
		template <std::convertible_to<value_type> T_oth> requires (!utils::concepts::reference<value_type>)
		static self_t from_ul_dr  (utils::math::vec2<T_oth> ul      , utils::math::vec2<T_oth> dr  ) { return {.ll{ul      .x}, .up{ul      .y}, .rr{dr      .x         }, .dw{dr      .y         }}; }

		template <std::same_as<nonref_value_type> T_oth> requires (utils::concepts::reference<value_type>)
		static self_t from_possize(utils::math::vec2<T_oth> position, utils::math::vec2<T_oth> size) { return {.ll{position.x}, .up{position.y}, .rr{position.x + size.x}, .dw{position.y + size.y}}; }
		template <std::same_as<nonref_value_type> T_oth> requires (utils::concepts::reference<value_type>)
		static self_t from_ul_dr  (utils::math::vec2<T_oth> ul      , utils::math::vec2<T_oth> dr  ) { return {.ll{ul      .x}, .up{ul      .y}, .rr{dr      .x         }, .dw{dr      .y         }}; }

#pragma region Variables
		value_type ll;
		value_type up;
		value_type rr;
		value_type dw;
#pragma endregion Variables

#pragma region Proxies
	#pragma region Position
		template <bool is_const>
		class p_proxy;
		template <bool is_const>
		class x_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class p_proxy<is_const>;

			public:
				operator nonref_value_type () const noexcept { return r.ll; }
				operator nonref_value_type&() noexcept requires(!is_const) { return utils::remove_reference_v(r.ll); }

				x_proxy& operator=(const nonref_value_type& new_value) noexcept 
					requires(!is_const)
					{
					nonref_value_type previous_width{r.width()};
					remove_reference_v(r.ll) = new_value;
					r.width() = previous_width; 
					return *this; 
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return utils::remove_reference_v(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return utils::remove_reference_v(*this) - delta; }

				x_proxy& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				x_proxy& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				x_proxy(self_t& r) : r{r} {}
				self_t& r;
			};
		template <bool is_const>
		class y_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class p_proxy<is_const>;

			public:
				operator nonref_value_type() const noexcept { return r.up; }
				operator nonref_value_type& () noexcept requires(!is_const) { return utils::remove_reference_v(r.up); }

				y_proxy& operator=(const nonref_value_type& new_value) noexcept
					requires(!is_const)
					{
					nonref_value_type previous_height{r.height()};
					remove_reference_v(r.up) = new_value;
					r.height() = previous_height;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return utils::remove_reference_v(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return utils::remove_reference_v(*this) - delta; }

				y_proxy& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				y_proxy& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				y_proxy(self_t& r) : r{r} {}
				self_t& r;
			};
		template <bool is_const>
		class p_proxy
			{
			template <typename T2>
			friend struct rect;

			public:
				x_proxy<is_const> x;
				y_proxy<is_const> y;

				operator vec2<nonref_value_type>() const noexcept { return {r.ul()}; }
				operator vecref2<nonref_value_type>() noexcept { return {r.ul()}; }

				p_proxy& operator=(const vec2<nonref_value_type>& new_value) noexcept
					requires(!is_const)
					{
					x = new_value.x;
					y = new_value.y;
					return *this;
					}

				vec2<nonref_value_type> operator+(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) + delta; }
				vec2<nonref_value_type> operator-(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) - delta; }

				p_proxy& operator+=(const vec2<nonref_value_type>& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				p_proxy& operator-=(const vec2<nonref_value_type>& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				p_proxy(self_t& r) : x{r}, y{r}, r{r} {}
				self_t& r;
			};
	#pragma endregion Position
	#pragma region Size
		template <bool is_const>
		class s_proxy;
		template <bool is_const>
		class w_proxy
			{
			template <typename T2>
			friend struct rect;
			friend class s_proxy<is_const>;

			public:
				operator nonref_value_type() const noexcept { return r.rr - r.ll; }

				w_proxy& operator=(const nonref_value_type& new_value) noexcept
					requires(!is_const)
					{
					remove_reference_v(r.rr) = r.ll + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				w_proxy& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				w_proxy& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				w_proxy(self_t& r) : r{r} {}
				self_t& r;
			};
		template <bool is_const>
		class h_proxy 
			{
			template <typename T2>
			friend struct rect;
			friend class s_proxy<is_const>;

			public:
				operator nonref_value_type() const noexcept { return r.dw - r.up; }

				h_proxy& operator=(const nonref_value_type& new_value) noexcept 
					requires(!is_const)
					{
					utils::remove_reference_v(r.dw) = r.up + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				h_proxy& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				h_proxy& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				h_proxy(self_t& r) : r{r} {}
				self_t& r;
			};
		template <bool is_const>
		class s_proxy
			{
			template <typename T2>
			friend struct rect;

			public:
				w_proxy<is_const> w;
				h_proxy<is_const> h;

				operator vec2<nonref_value_type>() const noexcept { return r.dr() - r.ul(); }

				s_proxy& operator=(const vec2<nonref_value_type>& new_value) noexcept requires(!is_const)
					{
					w = new_value.x;
					h = new_value.y;
					return *this;
					}

				vec2<nonref_value_type> operator+(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) + delta; }
				vec2<nonref_value_type> operator-(const vec2<nonref_value_type>& delta) const noexcept { return static_cast<vec2<nonref_value_type>>(*this) - delta; }

				s_proxy& operator+=(const vec2<nonref_value_type>& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				s_proxy& operator-=(const vec2<nonref_value_type>& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				s_proxy(self_t& r) : w{r}, h{r}, r{r} {}
				self_t& r;
			};
	#pragma endregion Size
#pragma endregion Proxies

		bool operator==(const rect& other) const noexcept = default;

#pragma region Accessors
		// Corners
		auto  get_up() const noexcept { return utils::remove_reference_v(up); }
		auto  get_dw() const noexcept { return utils::remove_reference_v(dw); }
		auto  get_rr() const noexcept { return utils::remove_reference_v(rr); }
		auto  get_ll() const noexcept { return utils::remove_reference_v(ll); }
		auto& get_up()       noexcept { return utils::remove_reference_v(up); }
		auto& get_dw()       noexcept { return utils::remove_reference_v(dw); }
		auto& get_rr()       noexcept { return utils::remove_reference_v(rr); }
		auto& get_ll()       noexcept { return utils::remove_reference_v(ll); }

		void  set_up(nonref_value_type v) noexcept { utils::remove_reference_v(up) = v; }
		void  set_dw(nonref_value_type v) noexcept { utils::remove_reference_v(dw) = v; }
		void  set_rr(nonref_value_type v) noexcept { utils::remove_reference_v(rr) = v; }
		void  set_ll(nonref_value_type v) noexcept { utils::remove_reference_v(ll) = v; }

		const vec2   <nonref_value_type> get_ul() const noexcept { return {utils::remove_reference_v(ll), utils::remove_reference_v(up)}; }
		const vec2   <nonref_value_type> get_ur() const noexcept { return {utils::remove_reference_v(rr), utils::remove_reference_v(up)}; }
		const vec2   <nonref_value_type> get_dr() const noexcept { return {utils::remove_reference_v(rr), utils::remove_reference_v(dw)}; }
		const vec2   <nonref_value_type> get_dl() const noexcept { return {utils::remove_reference_v(ll), utils::remove_reference_v(dw)}; }
		      vecref2<nonref_value_type> get_ul()       noexcept { return {utils::remove_reference_v(ll), utils::remove_reference_v(up)}; }
		      vecref2<nonref_value_type> get_ur()       noexcept { return {utils::remove_reference_v(rr), utils::remove_reference_v(up)}; }
		      vecref2<nonref_value_type> get_dr()       noexcept { return {utils::remove_reference_v(rr), utils::remove_reference_v(dw)}; }
		      vecref2<nonref_value_type> get_dl()       noexcept { return {utils::remove_reference_v(ll), utils::remove_reference_v(dw)}; }
		
		void set_ul(vec2<nonref_value_type> value) noexcept { utils::remove_reference_v(ll) = value.x; utils::remove_reference_v(up) = value.y; }
		void set_ur(vec2<nonref_value_type> value) noexcept { utils::remove_reference_v(rr) = value.x; utils::remove_reference_v(up) = value.y; }
		void set_dr(vec2<nonref_value_type> value) noexcept { utils::remove_reference_v(rr) = value.x; utils::remove_reference_v(dw) = value.y; }
		void set_dl(vec2<nonref_value_type> value) noexcept { utils::remove_reference_v(ll) = value.x; utils::remove_reference_v(dw) = value.y; }

		// Pos-size
		x_proxy<true > get_x () const noexcept { return {*this}; }
		y_proxy<true > get_y () const noexcept { return {*this}; }
		w_proxy<true > get_w () const noexcept { return {*this}; }
		h_proxy<true > get_h () const noexcept { return {*this}; }
		p_proxy<true > get_p () const noexcept { return {*this}; }
		s_proxy<true > get_s () const noexcept { return {*this}; }
		x_proxy<false> get_x ()       noexcept requires(!std::is_const<nonref_value_type>) { return {*this}; }
		y_proxy<false> get_y ()       noexcept requires(!std::is_const<nonref_value_type>) { return {*this}; }
		w_proxy<false> get_w ()       noexcept requires(!std::is_const<nonref_value_type>) { return {*this}; }
		h_proxy<false> get_h ()       noexcept requires(!std::is_const<nonref_value_type>) { return {*this}; }
		p_proxy<false> get_p ()       noexcept requires(!std::is_const<nonref_value_type>) { return {*this}; }
		s_proxy<false> get_s ()       noexcept requires(!std::is_const<nonref_value_type>) { return {*this}; }

		      void     set_x (     nonref_value_type  value) noexcept { nonref_value_type tmp = get_w(); remove_reference_v(ll) = value; set_w(tmp); }
		      void     set_y (     nonref_value_type  value) noexcept { nonref_value_type tmp = get_h(); remove_reference_v(up) = value; set_h(tmp); }
		      void     set_h (     nonref_value_type  value) noexcept { remove_reference_v(dw) = up + value; }
		      void     set_w (     nonref_value_type  value) noexcept { remove_reference_v(rr) = ll + value; }
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
		void set_center_x(T value) noexcept { auto half_w{get_w() / nonref_value_type{2}}; remove_reference_v(ll) = value - half_w; remove_reference_v(rr) = value + half_w; }
		void set_center_y(T value){ auto half_h{get_h() / nonref_value_type{2}}; remove_reference_v(up) = value - half_h; remove_reference_v(dw) = value + half_h; }
		void set_center(vec2<nonref_value_type> value) noexcept { set_center_x(value.x); set_center_y(value.y); }
#pragma endregion Accessors

#pragma region Properties

		// Corners
		const vec2   <nonref_value_type> ul          () const noexcept { return  get_ul() ; }
		const vec2   <nonref_value_type> up_left     () const noexcept { return  get_ul() ; }
		const vec2   <nonref_value_type> top_left    () const noexcept { return  get_ul() ; }
		const vec2   <nonref_value_type> ur          () const noexcept { return  get_ur() ; }
		const vec2   <nonref_value_type> up_right    () const noexcept { return  get_ur() ; }
		const vec2   <nonref_value_type> top_right   () const noexcept { return  get_ur() ; }
		const vec2   <nonref_value_type> dr          () const noexcept { return  get_dr() ; }
		const vec2   <nonref_value_type> down_right  () const noexcept { return  get_dr() ; }
		const vec2   <nonref_value_type> bottom_right() const noexcept { return  get_dr() ; }
		const vec2   <nonref_value_type> dl          () const noexcept { return  get_dl() ; }
		const vec2   <nonref_value_type> down_left   () const noexcept { return  get_dl() ; }
		const vec2   <nonref_value_type> bottom_left () const noexcept { return  get_dl() ; }
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
		x_proxy<true >  x           () const noexcept { return get_x (); }
		y_proxy<true >  y           () const noexcept { return get_y (); }
		h_proxy<true >  h           () const noexcept { return get_h (); }
		h_proxy<true >  height      () const noexcept { return get_h (); }
		w_proxy<true >  w           () const noexcept { return get_w (); }
		w_proxy<true >  width       () const noexcept { return get_w (); }
		s_proxy<true >  s           () const noexcept { return get_s (); }
		s_proxy<true >  size        () const noexcept { return get_s (); }
		p_proxy<true >  p           () const noexcept { return get_p (); }
		p_proxy<true >  pos         () const noexcept { return get_p (); }
		p_proxy<true >  position    () const noexcept { return get_p (); }
		x_proxy<false>  x           ()       noexcept { return get_x (); }
		y_proxy<false>  y           ()       noexcept { return get_y (); }
		h_proxy<false>  h           ()       noexcept { return get_h (); }
		h_proxy<false>  height      ()       noexcept { return get_h (); }
		w_proxy<false>  w           ()       noexcept { return get_w (); }
		w_proxy<false>  width       ()       noexcept { return get_w (); }
		s_proxy<false>  s           ()       noexcept { return get_s (); }
		s_proxy<false>  size        ()       noexcept { return get_s (); }
		p_proxy<false>  p           ()       noexcept { return get_p (); }
		p_proxy<false>  pos         ()       noexcept { return get_p (); }
		p_proxy<false>  position    ()       noexcept { return get_p (); }

		// Center
		__declspec(property(get = get_center)) vec2<nonref_value_type> center;

#pragma endregion Properties
		

		//using geometry::shape_base<self_t>::closest_point_and_distance;
		//using geometry::shape_base<self_t>::closest_point_to;
		//using geometry::shape_base<self_t>::distance_min;
		//using geometry::shape_base<self_t>::vector_to;
		//using geometry::shape_base<self_t>::intersects;
		//using geometry::shape_base<self_t>::intersection;
		//using geometry::shape_base<self_t>::contains;
		//using geometry::shape_base<self_t>::collides_with;
		//
		//vec2f closest_point_to(const geometry::point& other) const noexcept;
		//float                distance_min    (const geometry::point& b) const noexcept;
		//bool                 intersects      (const geometry::point& other) const noexcept;
		//std::optional<vec2f> intersection    (const geometry::point& other) const noexcept;
		////bool                 contains        (const geometry::point& other) const noexcept;
		//
		//geometry::closest_point_and_distance_t closest_point_and_distance(const geometry::segment& other) const noexcept;
		//bool                 intersects      (const geometry::segment& other) const noexcept;
		//std::optional<vec2f> intersection    (const geometry::segment& other) const noexcept;
		//bool                 contains        (const geometry::segment& other) const noexcept;
		//
		//geometry::closest_point_and_distance_t closest_point_and_distance(const geometry::aabb& other) const noexcept;
		//bool                 intersects      (const geometry::aabb& other) const noexcept;
		//std::optional<vec2f> intersection    (const geometry::aabb& other) const noexcept;
		//bool                 contains        (const geometry::aabb& other) const noexcept;
		//
		//geometry::closest_point_and_distance_t closest_point_and_distance(const geometry::polygon& other) const noexcept;
		//bool                 intersects      (const geometry::polygon& other) const noexcept;
		//std::optional<vec2f> intersection    (const geometry::polygon& other) const noexcept;
		//bool                 contains        (const geometry::polygon& other) const noexcept;
		//
		//geometry::closest_point_and_distance_t closest_point_and_distance(const geometry::circle& other) const noexcept;
		//bool                 intersects      (const geometry::circle& other) const noexcept;
		//std::optional<vec2f> intersection    (const geometry::circle& other) const noexcept;
		//bool                 contains        (const geometry::circle& other) const noexcept;
		//
		//auto get_edges()       noexcept;
		//auto get_edges() const noexcept;
		//
		//self_t& scale_self    (const float      & scaling    ) noexcept;
		//self_t& rotate_self   (const angle::radf& rotation   ) noexcept;
		//self_t& translate_self(const vec2f      & translation) noexcept;
		//
		//self_t bounding_box() const noexcept { return *this; }

		template <std::convertible_to<nonref_value_type> point_value_type>
		bool contains(const vec2<point_value_type>& point) const noexcept { return point.x >= ll && point.x <= rr && point.y >= up && point.y <= dw; }

		
		#pragma region geometry shape methods
			utils_gpu_available constexpr rect<value_type>  scale         (const float                    & scaling    ) const noexcept { auto ret{*this}; return ret.scale_self    (scaling    ); }
			utils_gpu_available constexpr rect<value_type>  rotate        (const angle::base<float, 360.f>& rotation   ) const noexcept { auto ret{*this}; return ret.rotate_self   (rotation   ); }
			utils_gpu_available constexpr rect<value_type>  translate     (const vec<float, 2>            & translation) const noexcept { auto ret{*this}; return ret.translate_self(translation); }
			utils_gpu_available constexpr rect<value_type>  transform     (const transform2               & transform  ) const noexcept { auto ret{*this}; return ret.transform_self(transform  ); }

			utils_gpu_available constexpr rect<value_type>& scale_self    (const float      & scaling    ) noexcept;
			utils_gpu_available constexpr rect<value_type>& rotate_self   (const angle::degf& rotation   ) noexcept;
			utils_gpu_available constexpr rect<value_type>& translate_self(const vec2f      & translation) noexcept;
			utils_gpu_available constexpr rect<value_type>& transform_self(const transform2 & transform  ) noexcept;

			utils_gpu_available constexpr rect<float> bounding_box() const noexcept;
		#pragma endregion geometry shape methods
		};
	}



//namespace utils::output
//	{
//	namespace typeless
//		{
//		template <typename T>
//		inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::rect<T>& aabb)
//			{
//			namespace ucc = utils::console::colour;
//
//			os  << ucc::brace << "(" 
//				<< ucc::type << "left"  << ucc::separ << ": " << ucc::value << aabb.ll << ucc::separ << ", "
//				<< ucc::type << "up"    << ucc::separ << ": " << ucc::value << aabb.up << ucc::separ << ", "
//				<< ucc::type << "right" << ucc::separ << ": " << ucc::value << aabb.rr << ucc::separ << ", "
//				<< ucc::type << "down"  << ucc::separ << ": " << ucc::value << aabb.dw << ucc::separ
//				<< ucc::brace << ")";
//			return os;
//			}
//		}
//
//	template <typename T>
//	inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::rect<T>& aabb)
//		{
//		namespace ucc = utils::console::colour;
//		os << ucc::type << "rect" << ucc::brace << "<" << ucc::type << typeid(T).name() << ucc::brace << ">";
//		return utils::output::typeless::operator<<(os, aabb);
//		}
//
//	//TODO understand why not working
//	template <typename T> 
//	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::x_proxy& proxy) { return os << static_cast<T>(proxy); }
//	template <typename T> 
//	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::y_proxy& proxy) { return os << static_cast<T>(proxy); }
//	template <typename T> 
//	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::p_proxy& proxy) { return os << static_cast<utils::math::vec2<T>>(proxy); }
//	template <typename T> 
//	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::w_proxy& proxy) { return os << static_cast<T>(proxy); }
//	template <typename T> 
//	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::h_proxy& proxy) { return os << static_cast<T>(proxy); }
//	template <typename T> 
//	inline ::std::ostream& operator<<(::std::ostream& os, const typename utils::math::rect<T>::s_proxy& proxy) { return os << static_cast<utils::math::vec2<T>>(proxy); }
//	}