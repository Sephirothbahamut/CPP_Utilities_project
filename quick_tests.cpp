#include <vector>
#include <optional>
#include <iostream>

#include "Library/include/utils/math/math.h"
#include "Library/include/utils/aggregate.h"
#include "Library/include/utils/containers/aggregate_regions.h"


void print_state(const utils::containers::regions<float>& regions) noexcept
	{
	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}
	std::cout << "\n________________________________\n" << std::endl;
	}


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& v)
	{
	if (v) { return os << (*v); }
	return os << "null";
	}

namespace example
	{
	struct aggregate
		{
		struct inner
			{
			int x, y;
			inline friend std::ostream& operator<<(std::ostream& os, const inner& v)
				{
				return os << "{x: " << v.x << ", y: " << v.y << "}";
				}

			bool operator==(const inner&) const noexcept = default;
			};

		inner a;
		int   b;
		inner c;

		bool operator==(const aggregate&) const noexcept = default;
		};

	struct aggregate_opt
		{
		struct inner_opt
			{
			std::optional<int> x;
			std::optional<int> y;

			friend std::ostream& operator<<(std::ostream& os, const inner_opt& v)
				{
				return os << "{x: " << v.x << ", y: " << v.y << "}";
				}

			bool operator==(const inner_opt&) const noexcept = default;
			};

		              inner_opt         a;
		std::optional<int             > b;
		std::optional<aggregate::inner> c;

		friend std::ostream& operator<<(std::ostream& os, const aggregate_opt& v)
			{
			return os << "{ a: " << v.a << ", b: " << v.b << ", c: " << v.c << "}";
			}
		bool operator==(const aggregate_opt&) const noexcept = default;
		};

	struct aggregate_reg
		{
		struct inner_reg
			{
			utils::containers::regions<std::optional<int>> x{std::nullopt};
			utils::containers::regions<std::optional<int>> y{std::nullopt};
			};

		inner_reg         a;
		utils::containers::regions<std::optional<int             >> b{std::nullopt};
		utils::containers::regions<std::optional<aggregate::inner>> c{std::nullopt};
		};

	using accessors_helper = utils::aggregate::accessors_helper
		<
		utils::aggregate::accessors_recursive_helper
			<
			[](auto& owner) noexcept -> auto& { return owner.a; },
			utils::aggregate::accessors_helper
				<
				[](auto& instance) noexcept -> auto& { return instance.x; },
				[](auto& instance) noexcept -> auto& { return instance.y; }
				>
			> {},
		[](auto& instance) noexcept -> auto& { return instance.b; },
		[](auto& instance) noexcept -> auto& { return instance.c; }
		>;
	}

void aggregate_and_regions()
	{
	example::aggregate aggregate
		{
		.a{.x{0}, .y{1}},
		.b{2},
		.c{.x{3}, .y{4}},
		};
	const example::aggregate_opt aggregate_opt
		{
		.a{.x{10}, .y{std::nullopt}},
		.b{std::nullopt},
		.c{example::aggregate::inner{.x{13}, .y{14}}},
		};
	utils::aggregate::apply<example::accessors_helper>([](auto& value, auto& value_opt)
		{
		value = value_opt.value_or(value);
		std::cout << value << ", \n";
		}, aggregate, aggregate_opt);
	std::cout << std::endl;


	using aggregate_regions_t = utils::containers::aggregate_regions<example::aggregate_opt, example::aggregate_reg, example::accessors_helper>;
	aggregate_regions_t aggregate_regions{example::aggregate_reg{}};

	aggregate_regions.regions_per_field.a.x.add(0     , {0,  5});
	aggregate_regions.regions_per_field.a.x.add(1     , {5,  3});
	aggregate_regions.regions_per_field.a.y.add(2     , {3,  4});
	aggregate_regions.regions_per_field.b  .add(5     , {8,  2});
	aggregate_regions.regions_per_field.c  .add(example::aggregate::inner{1, 2}, {8,  6});
	aggregate_regions.regions_per_field.c  .add(example::aggregate::inner{2, 3}, {8, 14});

	const auto combined{aggregate_regions.combine_regions()};
	for (const auto slot : combined)
		{
		std::cout << slot.region.begin << " - " << slot.region.end() << ": " << slot.value << "\n";
		}
	}

void regions()
	{
	utils::containers::regions<int> regions{0};
	regions.add(1, { 3, 20});
	regions.add(2, { 5,  3});
	regions.add(2, { 8,  2});
	regions.add(2, { 9,  2});
	regions.add(1, { 2,  1});
	regions.add(3, {30,  3});
	regions.add(4, {35,  1});
	regions.add(5, {40,  3});
	regions.add(6, {50,  3});
	regions.add(7, {60,  3});
	}

int main()
	{
	aggregate_and_regions();
	regions();
	}