#pragma once

#include "vec2.h"
#include "transform2.h"
#include "../memory.h"
#include "../storage.h"
#include "../alignment.h"
#include "../compilation/gpu.h"
#include "../math/geometry/details/base_types.h"

namespace utils::math
	{
	template <typename T>
	concept vec_range = std::ranges::range<T> && utils::math::concepts::vec_size<typename T::value_type, 2> && utils::math::concepts::vec_compatible_type<typename T::value_type, float>;

	template <typename T = float>
	struct utils_oop_empty_bases rect : utils::storage::multiple<T, 4, false>, utils::math::geometry::shape_flag
		{
		using storage_t = utils::storage::multiple<T, 4, false>;

		using self_t = rect<T>;
		using typename storage_t::value_type;
		using typename storage_t::const_aware_value_type;
		using storage_t::storage_type;
		using vertex_owner    = utils::math::vec2<value_type>;
		using vertex_observer = utils::math::vecref2<const_aware_value_type>;
		using nonref_self_t   = rect<std::remove_const_t<value_type>>;

		using storage_t::multiple;

		struct create : ::utils::oop::non_constructible
			{
			template <std::convertible_to<value_type> T_oth> requires (!utils::concepts::reference<value_type>)
			utils_gpu_available inline static constexpr self_t from_possize(utils::math::vec2<T_oth> position, utils::math::vec2<T_oth> size) { return {position.x(), position.y(), position.x() + size.x(), position.y() + size.y()}; }
			
			template <std::convertible_to<value_type> T_oth>
			utils_gpu_available inline static constexpr  self_t from_ul_dr  (utils::math::vec2<T_oth> ul, utils::math::vec2<T_oth> dr) { return {ul.x(), ul.y(), dr.x(), dr.y()}; }

			template <std::same_as<value_type> T_oth>
			utils_gpu_available inline static constexpr self_t from_vertices(utils::math::vec2<T_oth>& a, utils::math::vec2<T_oth>& b)
				requires (storage_type.is_observer())
				{
				return 
					{
					a.x() < b.x() ? a.x() : b.x(),
					a.y() < b.y() ? a.y() : b.y(),
					a.x() > b.x() ? a.x() : b.x(),
					a.y() > b.y() ? a.y() : b.y(),
					};
				}
			template <std::convertible_to<value_type> T_oth>
			utils_gpu_available inline static constexpr self_t from_vertices(const utils::math::vec2<T_oth>& a, const utils::math::vec2<T_oth>& b)
				requires (storage_type.is_owner())
				{
				return 
					{
					a.x() < b.x() ? a.x() : b.x(),
					a.y() < b.y() ? a.y() : b.y(),
					a.x() > b.x() ? a.x() : b.x(),
					a.y() > b.y() ? a.y() : b.y(),
					};
				}

			utils_gpu_available inline static constexpr self_t from_vertices(vec_range auto range)
				requires (storage_type.is_observer())
				{
				const_aware_value_type& first{*range.begin()};
				self_t ret{first.x(), first.y(), first.x(), first.y()};
			
				for (const_aware_value_type& vertex : range)
					{
					if (vertex.x() < ret[0]) { ret.rebind(0, vertex.x()); }
					if (vertex.y() < ret[1]) { ret.rebind(1, vertex.y()); }
					if (vertex.x() > ret[2]) { ret.rebind(2, vertex.x()); }
					if (vertex.y() > ret[3]) { ret.rebind(3, vertex.y()); }
					}
			
				return ret;
				}
			
			utils_gpu_available inline static constexpr self_t from_vertices(vec_range auto range)
				requires (storage_type.is_owner())
				{
				self_t ret{utils::math::constants::finf, utils::math::constants::finf, -utils::math::constants::finf, -utils::math::constants::finf};
			
				for (const auto& vertex : range)
					{
					if (vertex.x() < ret[0]) { ret[0] = vertex.x(); }
					if (vertex.y() < ret[1]) { ret[1] = vertex.y(); }
					if (vertex.x() > ret[2]) { ret[2] = vertex.x(); }
					if (vertex.y() > ret[3]) { ret[3] = vertex.y(); }
					}
			
				return ret;
				}
			};
		
		utils_gpu_available const const_aware_value_type& ll() const noexcept { return (*this)[0]; }
		utils_gpu_available       const_aware_value_type& ll()       noexcept { return (*this)[0]; }
		utils_gpu_available const const_aware_value_type& up() const noexcept { return (*this)[1]; }
		utils_gpu_available       const_aware_value_type& up()       noexcept { return (*this)[1]; }
		utils_gpu_available const const_aware_value_type& rr() const noexcept { return (*this)[2]; }
		utils_gpu_available       const_aware_value_type& rr()       noexcept { return (*this)[2]; }
		utils_gpu_available const const_aware_value_type& dw() const noexcept { return (*this)[3]; }
		utils_gpu_available       const_aware_value_type& dw()       noexcept { return (*this)[3]; }

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
				utils_gpu_available constexpr operator value_type () const noexcept { return r.ll(); }
				utils_gpu_available constexpr operator value_type&() noexcept requires(!is_const) { return utils::remove_reference_v(r.ll()); }

				utils_gpu_available constexpr proxy_x& operator=(const value_type& new_value) noexcept
					requires(!is_const)
					{
					value_type previous_width{r.width()};
					r.ll() = new_value;
					r.width() = previous_width; 
					return *this; 
					}

				utils_gpu_available constexpr value_type operator+(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) + delta ; }
				utils_gpu_available constexpr value_type operator-(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) - delta ; }
				utils_gpu_available constexpr value_type operator*(const value_type& factor) const noexcept { return static_cast<value_type>(*this) * factor; }
				utils_gpu_available constexpr value_type operator/(const value_type& factor) const noexcept { return static_cast<value_type>(*this) / factor; }

				utils_gpu_available constexpr proxy_x& operator+=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator+(delta ); }
				utils_gpu_available constexpr proxy_x& operator-=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator-(delta ); }
				utils_gpu_available constexpr proxy_x& operator*=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator*(factor); }
				utils_gpu_available constexpr proxy_x& operator/=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator/(factor); }

			private:
				utils_gpu_available constexpr proxy_x(rect_t& r) : r{r} {}
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
				utils_gpu_available constexpr operator value_type () const noexcept { return r.up(); }
				utils_gpu_available constexpr operator value_type&() noexcept requires(!is_const) { return utils::remove_reference_v(r.up()); }

				utils_gpu_available constexpr proxy_y& operator=(const value_type& new_value) noexcept
					requires(!is_const)
					{
					value_type previous_height{r.height()};
					r.up() = new_value;
					r.height() = previous_height;
					return *this;
					}
				
				utils_gpu_available constexpr value_type operator+(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) + delta ; }
				utils_gpu_available constexpr value_type operator-(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) - delta ; }
				utils_gpu_available constexpr value_type operator*(const value_type& factor) const noexcept { return static_cast<value_type>(*this) * factor; }
				utils_gpu_available constexpr value_type operator/(const value_type& factor) const noexcept { return static_cast<value_type>(*this) / factor; }

				utils_gpu_available constexpr proxy_y& operator+=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator+(delta ); }
				utils_gpu_available constexpr proxy_y& operator-=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator-(delta ); }
				utils_gpu_available constexpr proxy_y& operator*=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator*(factor); }
				utils_gpu_available constexpr proxy_y& operator/=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator/(factor); }

			private:
				utils_gpu_available constexpr proxy_y(rect_t& r) : r{r} {}
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

				utils_gpu_available constexpr operator vertex_owner() const noexcept { return {r.ul()}; }
				utils_gpu_available constexpr operator vertex_observer() noexcept requires(!is_const) { return r.ul(); }

				utils_gpu_available constexpr proxy_position& operator=(const vertex_owner& new_value) noexcept
					requires(!is_const)
					{
					x = new_value.x();
					y = new_value.y();
					return *this;
					}
				
				utils_gpu_available constexpr vertex_owner operator+(const vertex_owner& delta ) const noexcept { return static_cast<vertex_owner>(*this) + delta ; }
				utils_gpu_available constexpr vertex_owner operator-(const vertex_owner& delta ) const noexcept { return static_cast<vertex_owner>(*this) - delta ; }
				utils_gpu_available constexpr vertex_owner operator*(const vertex_owner& factor) const noexcept { return static_cast<vertex_owner>(*this) * factor; }
				utils_gpu_available constexpr vertex_owner operator/(const vertex_owner& factor) const noexcept { return static_cast<vertex_owner>(*this) / factor; }

				utils_gpu_available constexpr proxy_position& operator+=(const vertex_owner& delta ) noexcept requires(!is_const) { return (*this) = operator+(delta ); }
				utils_gpu_available constexpr proxy_position& operator-=(const vertex_owner& delta ) noexcept requires(!is_const) { return (*this) = operator-(delta ); }
				utils_gpu_available constexpr proxy_position& operator*=(const vertex_owner& factor) noexcept requires(!is_const) { return (*this) = operator*(factor); }
				utils_gpu_available constexpr proxy_position& operator/=(const vertex_owner& factor) noexcept requires(!is_const) { return (*this) = operator/(factor); }

			private:
				utils_gpu_available constexpr proxy_position(rect_t& r) : x{r}, y{r}, r{r} {}
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
				utils_gpu_available constexpr operator value_type() const noexcept { return r.rr() - r.ll(); }

				utils_gpu_available constexpr proxy_width& operator=(const value_type& new_value) noexcept
					requires(!is_const)
					{
					r.rr() = r.ll() + new_value;
					return *this;
					}

				utils_gpu_available constexpr proxy_width& resize(const utils::alignment::horizontal& anchor, const value_type& new_value) noexcept
					requires(!is_const)
					{
					switch (anchor)
						{
						case ::utils::alignment::horizontal::left:
							{
							r.rr() = r.ll() + new_value;
							}
							break;
						case ::utils::alignment::horizontal::centre:
							{
							const auto centre{r.centre_x()};
							const auto half_value{new_value / value_type{2}};
							r.ll() = centre - half_value;
							r.rr() = centre + half_value;
							}
							break;
						case ::utils::alignment::horizontal::right:
							{
							r.ll() = r.rr() - new_value;
							}
							break;
						}
					return *this;
					}

				utils_gpu_available constexpr proxy_width& scale(const utils::alignment::horizontal& anchor, const value_type& factor) noexcept
					requires(!is_const)
					{
					return resize(anchor, static_cast<value_type>(*this) * factor);
					}

				utils_gpu_available constexpr value_type operator+(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) + delta ; }
				utils_gpu_available constexpr value_type operator-(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) - delta ; }
				utils_gpu_available constexpr value_type operator*(const value_type& factor) const noexcept { return static_cast<value_type>(*this) * factor; }
				utils_gpu_available constexpr value_type operator/(const value_type& factor) const noexcept { return static_cast<value_type>(*this) / factor; }

				utils_gpu_available constexpr proxy_width& operator+=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator+(delta ); }
				utils_gpu_available constexpr proxy_width& operator-=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator-(delta ); }
				utils_gpu_available constexpr proxy_width& operator*=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator*(factor); }
				utils_gpu_available constexpr proxy_width& operator/=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator/(factor); }

			private:
				utils_gpu_available constexpr proxy_width(rect_t& r) : r{r} {}
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
				utils_gpu_available constexpr operator value_type() const noexcept { return r.dw() - r.up(); }

				utils_gpu_available constexpr proxy_height& operator=(const value_type& new_value) noexcept
					requires(!is_const)
					{
					r.dw() = r.up() + new_value;
					return *this;
					}

				utils_gpu_available constexpr proxy_height& resize(const utils::alignment::vertical& anchor, const value_type& new_value) noexcept
					requires(!is_const)
					{
					switch (anchor)
						{
						case ::utils::alignment::vertical::top:
							{
							r.dw() = r.up() + new_value;
							}
							break;
						case ::utils::alignment::vertical::middle:
							{
							const auto centre{r.centre_y()};
							const auto half_value{new_value / value_type{2}};
							r.up() = centre - half_value;
							r.dw() = centre + half_value;
							}
							break;
						case ::utils::alignment::vertical::bottom:
							{
							r.up() = r.dw() - new_value;
							}
							break;
						}
					return *this;
					}

				utils_gpu_available constexpr proxy_height& scale(const utils::alignment::vertical& anchor, const value_type& factor) noexcept
					requires(!is_const)
					{
					return resize(anchor, static_cast<value_type>(*this) * factor);
					}

				utils_gpu_available constexpr value_type operator+(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) + delta ; }
				utils_gpu_available constexpr value_type operator-(const value_type& delta ) const noexcept { return static_cast<value_type>(*this) - delta ; }
				utils_gpu_available constexpr value_type operator*(const value_type& factor) const noexcept { return static_cast<value_type>(*this) * factor; }
				utils_gpu_available constexpr value_type operator/(const value_type& factor) const noexcept { return static_cast<value_type>(*this) / factor; }
				
				utils_gpu_available constexpr proxy_height& operator+=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator+(delta ); }
				utils_gpu_available constexpr proxy_height& operator-=(const value_type& delta ) noexcept requires(!is_const) { return (*this) = operator-(delta ); }
				utils_gpu_available constexpr proxy_height& operator*=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator*(factor); }
				utils_gpu_available constexpr proxy_height& operator/=(const value_type& factor) noexcept requires(!is_const) { return (*this) = operator/(factor); }

			private:
				utils_gpu_available constexpr proxy_height(rect_t& r) : r{r} {}
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

				utils_gpu_available constexpr operator vertex_owner() const noexcept { return r.dr() - r.ul(); }

				utils_gpu_available constexpr proxy_size& operator=(const vertex_owner& new_value) noexcept requires(!is_const)
					{
					w = new_value.x();
					h = new_value.y();
					return *this;
					}

				utils_gpu_available constexpr proxy_size& resize(const utils::alignment& anchor, const vertex_owner& new_value) noexcept
					requires(!is_const)
					{
					w.resize(anchor.horizontal_alignment, new_value.x());
					h.resize(anchor.vertical_alignment  , new_value.y());
					return *this;
					}

				utils_gpu_available constexpr proxy_size& scale(const utils::alignment& anchor, const vertex_owner& factor) noexcept
					requires(!is_const)
					{
					return resize(anchor, static_cast<vertex_owner>(*this) * factor);
					}

				utils_gpu_available constexpr vertex_owner operator+(const vertex_owner& delta ) const noexcept { return static_cast<vertex_owner>(*this) + delta ; }
				utils_gpu_available constexpr vertex_owner operator-(const vertex_owner& delta ) const noexcept { return static_cast<vertex_owner>(*this) - delta ; }
				utils_gpu_available constexpr vertex_owner operator*(const vertex_owner& factor) const noexcept { return static_cast<vertex_owner>(*this) * factor; }
				utils_gpu_available constexpr vertex_owner operator/(const vertex_owner& factor) const noexcept { return static_cast<vertex_owner>(*this) / factor; }
				
				utils_gpu_available constexpr proxy_size& operator+=(const vertex_owner& delta ) noexcept requires(!is_const) { return (*this) = operator+(delta ); }
				utils_gpu_available constexpr proxy_size& operator-=(const vertex_owner& delta ) noexcept requires(!is_const) { return (*this) = operator-(delta ); }
				utils_gpu_available constexpr proxy_size& operator*=(const vertex_owner& factor) noexcept requires(!is_const) { return (*this) = operator*(factor); }
				utils_gpu_available constexpr proxy_size& operator/=(const vertex_owner& factor) noexcept requires(!is_const) { return (*this) = operator/(factor); }

			private:
				proxy_size(rect_t& r) : w{r}, h{r}, r{r} {}
				rect_t& r;
			};
	#pragma endregion Size
