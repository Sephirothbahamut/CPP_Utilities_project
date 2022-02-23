
#include "../vec2.h"
#include "../angle.h"
#include "segment.h"

namespace utils::math::geometry
	{
	class point;
	class segment;


	class shape
		{
		public:

			virtual bool  contains       (const vec2f&   point)   const noexcept = 0;
			virtual bool  contains       (const segment& segment) const noexcept = 0;
			virtual bool  contains       (const shape&   shape)   const noexcept = 0;
			virtual bool  contains_strict(const vec2f&   point)   const noexcept = 0;
			virtual bool  contains_strict(const segment& segment) const noexcept = 0;
			virtual bool  contains_strict(const shape&   shape)   const noexcept = 0;
			
			virtual bool  intersects     (const segment& segment) const noexcept = 0;
			virtual bool  intersects     (const shape&   shape)   const noexcept = 0;
			virtual vec2f intersection   (const segment& segment) const noexcept = 0;
			virtual vec2f intersection   (const shape&   shape)   const noexcept = 0;

			virtual shape& rotate  (const angle::deg& angle) noexcept = 0;
			virtual shape  rotation(const angle::deg& angle) const noexcept = 0;
			virtual shape& translate(const vec2f& distance) noexcept = 0;
			virtual shape& translation(const vec2f& distance) const noexcept = 0;
		};
	}