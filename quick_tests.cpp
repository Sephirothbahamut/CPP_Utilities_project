
#include <tuple>
#include <string>
#include <iostream>
#include <execution>

#include "Library/include/utils/variadic.h"

#include "Library/include/utils/containers/regions.h"


struct test_aggregate
	{
	char c;
	int i;
	float f;
	};

template <typename aggregate_t, utils::containers::concepts::regions ...regions_containers_ts>
utils::containers::regions<aggregate_t> make_aggregate_regions(regions_containers_ts&& ...regions_containers)
	{
	std::tuple<const regions_containers_ts&...> containers_tuple{std::forward<regions_containers_ts>(regions_containers)...};
	std::array<std::size_t, sizeof...(regions_containers_ts)> per_container_slots_size{regions_containers.slots_size()...};
	std::array<std::size_t, sizeof...(regions_containers_ts)> per_container_slot_index{0};

	std::tuple<typename regions_containers_ts::read_slot...> per_container_current_slot;

	size_t element_index{0};

	while (per_container_slot_index != per_container_slots_size)
		{
		//When at least one of the containers changes slot we make a new region

		utils::tuple::for_each_index(containers_tuple, [&](const auto& container, size_t container_index)
			{
			const size_t slots_size{per_container_slots_size[container_index]};
			const size_t slot_index{per_container_slot_index[container_index]};
			if (slot_index == slots_size) { return; }


			});

		element_index++;
		}
	}

int main()
	{


	std::tuple<int, float, double> tp{1, 2.3f, 4.5};
	utils::tuple::for_each_index(tp, [](const auto& element, size_t index) { std::cout << index << ": " << element << "\n"; });

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

	const size_t regions_size{regions.values_size()};
	for (size_t i{0}; i < regions_size; i++)
		{
		const auto& [region, value_opt_ref]{regions.value_at(i)};

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
