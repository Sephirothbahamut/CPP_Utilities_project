#include <iostream>

#include <utils/flags.h>

enum class example_enum
	{
	a = 0b00000001,
	b = 0b00000010,
	c = 0b00000100,
	d = 0b00001000
	};

void f(const utils::flags<example_enum>& flags)
	{
	std::cout << flags.test(example_enum::a) << std::endl;
	}

void flags()
	{

	utils::flags<example_enum> flags{example_enum::a, example_enum::b};
	flags |=    example_enum::d ;
	flags.flip (example_enum::a);
	flags.reset(example_enum::b);
	flags = example_enum::a | flags;
	
	std::cout << flags.test(example_enum::a) << std::endl;
	std::cout << flags.test(example_enum::b) << std::endl;
	std::cout << flags.test(example_enum::c) << std::endl;
	std::cout << flags.test(example_enum::d) << std::endl;
	
	f(flags.flip(example_enum::a));
	}