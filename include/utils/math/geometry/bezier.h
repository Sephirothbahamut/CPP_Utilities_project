#include <concepts>

#include "../vec.h"

namespace utils::math::geometry
	{
	template <size_t order, utils::math::concepts::vec vec_T>
	class bezier_curve
		{
		public:
			using value_type = vec_T;

		private:
			using self_t = bezier_curve<order, vec_T>;

		public:
			std::vector<vec_t> points;

			template <std::floating_point float_t>
			class proxy
				{
				public:
					value_type point() const noexcept
						{
						//TODO calculate point at t
						}
					value_type normal() const noexcept
						{
						//TODO calculate point at t
						}
					value_type tangent() const noexcept
						{
						//TODO calculate point at t
						}

					operator value_type() const noexcept { return point(); }

				private:
					proxy(const self_t& bezier_curve, const float_t t) : bezier_curve{bezier_curve}, t{t} {}
					const self_t& bezier_curve;
					const float_t t;
				};

			template <std::floating_point float_t>
			const proxy operator[](float_t t) const noexcept
				{
				return {*this, t};
				}
		};
	}