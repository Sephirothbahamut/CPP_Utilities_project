#pragma once

#include "../aabb.h"
#include "../../../../../math/rect.h"

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct math::rect<storage::single<storage::type::owner, float>>;
		template struct math::rect<storage::single<storage::type::owner, float>>;
		template struct math::rect<storage::single<storage::type::owner, float>>;
		}
	namespace observer
		{
		template struct math::rect<storage::single<storage::type::observer, float>>;
		template struct math::rect<storage::single<storage::type::observer, float>>;
		template struct math::rect<storage::single<storage::type::observer, float>>;
		}
	namespace const_observer
		{
		template struct math::rect<storage::single<storage::type::const_observer, float>>;
		template struct math::rect<storage::single<storage::type::const_observer, float>>;
		template struct math::rect<storage::single<storage::type::const_observer, float>>;
		}
	}