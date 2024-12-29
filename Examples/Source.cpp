#include <utils/math/vec.h>

utils::math::vec2f v;
//static_assert(std::derived_from<utils::math::vec2f, utils::details::vector::concept_common_flag_type>);
static_assert(std::is_base_of_v<utils::details::vector::concept_common_flag_type, utils::math::vec2f>);
//static_assert(__is_base_of(utils::details::vector::concept_common_flag_type, utils::math::vec2f));
//static_assert(__is_convertible_to(const volatile utils::math::vec2f*, const volatile utils::details::vector::concept_common_flag_type*));

void f()
	{
	utils::math::vec2f v0{0.f};
	utils::math::vec2f v1{0.f, 1.f};

	//static_assert(std::derived_from<utils::math::vec2f, utils::details::vector::concept_common_flag_type>);
	static_assert(std::is_base_of_v<utils::details::vector::concept_common_flag_type, utils::math::vec2f>);
	//static_assert(__is_base_of(utils::details::vector::concept_common_flag_type, utils::math::vec2f));
	//static_assert(__is_convertible_to(const volatile utils::math::vec2f*, const volatile utils::details::vector::concept_common_flag_type*));

	v + v;
	v += v;
	v = v + v;
	}

