#include <optional>
#include <iostream>

#include <utils/aggregate.h>
#include <utils/containers/aggregate_regions.h>



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
			std::optional<int> x, y;

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
			utils::containers::regions<int> x, y;
			};

		                           inner_reg         a;
		utils::containers::regions<int             > b;
		utils::containers::regions<aggregate::inner> c;
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
		std::cout << value << ", ";
		}, aggregate, aggregate_opt);
	std::cout << std::endl;


	using aggregate_regions_t = utils::containers::aggregate_regions<example::aggregate, example::aggregate_opt, example::aggregate_reg, example::accessors_helper>;
	aggregate_regions_t aggregate_regions;

	aggregate_regions.regions_per_field.a.x.add(0     , {0,  5});
	aggregate_regions.regions_per_field.a.x.add(1     , {5,  3});
	aggregate_regions.regions_per_field.a.y.add(2     , {3,  4});
	aggregate_regions.regions_per_field.b  .add(5     , {8,  2});
	aggregate_regions.regions_per_field.c  .add({1, 2}, {8,  6});
	aggregate_regions.regions_per_field.c  .add({2, 3}, {8, 14});

	const auto combined{aggregate_regions.combine_regions()};
	for (size_t i{0}; i < combined.size();)
		{
		const auto slot{combined.slot_at(i)};
		i = slot.region.end();

		std::cout << slot.region.begin << " - " << slot.region.end() << ": ";
		if (slot.value_ptr)
			{
			std::cout << (*slot.value_ptr);
			}
		else
			{
			std::cout << "-";
			}
		std::cout << "\n";
		}
	}