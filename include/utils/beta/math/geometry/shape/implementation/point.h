#pragma once

#include "../point.h"
#include "../../../../../math/vec2.h"

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct math::vec<storage::single<storage::type::owner, float>, 2>;
		template struct math::vec<storage::single<storage::type::owner, float>, 2>;
		template struct math::vec<storage::single<storage::type::owner, float>, 2>;
		}
	namespace observer
		{
		template struct math::vec<storage::single<storage::type::observer, float>, 2>;
		template struct math::vec<storage::single<storage::type::observer, float>, 2>;
		template struct math::vec<storage::single<storage::type::observer, float>, 2>;
		}
	namespace const_observer
		{
		template struct math::vec<storage::single<storage::type::const_observer, float>, 2>;
		template struct math::vec<storage::single<storage::type::const_observer, float>, 2>;
		template struct math::vec<storage::single<storage::type::const_observer, float>, 2>;
		}
	}