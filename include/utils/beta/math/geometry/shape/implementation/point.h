#pragma once

#include "../point.h"
#include "../../../../../math/vec2.h"

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template class math::vec<storage::single<storage::type::owner, float, true>, 2>;
		template class math::vec<storage::single<storage::type::owner, float, true>, 2>;
		template class math::vec<storage::single<storage::type::owner, float, true>, 2>;
		}
	namespace observer
		{
		template class math::vec<storage::single<storage::type::observer, float, true>, 2>;
		template class math::vec<storage::single<storage::type::observer, float, true>, 2>;
		template class math::vec<storage::single<storage::type::observer, float, true>, 2>;
		}
	namespace const_observer
		{
		template class math::vec<storage::single<storage::type::const_observer, float, true>, 2>;
		template class math::vec<storage::single<storage::type::const_observer, float, true>, 2>;
		template class math::vec<storage::single<storage::type::const_observer, float, true>, 2>;
		}
	}