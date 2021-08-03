
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <array>

#include "include/utils/containers/buffer.h"


int main()
	{
	utils::container::buffer buffer;

	int a = 3;
	std::string s{"hello"};

	buffer.push(a);
	buffer.push(s);

	std::cout << buffer.get<int>() << std::endl;
	//std::cout << buffer.get<std::string>() << std::endl;
	}