#pragma endregion Proxies

		utils_gpu_available constexpr bool operator==(const rect& other) const noexcept = default;

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
		utils_gpu_available constexpr proxy_x       <true                              > x () const noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_y       <true                              > y () const noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_width   <true                              > w () const noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_height  <true                              > h () const noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_position<true                              > p () const noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_size    <true                              > s () const noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_x       <storage_t::storage_type.is_const()> x () noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_y       <storage_t::storage_type.is_const()> y () noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_width   <storage_t::storage_type.is_const()> w () noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_height  <storage_t::storage_type.is_const()> h () noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_position<storage_t::storage_type.is_const()> p () noexcept { return {*this}; }
		utils_gpu_available constexpr proxy_size    <storage_t::storage_type.is_const()> s () noexcept { return {*this}; }

		// Center
		value_type   centre_x()        const noexcept { return ll() + (w() / value_type{2}); }
		value_type   centre_y()        const noexcept { return up() + (h() / value_type{2}); }
		vertex_owner centre  ()        const noexcept { return {centre_x(), centre_y()}; }
		//void set_center_x(T value) noexcept { auto half_w{get_w() / value_type{2}}; remove_reference_v(ll) = value - half_w; remove_reference_v(rr) = value + half_w; }
		//void set_center_y(T value){ auto half_h{get_h() / value_type{2}}; remove_reference_v(up) = value - half_h; remove_reference_v(dw()) = value + half_h; }
		//void set_center(vec2<value_type> value) noexcept { set_center_x(value.x); set_center_y(value.y); }
