#include <vector>
#include <optional>
#include <iostream>

#include "Library/include/utils/aggregate.h"
#include "Library/include/utils/containers/aggregate_regions.h"


namespace utils::beta
	{
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






	template <typename T>
	class regions
		{
		public:
			using value_type = T;

			struct read_slot
				{
				using value_type = regions::value_type;
				const region region;
				const value_type& value;
				};

		private:
			struct inner_slot_t
				{
				value_type value;
				size_t end{std::numeric_limits<size_t>::max()};
				};
			using inner_slots_t = std::vector<inner_slot_t>;

			inner_slots_t inner_slots;

			size_t inner_slots_index_for_value_at(const size_t& index, const size_t& inner_slots_starting_index = 0) const noexcept
				{
				for (size_t index_in_slots{inner_slots_starting_index}; index_in_slots < inner_slots.size(); index_in_slots++)
					{
					const inner_slot_t& inner_slot{inner_slots[index_in_slots]};
					if (inner_slot.end > index) 
						{
						return index_in_slots;
						}
					}
				std::unreachable();
				}
			const inner_slot_t& inner_slot_for_value_at(const size_t& index) const noexcept { return inner_slots[inner_slots_index_for_value_at(index)]; }
			/***/ inner_slot_t& inner_slot_for_value_at(const size_t& index) /***/ noexcept { return inner_slots[inner_slots_index_for_value_at(index)]; }
			/***/ value_type  & value_at               (const size_t& index) /***/ noexcept { return inner_slot_for_value_at(index).value; }


		public:
			const value_type& value_at(const size_t& index) const noexcept { return inner_slot_for_value_at(index).value; }

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
						.value{inner_slot.value}
						};
					return ret;
					}
				else
					{
					const inner_slot_t& previous_inner_slot{inner_slots[inner_slots_index - 1]};
					const read_slot ret
						{
						.region{region::create::from_to(previous_inner_slot.end, inner_slot.end)},
						.value{inner_slot.value}
						};
					return ret;
					}
				}

		public:
			regions(const value_type& starting_value) noexcept : 
				inner_slots{inner_slot_t
					{
					.value{starting_value},
					.end{std::numeric_limits<size_t>::max()}
					}}
				{}

			void add(const value_type& new_value, const region& in_region)
				{
				const size_t index_of_slot_starting_before_or_at_begin{inner_slots_index_for_value_at(in_region.begin)};
				inner_slot_t& slot_starting_before_or_at_begin{inner_slots[index_of_slot_starting_before_or_at_begin]};

				if (slot_starting_before_or_at_begin.value != new_value)
					{
					const size_t begin_of_slot_starting_before_or_at_begin{index_of_slot_starting_before_or_at_begin == 0 ? static_cast<size_t>(0) : inner_slots[index_of_slot_starting_before_or_at_begin - 1].end};
					if (begin_of_slot_starting_before_or_at_begin == in_region.begin)
						{
						if (slot_starting_before_or_at_begin.end > in_region.end())
							{
							const inner_slot_t new_slot
								{
								.value{new_value},
								.end{in_region.end()}
								};

							inner_slots.insert(inner_slots.begin() + index_of_slot_starting_before_or_at_begin, new_slot);
							return;
							}
						else
							{
							slot_starting_before_or_at_begin.value = new_value;
							slot_starting_before_or_at_begin.end = in_region.end();
							add_inner_guaranteed_previous_contains_same_value(index_of_slot_starting_before_or_at_begin, new_value, in_region);
							return;
							}
						}
					else if (slot_starting_before_or_at_begin.end > in_region.end())
						{
						const inner_slot_t new_slot
							{
							.value{new_value},
							.end{in_region.end()}
							};
						const inner_slot_t replicate_previous_after_new
							{
							.value{slot_starting_before_or_at_begin.value},
							.end{slot_starting_before_or_at_begin.end}
							};

						slot_starting_before_or_at_begin.end = in_region.begin;

						inner_slots.insert(inner_slots.begin() + index_of_slot_starting_before_or_at_begin + 1, new_slot);
						inner_slots.insert(inner_slots.begin() + index_of_slot_starting_before_or_at_begin + 2, replicate_previous_after_new);
						return;
						}
					else //if (slot_starting_before_or_at_begin.end <= in_region.end() %% begin_of_... != in_region.begin) 
						{
						slot_starting_before_or_at_begin.end = in_region.begin;
						add(new_value, in_region);
						return;
						}
					}

				if (slot_starting_before_or_at_begin.value == new_value)
					{
					add_inner_guaranteed_previous_contains_same_value(index_of_slot_starting_before_or_at_begin, new_value, in_region);
					}
				}
	
			class const_forward_iterator
				{
				private:
					friend class regions;

					using difference_type = std::ptrdiff_t;
					using value_type = regions::value_type;

					size_t begin{0};
					inner_slots_t::const_iterator inner_it;

					const_forward_iterator(size_t begin, inner_slots_t::const_iterator inner_it) :
						begin   {begin   },
						inner_it{inner_it}
						{
						}

				public:
					const_forward_iterator& operator++() noexcept
						{
						begin = inner_it->end;
						++inner_it;
						return *this;
						}
					const_forward_iterator operator++(int) noexcept
						{
						auto tmp{*this};
						++(*this);
						return tmp;
						}

					bool operator==(const const_forward_iterator& other) const noexcept
						{
						return inner_it == other.inner_it;
						}

					const read_slot operator*() const noexcept 
						{
						const read_slot ret
							{
							.region{region()},
							.value {value ()}
							};
						return ret; 
						}

					const value_type& value() const noexcept { return inner_it->value; }

					const region region() const noexcept
						{
						const auto ret{utils::beta::region::create::from_to(begin, inner_it->end)};
						return ret;
						}

					bool is_end() const noexcept 
						{
						const bool ret{begin == std::numeric_limits<size_t>::max()};
						return ret;
						}
				};

			const_forward_iterator begin() const noexcept
				{
				const const_forward_iterator ret
					{
					static_cast<size_t>(0),
					inner_slots.begin()
					};
				return ret;
				}
			const_forward_iterator end() const noexcept
				{
				const const_forward_iterator ret
					{
					std::numeric_limits<size_t>::max(),
					inner_slots.end()
					};
				return ret;
				}

			private:
				void add_inner_guaranteed_previous_contains_same_value(size_t index_of_slot_starting_before_or_at_begin, const value_type& new_value, const region& in_region)
					{
					inner_slot_t& slot_starting_before_or_at_begin{inner_slots[index_of_slot_starting_before_or_at_begin]};

					if (slot_starting_before_or_at_begin.end >= in_region.end())
						{
						return;
						}

					//Index guaranteed to be different because otherwise we'd have entered the above if branch.
					const size_t index_of_slot_starting_before_or_at_end{inner_slots_index_for_value_at(in_region.end(), index_of_slot_starting_before_or_at_begin)};
					inner_slot_t& slot_starting_before_or_at_end{inner_slots[index_of_slot_starting_before_or_at_end]};

					if (slot_starting_before_or_at_end.value == new_value)
						{
						//Next slot has same value as previous slot, and anything in between was different. Erase everything in between and next, extend previous to the end of next
						const size_t new_end{slot_starting_before_or_at_end.end};
						slot_starting_before_or_at_begin.end = new_end;

						const auto erase_from{inner_slots.begin() + index_of_slot_starting_before_or_at_begin + 1};
						const auto erase_to{inner_slots.begin() + index_of_slot_starting_before_or_at_end + 1};
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
						const auto erase_to{inner_slots.begin() + index_of_slot_starting_before_or_at_end};
						inner_slots.erase(erase_from, erase_to);
						return;
						}
					std::unreachable();
					}
		};
	}



