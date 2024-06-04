#pragma once

#include <concepts>
#include "../../../../compilation/gpu.h"

namespace utils::math
	{
	template <typename T, size_t size>
	class vec;
	template <typename T>
	struct rect;

	namespace angle
		{
		template <std::floating_point T, T full_angle_value>
		class base;
		}

	using vec2f = vec<float, 2>;

	struct transform2;
	};

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <typename derived_T>
		struct root
			{
			protected:
				using derived_t = derived_T;
				utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

			public:
				utils_gpu_available constexpr derived_t  scale         (const float                    & scaling    ) const noexcept { auto ret{derived()}; return ret.scale_self    (scaling    ); }
				utils_gpu_available constexpr derived_t  rotate        (const angle::base<float, 360.f>& rotation   ) const noexcept { auto ret{derived()}; return ret.rotate_self   (rotation   ); }
				utils_gpu_available constexpr derived_t  translate     (const vec<float, 2>            & translation) const noexcept { auto ret{derived()}; return ret.translate_self(translation); }
				utils_gpu_available constexpr derived_t  transform     (const transform2               & transform  ) const noexcept { auto ret{derived()}; return ret.transform_self(transform  ); }

				utils_gpu_available constexpr derived_t& scale_self    (const float                    & scaling    ) noexcept = delete;
				utils_gpu_available constexpr derived_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept = delete;
				utils_gpu_available constexpr derived_t& translate_self(const vec2f                    & translation) noexcept = delete;
				utils_gpu_available constexpr derived_t& transform_self(const transform2               & transform  ) noexcept = delete;

				utils_gpu_available constexpr math::rect<float> bounding_box() const noexcept = delete;
			};
		}

	namespace concepts
		{
		template <typename T>
		concept any = std::derived_from<std::remove_cvref_t<T>, shape::generic::root<typename T::derived_t>>;
		}
	}