#pragma endregion Accessors

#pragma region Aliases
		// Corners
		utils_gpu_available constexpr auto up_left     () const noexcept { return ul(); }
		utils_gpu_available constexpr auto top_left    () const noexcept { return ul(); }
		utils_gpu_available constexpr auto up_right    () const noexcept { return ur(); }
		utils_gpu_available constexpr auto top_right   () const noexcept { return ur(); }
		utils_gpu_available constexpr auto down_right  () const noexcept { return dr(); }
		utils_gpu_available constexpr auto bottom_right() const noexcept { return dr(); }
		utils_gpu_available constexpr auto down_left   () const noexcept { return dl(); }
		utils_gpu_available constexpr auto bottom_left () const noexcept { return dl(); }
		utils_gpu_available constexpr auto up_left     ()       noexcept { return ul(); }
		utils_gpu_available constexpr auto top_left    ()       noexcept { return ul(); }
		utils_gpu_available constexpr auto up_right    ()       noexcept { return ur(); }
		utils_gpu_available constexpr auto top_right   ()       noexcept { return ur(); }
		utils_gpu_available constexpr auto down_right  ()       noexcept { return dr(); }
		utils_gpu_available constexpr auto bottom_right()       noexcept { return dr(); }
		utils_gpu_available constexpr auto down_left   ()       noexcept { return dl(); }
		utils_gpu_available constexpr auto bottom_left ()       noexcept { return dl(); }

		// Pos-size
		utils_gpu_available constexpr auto height  () const noexcept { return h(); }
		utils_gpu_available constexpr auto width   () const noexcept { return w(); }
		utils_gpu_available constexpr auto size    () const noexcept { return s(); }
		utils_gpu_available constexpr auto pos     () const noexcept { return p(); }
		utils_gpu_available constexpr auto position() const noexcept { return p(); }
		utils_gpu_available constexpr auto height  ()       noexcept { return h(); }
		utils_gpu_available constexpr auto width   ()       noexcept { return w(); }
		utils_gpu_available constexpr auto size    ()       noexcept { return s(); }
		utils_gpu_available constexpr auto pos     ()       noexcept { return p(); }
		utils_gpu_available constexpr auto position()       noexcept { return p(); }
#pragma endregion Aliases

		utils_gpu_available constexpr bool contains(const concepts::vec_size<2> auto& point) const noexcept { return point.x() >= ll() && point.x() <= rr() && point.y() >= up() && point.y() <= dw(); }
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