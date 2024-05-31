#pragma once

#include "../../../../math/vec2.h"
#include "../../../../math/rect.h"
#include "../../../../math/constants.h"
#include "../../../../math/transform2.h"

namespace utils::math::geometry
	{
	enum class shapes_enum
		{
		point,
		circle,
		aabb,
		capsule,

		ab,
		line, //ab specialization
		ray, //ab specialization
		segment, //ab specialization

		polyline,
		polygon, //polyline specialization

		bezier,
		spline,

		mixed
		};
	
	struct ends_t
		{
		struct create
			{
			create() = delete;

			utils_gpu_available static consteval ends_t open    (bool finite_a = false, bool finite_b = false) noexcept { return ends_t{.finite_a{finite_a}, .finite_b{finite_b}, .open{true }}; }
			utils_gpu_available static consteval ends_t infinite(                                            ) noexcept { return ends_t{.finite_a{false   }, .finite_b{false   }, .open{true }}; }
			utils_gpu_available static consteval ends_t closed  (                                            ) noexcept { return ends_t{.finite_a{true    }, .finite_b{true    }, .open{false}}; }
			};

		bool finite_a;
		bool finite_b;
		bool open;

		utils_gpu_available inline constexpr bool operator==(const ends_t& other) const noexcept = default;

		utils_gpu_available inline constexpr bool is_open      () const noexcept { return open; }
		utils_gpu_available inline constexpr bool is_closed    () const noexcept { return !is_open(); }
		utils_gpu_available inline constexpr bool is_a_infinite() const noexcept { return is_open() && !finite_a; }
		utils_gpu_available inline constexpr bool is_b_infinite() const noexcept { return is_open() && !finite_b; }
		utils_gpu_available inline constexpr bool is_a_finite  () const noexcept { return is_open() &&  finite_a; }
		utils_gpu_available inline constexpr bool is_b_finite  () const noexcept { return is_open() &&  finite_b; }
		utils_gpu_available inline constexpr bool is_finite    () const noexcept { return is_closed() || (finite_a && finite_b); }
		utils_gpu_available inline constexpr bool is_infinite  () const noexcept { return !is_finite(); }
		};

	class side
		{
		public:
			constexpr side() = default; // No need to specify utils_gpu_available for defaults
			utils_gpu_available constexpr side(float value) : _value{value < -math::constants::epsilonf ? -1.f : value > math::constants::epsilonf ? 1.f : 0.f }  {}

			struct create
				{
				create() = delete;
				
				utils_gpu_available static consteval side left      () noexcept { return {-1.f}; }
				utils_gpu_available static consteval side right     () noexcept { return { 1.f}; }
				utils_gpu_available static consteval side coincident() noexcept { return { 0.f}; }

				utils_gpu_available static consteval side inside    () noexcept { return left (); }
				utils_gpu_available static consteval side outside   () noexcept { return right(); }
				};
			
			utils_gpu_available constexpr bool is_left      (float epsilon = 0.f) const noexcept { return _value <  -epsilon; }
			utils_gpu_available constexpr bool is_coincident(float epsilon = 0.f) const noexcept { return _value >= -epsilon && _value <= epsilon; }
			utils_gpu_available constexpr bool is_right     (float epsilon = 0.f) const noexcept { return _value >   epsilon; }

			utils_gpu_available constexpr bool is_outside   (float epsilon = 0.f) const noexcept { return  is_right  (epsilon); }
			utils_gpu_available constexpr bool is_inside    (float epsilon = 0.f) const noexcept { return !is_outside(epsilon); }

			utils_gpu_available constexpr bool is_exactly_left      () const noexcept { return is_left      (0.f); }
			utils_gpu_available constexpr bool is_exactly_coincident() const noexcept { return is_coincident(0.f); }
			utils_gpu_available constexpr bool is_exactly_right     () const noexcept { return is_right     (0.f); }
			utils_gpu_available constexpr bool is_approx_left       () const noexcept { return is_left      (utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_coincident () const noexcept { return is_coincident(utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_right      () const noexcept { return is_right     (utils::math::constants::epsilonf); }
			
			utils_gpu_available constexpr bool is_exactly_inside () const noexcept { return is_inside (0.f); }
			utils_gpu_available constexpr bool is_exactly_outside() const noexcept { return is_outside(0.f); }
			utils_gpu_available constexpr bool is_approx_inside  () const noexcept { return is_inside (utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_outside () const noexcept { return is_outside(utils::math::constants::epsilonf); }


			utils_gpu_available constexpr float value    () const noexcept { return   _value ; }
			utils_gpu_available constexpr side  operator-() const noexcept { return {-_value}; }

		private:
			float _value{0.f};
		};
	utils_gpu_available constexpr float  operator* (const float& f, const side& side) noexcept { return f * side.value(); }
	utils_gpu_available constexpr float& operator*=(      float& f, const side& side) noexcept { return f = f * side.value(); }
	utils_gpu_available constexpr bool   operator==(      float& f, const side& side) noexcept { return (math::sign(f) == math::sign(side.value())) || (f == 0.f && side.value() == 0.f); }
	
	struct signed_distance_t
		{
		float value{std::numeric_limits<float>::infinity()};
		utils_gpu_available constexpr float             absolute () const noexcept { return math::abs( value); }
		utils_gpu_available constexpr side              side     () const noexcept { return          { value}; }
		utils_gpu_available constexpr signed_distance_t operator-() const noexcept { return          {-value}; }
		utils_gpu_available constexpr auto operator<=>(const signed_distance_t& other) const noexcept { return value <=> other.value; }
		utils_gpu_available constexpr auto operator== (const signed_distance_t& other) const noexcept { return value ==  other.value; }
		};

	template <typename T>
	struct closest_with_distance_t  
		{ 
		T closest{};
		signed_distance_t distance{utils::math::constants::finf};

		utils_gpu_available static constexpr closest_with_distance_t<T> pick_closest(const closest_with_distance_t<T>& a, const closest_with_distance_t<T>& b) noexcept
			{
			return a.distance.absolute() < b.distance.absolute() ? a : b;
			}
		utils_gpu_available static constexpr closest_with_distance_t<T> pick_closest_raw(const closest_with_distance_t<T>& a, const closest_with_distance_t<T>& b) noexcept
			{
			return a.distance.value < b.distance.value ? a : b;
			}
		};
	
	using closest_point_with_distance_t = closest_with_distance_t<vec2f>;

	struct closest_pair_with_distance_t 
		{
		vec2f closest_in_source; 
		vec2f closest_in_target; 
		signed_distance_t distance;
		utils_gpu_available constexpr closest_pair_with_distance_t operator-() const noexcept { return {closest_in_target, closest_in_source, -distance}; }
		};

	namespace shape
		{
		namespace details
			{
			template <typename derived_t, shapes_enum shape_id> struct base;
			}

		namespace concepts
			{
			template <typename T> concept any = std::derived_from<std::remove_cvref_t<T>, details::base<std::remove_cvref_t<T>, std::remove_cvref_t<T>::static_shape_id>>;

			template <typename T> concept point    = any<T> && (T::static_shape_id == shapes_enum::point   );
			template <typename T> concept circle   = any<T> && (T::static_shape_id == shapes_enum::circle  );
			template <typename T> concept aabb     = any<T> && (T::static_shape_id == shapes_enum::aabb    );
			template <typename T> concept capsule  = any<T> && (T::static_shape_id == shapes_enum::capsule );

			template <typename T> concept ab       = any<T> && (T::static_shape_id == shapes_enum::ab      );

			template <typename T> concept line     = ab<T> && (T::static_ends == ends_t::create::open(false, false));
			template <typename T> concept ray      = ab<T> && (T::static_ends == ends_t::create::open(true , false));
			template <typename T> concept segment  = ab<T> && (T::static_ends == ends_t::create::open(true , true ));

			template <typename T> concept polyline      = any<T> && (T::static_shape_id == shapes_enum::polyline);
			template <typename T> concept polyline_open = polyline<T> && (T::static_ends.is_open());
			template <typename T> concept polygon       = polyline<T> && (T::static_ends.is_closed());

			template <typename T> concept bezier   = any<T> && (T::static_shape_id == shapes_enum::bezier  );
			template <typename T> concept spline   = any<T> && (T::static_shape_id == shapes_enum::spline  );
			template <typename T> concept mixed    = any<T> && (T::static_shape_id == shapes_enum::mixed   );
			}

		struct aabb;
		struct point;

		namespace details
			{
			template <typename derived_T, shapes_enum shape_id>
			struct base
				{
				protected:
					using derived_t = derived_T;
					utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
					utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

				public:

					inline static constexpr shapes_enum static_shape_id{shape_id};

					//Interactions are defined in common, so interactions.h can be included after base_types.h and all shapes definitions
					utils_gpu_available constexpr side                          side_of              (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr float                         distance             (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr geometry::signed_distance_t   distance_signed      (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr vec<float, 2>                 closest_point_of     (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr vec<float, 2>                 closest_point_to     (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr closest_point_with_distance_t closest_with_distance(const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr closest_pair_with_distance_t  closest_pair         (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr vec<float, 2>                 vector_to            (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr bool                          intersects           (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr std::optional<vec<float, 2>>  intersection_with    (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr bool                          contains             (const concepts::any auto& other) const noexcept;
					utils_gpu_available constexpr bool                          collides_with        (const concepts::any auto& other) const noexcept;
		
					utils_gpu_available constexpr derived_t  scale         (const float                    & scaling    ) const noexcept { auto ret{derived()}; return ret.scale_self    (scaling    ); }
					utils_gpu_available constexpr derived_t  rotate        (const angle::base<float, 360.f>& rotation   ) const noexcept { auto ret{derived()}; return ret.rotate_self   (rotation   ); }
					utils_gpu_available constexpr derived_t  translate     (const vec<float, 2>            & translation) const noexcept { auto ret{derived()}; return ret.translate_self(translation); }
					utils_gpu_available constexpr derived_t  transform     (const transform2               & transform  ) const noexcept { auto ret{derived()}; return ret.transform_self(transform  ); }

					utils_gpu_available constexpr derived_t& scale_self    (const float      & scaling    ) noexcept;
					utils_gpu_available constexpr derived_t& rotate_self   (const angle::degf& rotation   ) noexcept;
					utils_gpu_available constexpr derived_t& translate_self(const vec2f      & translation) noexcept;
					utils_gpu_available constexpr derived_t& transform_self(const transform2 & transform  ) noexcept;

					utils_gpu_available constexpr aabb bounding_box() const noexcept;
				};
			}
		}

	}