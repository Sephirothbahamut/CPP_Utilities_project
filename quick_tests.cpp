#include <vector>
#include <optional>
#include <iostream>

#include "Library/include/utils/aggregate.h"
#include "Library/include/utils/containers/aggregate_regions.h"


template <typename T> utils::observer_ptr<const T> observer_for_optional(const std::optional<      T>& opt) { return opt.has_value() ? std::addressof(*opt) : nullptr; }
template <typename T> utils::observer_ptr<const T> observer_for_optional(      std::optional<const T>& opt) { return opt.has_value() ? std::addressof(*opt) : nullptr; }
template <typename T> utils::observer_ptr<      T> observer_for_optional(      std::optional<      T>& opt) { return opt.has_value() ? std::addressof(*opt) : nullptr; }

struct region
	{
	size_t begin{0};
	size_t count{1};
	utils_gpu_available inline constexpr size_t end() const noexcept { return begin + count; }

	struct create : utils::oop::non_constructible
		{
		inline static constexpr region full_range() noexcept { return region{0, std::numeric_limits<size_t>::max()}; }
		inline static constexpr region from_to(size_t start, size_t end) noexcept
			{
			assert(end > start);
			return region{start, end - start};
			}
		inline static constexpr region from(size_t start) noexcept { return from_to(start, std::numeric_limits<size_t>::max()); }
		};

	bool operator==(const region& other) const noexcept = default;
	};






//template <typename T>
class regions
	{
	public:
		using value_type = int;// T;

		struct read_slot
			{
			using value_type = regions::value_type;
			const region region;
			utils::observer_ptr<const value_type> value_ptr;
			};

	private:
		struct inner_slot_t
			{
			std::optional<value_type> value_opt{std::nullopt};
			size_t end{std::numeric_limits<size_t>::max()};
			};

		std::vector<inner_slot_t> inner_slots;

		size_t inner_slots_index_for_value_at(const size_t& index) const noexcept
			{
			for (size_t index_in_slots{0}; index_in_slots < inner_slots.size(); index_in_slots++)
				{
				const inner_slot_t& inner_slot{inner_slots[index_in_slots]};
				if (inner_slot.end > index) 
					{
					return index_in_slots;
					}
				}
			std::unreachable();
			}
		const inner_slot_t             & inner_slot_for_value_at(const size_t& index) const noexcept { return inner_slots[inner_slots_index_for_value_at(index)]          ; }
		/***/ inner_slot_t             & inner_slot_for_value_at(const size_t& index) /***/ noexcept { return inner_slots[inner_slots_index_for_value_at(index)]          ; }
		/***/ std::optional<value_type>& value_at               (const size_t& index) /***/ noexcept { return inner_slot_for_value_at(index).value_opt; }


	public:
		const std::optional<value_type>& value_at(const size_t& index) const noexcept { return inner_slot_for_value_at(index).value_opt; }

		read_slot slot_at(const size_t& index) const noexcept
			{
			const size_t inner_slots_index{inner_slots_index_for_value_at(index)};
			const inner_slot_t& inner_slot{inner_slots[inner_slots_index]};

			if (inner_slots_index == 0)
				{
				const read_slot ret
					{
					.region
						{
						.begin{0},
						.count{inner_slot.end}
						},
					.value_ptr{observer_for_optional(inner_slot.value_opt)}
					};
				return ret;
				}
			else
				{
				const inner_slot_t& previous_inner_slot{inner_slots[inner_slots_index - 1]};
				const read_slot ret
					{
					.region{region::create::from_to(previous_inner_slot.end, inner_slot.end)},
					.value_ptr{observer_for_optional(inner_slot.value_opt)}
					};
				return ret;
				}
			}

	public:
		regions() noexcept : 
			inner_slots{inner_slot_t
				{
				.value_opt{std::nullopt},
				.end{std::numeric_limits<size_t>::max()}
				}}
			{}

		void add(const value_type& new_value, const region& in_region) { add(std::optional<value_type>{new_value}, in_region); }
		void erase(const region& in_region) { add(std::nullopt, in_region); }
		void add(const std::optional<value_type>& new_value_opt, const region& in_region)
			{
			const size_t index_of_slot_starting_before_or_at_begin{inner_slots_index_for_value_at(in_region.begin)};
			inner_slot_t& slot_starting_before_or_at_begin{inner_slots[index_of_slot_starting_before_or_at_begin]};

			if (slot_starting_before_or_at_begin.value_opt == new_value_opt)
				{
				if (slot_starting_before_or_at_begin.end >= in_region.end()) 
					{ 
					return; 
					}

				//Index guaranteed to be different because otherwise we'd have entered the above if branch.
				const size_t index_of_slot_starting_before_or_at_end{inner_slots_index_for_value_at(in_region.end())};
				inner_slot_t& slot_starting_before_or_at_end{inner_slots[index_of_slot_starting_before_or_at_end]};

				if (slot_starting_before_or_at_end.value_opt == new_value_opt)
					{
					//Next slot has same value as previous slot, and anything in between was different. Erase everything in between and next, extend previous to the end of next
					const size_t new_end{slot_starting_before_or_at_end.end};
					slot_starting_before_or_at_begin.end = new_end;

					const auto erase_from{inner_slots.begin() + index_of_slot_starting_before_or_at_begin + 1};
					const auto erase_to  {inner_slots.begin() + index_of_slot_starting_before_or_at_end   + 1};
					inner_slots.erase(erase_from, erase_to);
					return;
					}
				else if (index_of_slot_starting_before_or_at_end == (index_of_slot_starting_before_or_at_begin + 1))
					{
					//The first slot that ends after new region's end is right after the last slot that begins before new region's begin.
					//No in-between regions to remove, just move the end value.
					slot_starting_before_or_at_begin.end = in_region.end();
					return;
					}
				else if (index_of_slot_starting_before_or_at_end == (index_of_slot_starting_before_or_at_begin + 1))
					{
					//There are in-between regions with different values to erase
					slot_starting_before_or_at_begin.end = in_region.end();

					const auto erase_from{inner_slots.begin() + index_of_slot_starting_before_or_at_begin + 1};
					const auto erase_to  {inner_slots.begin() + index_of_slot_starting_before_or_at_end};
					inner_slots.erase(erase_from, erase_to);
					return;
					}
				}
			}
	};































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

void regions()
	{
	utils::containers::regions<int> regions;
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

	const auto found_it{regions.find(35)};
	if (found_it != regions.end())
		{
		std::cout << "Found value :\"" << (*found_it) << "\" in region [" << found_it.region().begin << ", " << found_it.region().end() << "]\n";
		for (auto it{found_it}; it != regions.end(); it++)
			{
			if (it.has_value())
				{
				std::cout << (*it) << ", ";
				}
			}
		std::cout << "\n";
		}
	else { std::cout << "not found\n"; }
	}

int main()
	{
	aggregate_and_regions();
	regions();
	}