void new_regions()
	{
	utils::beta::regions<float> regions{std::numeric_limits<float>::quiet_NaN()};

	regions.add(0.f, utils::beta::region::create::from_to(10, 20));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}
	std::cout << "\n________________________________\n" << std::endl;

	regions.add(1.f, utils::beta::region::create::from_to(20, 25));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}
	std::cout << "\n________________________________\n" << std::endl;

	regions.add(2.f, utils::beta::region::create::from_to(8, 10));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}

	std::cout << "\n________________________________\n" << std::endl;
	regions.add(3.f, utils::beta::region::create::from_to(6, 15));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}

	std::cout << "\n________________________________\n" << std::endl;
	regions.add(4.f, utils::beta::region::create::from_to(6, 20));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}

	std::cout << "\n________________________________\n" << std::endl;
	regions.add(5.f, utils::beta::region::create::from_to(19, std::numeric_limits<size_t>::max()));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}

	std::cout << "\n________________________________\n" << std::endl;
	regions.add(4.f, utils::beta::region::create::from_to(19, 20));

	for (const auto& slot : regions)
		{
		std::cout << "[" << slot.region.begin << "-" << slot.region.end() << "): " << slot.value << "\n";
		}
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
	//aggregate_and_regions();
	//regions();

	new_regions();
	}