#include <iostream>

#include <utils/name.h>

void name()
	{
	utils::name hello{"hello"};

	std::unordered_map<utils::name, int> map;

	map[hello] = 5;
	map["hello"] = 1;
	map["world"] = 2;

	std::cout << map[hello];
	std::cout << map["world"];
	std::cout << ((hello == "halo") ? "true" : "false");
	std::cout << std::endl;
	}