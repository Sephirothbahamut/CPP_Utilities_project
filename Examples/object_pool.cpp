#include <iostream>

//#include <utils/containers/object_pool.h>
//#include <utils/containers/multihandled_default.h>

struct angry_type
	{
	angry_type(int i, float) : i{i} {}
	int i;
	};

void object_pool()
	{
	//utils::containers::object_pool<angry_type> md;
	//auto u{md.make_unique(3, 2.f)};
	//auto s{md.make_shared(3, 2.f)};
	//auto s2{s};
	//auto r{md.emplace<utils::containers::object_pool_handle_version::raw>(3, 2.f)};
	//auto u2{md.emplace<utils::containers::object_pool_handle_version::unique>(3, 2.f)};
	//
	//utils::containers::multihandled_default<angry_type> md{5, 2.f};
	//
	//utils::containers::object_pool
	//	<
	//	angry_type, 
	//	8,
	//	utils::flags<utils::containers::object_pool_handle_version>::full()
	//	> 
	//	op;
	//
	//try
	//	{
	//	md.emplace_in_handle(u, 3, 2.f);
	//	}
	//catch (const std::exception& e)
	//	{
	//	std::cout << e.what() << std::endl;
	//	}
	//
	//utils::containers::object_pool<angry_type>::handle_shared s{md.clone_default<utils::containers::object_pool_handle_version::shared>()};
	//
	//md.emplace_in_handle(s, 55, 32.f);
	//
	//try
	//	{
	//	md.emplace_in_handle(s, 55, 32.f);
	//	}
	//catch (const std::exception& e)
	//	{
	//	std::cout << e.what() << std::endl;
	//	}
	}