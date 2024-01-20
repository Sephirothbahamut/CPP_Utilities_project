/*namespace utils::beta::math::geometry
	{
	template <size_t ORDER>
	class bezier_curve
		{
		public:
			using value_type = vec_T;
			inline static constexpr size_t order{ORDER};

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

			auto closest_point_to(const value_type& point) const noexcept
				{
				struct ret_t { float t; value_type point; };
				}
		};
	}*/