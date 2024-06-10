#pragma once

#include "vec2.h"
#include "transform2.h"
#include "../memory.h"
#include "../storage.h"
#include "../compilation/gpu.h"

namespace utils::math
	{
	template <typename T = float>
	struct utils_oop_empty_bases rect
		{
		using self_t = rect<T>;
		using value_type = T;
		using nonref_value_type = utils::remove_const_reference_t<value_type>;
		using nonref_self_t = rect<nonref_value_type>;

		using vertex_owner = utils::math::vec2<nonref_value_type>;
		using vertex_observer = utils::math::vecref2<nonref_value_type>;

		struct create : ::utils::oop::non_constructible
			{
			template <std::convertible_to<value_type> T_oth> requires (!utils::concepts::reference<value_type>)
			utils_gpu_available inline static constexpr self_t from_possize(utils::math::vec2<T_oth> position, utils::math::vec2<T_oth> size) { return {position.x(), position.y(), position.x() + size.x(), position.y() + size.y()}; }
			
			template <std::convertible_to<value_type> T_oth>
			utils_gpu_available inline static constexpr  self_t from_ul_dr  (utils::math::vec2<T_oth> ul, utils::math::vec2<T_oth> dr) { return {ul.x(), ul.y(), dr.x(), dr.y()}; }

			template <std::convertible_to<value_type> T_oth>
			utils_gpu_available inline static constexpr self_t from_vertices(utils::math::vec2<T_oth>& a, utils::math::vec2<T_oth>& b) //TODO version with any number of vertices?
				requires (utils::concepts::reference<value_type>)
				{
				nonref_value_type& min_x{std::min(a.x(), b.x())};//vertices.x(), ...)};
				nonref_value_type& min_y{std::min(a.y(), b.y())};//vertices.y(), ...)};
				nonref_value_type& max_x{std::max(a.x(), b.x())};//vertices.x(), ...)};
				nonref_value_type& max_y{std::max(a.y(), b.y())};//vertices.y(), ...)};
				return {min_x, min_y, max_x, max_y};
				}
			template <std::convertible_to<value_type> T_oth>
			utils_gpu_available inline static constexpr self_t from_vertices(const utils::math::vec2<T_oth>& a, const utils::math::vec2<T_oth>& b) //TODO version with any number of vertices?
				requires (!utils::concepts::reference<value_type>)
				{
				nonref_value_type min_x{std::min(a.x(), b.x())};//vertices.x(), ...)};
				nonref_value_type min_y{std::min(a.y(), b.y())};//vertices.y(), ...)};
				nonref_value_type max_x{std::max(a.x(), b.x())};//vertices.x(), ...)};
				nonref_value_type max_y{std::max(a.y(), b.y())};//vertices.y(), ...)};
				return {min_x, min_y, max_x, max_y};
				}
			};

		using storage_t = utils::storage::multiple<storage::get_type<value_type>(), nonref_value_type, 4>;
		storage_t container;

		utils_gpu_available const nonref_value_type& operator[](size_t index) const noexcept { return static_cast<const nonref_value_type&>(container[index]); }
		utils_gpu_available       nonref_value_type& operator[](size_t index)       noexcept { return static_cast<      nonref_value_type&>(container[index]); }
		
		utils_gpu_available const nonref_value_type& ll() const noexcept { return (*this)[0]; }
		utils_gpu_available       nonref_value_type& ll()       noexcept { return (*this)[0]; }
		utils_gpu_available const nonref_value_type& up() const noexcept { return (*this)[1]; }
		utils_gpu_available       nonref_value_type& up()       noexcept { return (*this)[1]; }
		utils_gpu_available const nonref_value_type& rr() const noexcept { return (*this)[2]; }
		utils_gpu_available       nonref_value_type& rr()       noexcept { return (*this)[2]; }
		utils_gpu_available const nonref_value_type& dw() const noexcept { return (*this)[3]; }
		utils_gpu_available       nonref_value_type& dw()       noexcept { return (*this)[3]; }

#pragma region Proxies
	#pragma region Position
		template <bool is_const>
		class proxy_position;
		template <bool is_const>
		class proxy_x
			{
			template <typename T2>
			friend struct rect;
			friend class proxy_position<is_const>;
			using rect_t = std::conditional_t<is_const, const self_t, self_t>;

			public:
				operator nonref_value_type () const noexcept { return r.ll(); }
				operator nonref_value_type&() noexcept requires(!is_const) { return utils::remove_reference_v(r.ll()); }

				proxy_x& operator=(const nonref_value_type& new_value) noexcept
					requires(!is_const)
					{
					nonref_value_type previous_width{r.width()};
					r.ll() = new_value;
					r.width() = previous_width; 
					return *this; 
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				proxy_x& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				proxy_x& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				proxy_x(rect_t& r) : r{r} {}
				rect_t& r;
			};
		template <bool is_const>
		class proxy_y
			{
			template <typename T2>
			friend struct rect;
			friend class proxy_position<is_const>;
			using rect_t = std::conditional_t<is_const, const self_t, self_t>;

			public:
				operator nonref_value_type() const noexcept { return r.up(); }
				operator nonref_value_type& () noexcept requires(!is_const) { return utils::remove_reference_v(r.up()); }

				proxy_y& operator=(const nonref_value_type& new_value) noexcept
					requires(!is_const)
					{
					nonref_value_type previous_height{r.height()};
					r.up() = new_value;
					r.height() = previous_height;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				proxy_y& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				proxy_y& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				proxy_y(rect_t& r) : r{r} {}
				rect_t& r;
			};
		template <bool is_const>
		class proxy_position
			{
			template <typename T2>
			friend struct rect;
			using rect_t = std::conditional_t<is_const, const self_t, self_t>;

			public:
				proxy_x<is_const> x;
				proxy_y<is_const> y;

				operator vertex_owner() const noexcept { return r.ul(); }
				operator vertex_observer() noexcept { return r.ul(); }

				proxy_position& operator=(const vertex_owner& new_value) noexcept
					requires(!is_const)
					{
					x = new_value.x();
					y = new_value.y();
					return *this;
					}

				vertex_owner operator+(const vertex_owner& delta) const noexcept { return static_cast<vertex_owner>(*this) + delta; }
				vertex_owner operator-(const vertex_owner& delta) const noexcept { return static_cast<vertex_owner>(*this) - delta; }

				proxy_position& operator+=(const vertex_owner& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				proxy_position& operator-=(const vertex_owner& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				proxy_position(rect_t& r) : x{r}, y{r}, r{r} {}
				rect_t& r;
			};
	#pragma endregion Position
	#pragma region Size
		template <bool is_const>
		class proxy_size;
		template <bool is_const>
		class proxy_width
			{
			template <typename T2>
			friend struct rect;
			friend class proxy_size<is_const>;
			using rect_t = std::conditional_t<is_const, const self_t, self_t>;

			public:
				operator nonref_value_type() const noexcept { return r.rr() - r.ll(); }

				proxy_width& operator=(const nonref_value_type& new_value) noexcept
					requires(!is_const)
					{
					r.rr() = r.ll() + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				proxy_width& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				proxy_width& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				proxy_width(rect_t& r) : r{r} {}
				rect_t& r;
			};
		template <bool is_const>
		class proxy_height 
			{
			template <typename T2>
			friend struct rect;
			friend class proxy_size<is_const>;
			using rect_t = std::conditional_t<is_const, const self_t, self_t>;

			public:
				operator nonref_value_type() const noexcept { return r.dw() - r.up(); }

				proxy_height& operator=(const nonref_value_type& new_value) noexcept 
					requires(!is_const)
					{
					r.dw() = r.up() + new_value;
					return *this;
					}

				nonref_value_type operator+(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) + delta; }
				nonref_value_type operator-(const nonref_value_type& delta) const noexcept { return static_cast<nonref_value_type>(*this) - delta; }

				proxy_height& operator+=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				proxy_height& operator-=(const nonref_value_type& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				proxy_height(rect_t& r) : r{r} {}
				rect_t& r;
			};
		template <bool is_const>
		class proxy_size
			{
			template <typename T2>
			friend struct rect;
			using rect_t = std::conditional_t<is_const, const self_t, self_t>;

			public:
				proxy_width<is_const> w;
				proxy_height<is_const> h;

				operator vec2<nonref_value_type>() const noexcept { return r.dr() - r.ul(); }

				proxy_size& operator=(const vec2<nonref_value_type>& new_value) noexcept requires(!is_const)
					{
					w = new_value.x();
					h = new_value.y();
					return *this;
					}


				vertex_owner operator+(const vertex_owner& delta) const noexcept { return static_cast<vertex_owner>(*this) + delta; }
				vertex_owner operator-(const vertex_owner& delta) const noexcept { return static_cast<vertex_owner>(*this) - delta; }

				proxy_size& operator+=(const vertex_owner& delta) noexcept requires(!is_const) { return (*this) = operator+(delta); }
				proxy_size& operator-=(const vertex_owner& delta) noexcept requires(!is_const) { return (*this) = operator-(delta); }

			private:
				proxy_size(rect_t& r) : w{r}, h{r}, r{r} {}
				rect_t& r;
			};
	#pragma endregion Size
#pragma endregion Proxies

		bool operator==(const rect& other) const noexcept = default;

#pragma region Accessors
		// Corners
		utils_gpu_available constexpr vertex_owner    ul() const noexcept { return {ll(), up()}; }
		utils_gpu_available constexpr vertex_owner    ur() const noexcept { return {rr(), up()}; }
		utils_gpu_available constexpr vertex_owner    dr() const noexcept { return {rr(), dw()}; }
		utils_gpu_available constexpr vertex_owner    dl() const noexcept { return {ll(), dw()}; }
		utils_gpu_available constexpr vertex_observer ul()       noexcept { return {ll(), up()}; }
		utils_gpu_available constexpr vertex_observer ur()       noexcept { return {rr(), up()}; }
		utils_gpu_available constexpr vertex_observer dr()       noexcept { return {rr(), dw()}; }
		utils_gpu_available constexpr vertex_observer dl()       noexcept { return {ll(), dw()}; }

		// Pos-size
		proxy_x       <true > x () const noexcept { return {*this}; }
		proxy_y       <true > y () const noexcept { return {*this}; }
		proxy_width   <true > w () const noexcept { return {*this}; }
		proxy_height  <true > h () const noexcept { return {*this}; }
		proxy_position<true > p () const noexcept { return {*this}; }
		proxy_size    <true > s () const noexcept { return {*this}; }
		proxy_x       <false> x ()       noexcept { return {*this}; }
		proxy_y       <false> y ()       noexcept { return {*this}; }
		proxy_width   <false> w ()       noexcept { return {*this}; }
		proxy_height  <false> h ()       noexcept { return {*this}; }
		proxy_position<false> p ()       noexcept { return {*this}; }
		proxy_size    <false> s ()       noexcept { return {*this}; }

		// Center
		//     nonref_value_type  get_center_x()        const noexcept { return ll + (w() / nonref_value_type{2}); }
		//     nonref_value_type  get_center_y()        const noexcept { return up + (h() / nonref_value_type{2}); }
		//vec2<nonref_value_type> get_center  ()        const noexcept { return {get_center_x(), get_center_y()}; }
		//void set_center_x(T value) noexcept { auto half_w{get_w() / nonref_value_type{2}}; remove_reference_v(ll) = value - half_w; remove_reference_v(rr) = value + half_w; }
		//void set_center_y(T value){ auto half_h{get_h() / nonref_value_type{2}}; remove_reference_v(up) = value - half_h; remove_reference_v(dw) = value + half_h; }
		//void set_center(vec2<nonref_value_type> value) noexcept { set_center_x(value.x); set_center_y(value.y); }
#pragma endregion Accessors

#pragma region Aliases
		// Corners
		utils_gpu_available constexpr auto  up_left     () const noexcept { return ul(); }
		utils_gpu_available constexpr auto  top_left    () const noexcept { return ul(); }
		utils_gpu_available constexpr auto  up_right    () const noexcept { return ur(); }
		utils_gpu_available constexpr auto  top_right   () const noexcept { return ur(); }
		utils_gpu_available constexpr auto  down_right  () const noexcept { return dr(); }
		utils_gpu_available constexpr auto  bottom_right() const noexcept { return dr(); }
		utils_gpu_available constexpr auto  down_left   () const noexcept { return dl(); }
		utils_gpu_available constexpr auto  bottom_left () const noexcept { return dl(); }
		utils_gpu_available constexpr auto& up_left     ()       noexcept { return ul(); }
		utils_gpu_available constexpr auto& top_left    ()       noexcept { return ul(); }
		utils_gpu_available constexpr auto& up_right    ()       noexcept { return ur(); }
		utils_gpu_available constexpr auto& top_right   ()       noexcept { return ur(); }
		utils_gpu_available constexpr auto& down_right  ()       noexcept { return dr(); }
		utils_gpu_available constexpr auto& bottom_right()       noexcept { return dr(); }
		utils_gpu_available constexpr auto& down_left   ()       noexcept { return dl(); }
		utils_gpu_available constexpr auto& bottom_left ()       noexcept { return dl(); }

		// Pos-size
		auto height  () const noexcept { return h(); }
		auto width   () const noexcept { return w(); }
		auto size    () const noexcept { return s(); }
		auto pos     () const noexcept { return p(); }
		auto position() const noexcept { return p(); }
		auto height  ()       noexcept { return h(); }
		auto width   ()       noexcept { return w(); }
		auto size    ()       noexcept { return s(); }
		auto pos     ()       noexcept { return p(); }
		auto position()       noexcept { return p(); }
#pragma endregion Aliases

		template <std::convertible_to<nonref_value_type> point_value_type>
		bool contains(const vec2<point_value_type>& point) const noexcept { return point.x() >= ll() && point.x() <= rr() && point.y() >= up() && point.y() <= dw(); }

		utils_gpu_available constexpr nonref_self_t scale    (const float                    & scaling    ) const noexcept { nonref_self_t ret{*this}; return ret.scale_self    (scaling    ); }
		utils_gpu_available constexpr nonref_self_t rotate   (const angle::base<float, 360.f>& rotation   ) const noexcept { nonref_self_t ret{*this}; return ret.rotate_self   (rotation   ); }
		utils_gpu_available constexpr nonref_self_t translate(const vec2f                    & translation) const noexcept { nonref_self_t ret{*this}; return ret.translate_self(translation); }
		utils_gpu_available constexpr nonref_self_t transform(const utils::math::transform2  & transform  ) const noexcept { nonref_self_t ret{*this}; return ret.transform_self(transform  ); }

		utils_gpu_available constexpr self_t& scale_self    (const float                    & scaling    ) noexcept requires(!utils::concepts::const_reference<value_type> && std::same_as<nonref_value_type, float>) { return size() *= scaling; }
		utils_gpu_available constexpr self_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept requires(!utils::concepts::const_reference<value_type> && std::same_as<nonref_value_type, float>) { return *this; }
		utils_gpu_available constexpr self_t& translate_self(const vec2f                    & translation) noexcept requires(!utils::concepts::const_reference<value_type> && std::same_as<nonref_value_type, float>) { return pos() += translation; }
		utils_gpu_available constexpr self_t& transform_self(const utils::math::transform2  & transform  ) noexcept requires(!utils::concepts::const_reference<value_type> && std::same_as<nonref_value_type, float>) { return scale_self(transform.scaling).rotate_self(transform.rotation).translate_self(transform.translation); }

		utils_gpu_available constexpr nonref_self_t bounding_box() const noexcept { return *this; }
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
//				<< ucc::type << "left"  << ucc::separ << ": " << ucc::value << aabb.ll() << ucc::separ << ", "
//				<< ucc::type << "up"    << ucc::separ << ": " << ucc::value << aabb.up() << ucc::separ << ", "
//				<< ucc::type << "right" << ucc::separ << ": " << ucc::value << aabb.rr() << ucc::separ << ", "
//				<< ucc::type << "down"  << ucc::separ << ": " << ucc::value << aabb.dw() << ucc::separ
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