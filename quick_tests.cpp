#include <string>
#include <iostream>
#include <execution>

#include "Library/include/utils/containers/regions.h"

int main()
	{
	utils::containers::regions<char> regions;

	regions.add('q', {3, 4});
	regions.add('w', {6, 2}); //shortens q region
	regions.add('r', {10, 4}); //after final null
	regions.add('w', {12, 2}); //shortens r region, shares end as previous r region
	regions.add('r', {9, 1}); //add before region with same value, no overlap and no hole
	regions.add('q', {4, 1}); //fully wrapped

	regions.add('q', {1, 2}); //add before region with same value, that also happens to be the first region, no overlap and no hole
	//regions.add('q', {4, 3}); //add overlapping end of region with same value, ending after, in middle of the next one
	//regions.add('q', {4, 4}); //add overlapping end of region with same value, ending at the end of the next region
	regions.add('q', {4, 5}); //add overlapping end of region with same value, ending after the next region

	const size_t regions_size{regions.size()};
	for (size_t i{0}; i < regions_size; i++)
		{
		const auto& [region, value_opt_ref]{regions[i]};

		if (value_opt_ref)
			{
			std::cout << value_opt_ref.value();
			}
		else
			{
			std::cout << "-";
			}
		}
	std::cout << std::endl;
	return 0;
	}
