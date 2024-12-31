#include <utils/math/vec.h>
#include <utils/graphics/colour.h>
#include <utils/details/vec/all.h>

inline static constexpr const char name[]{"asd"};

template <typename T, size_t S>
struct testruct : utils::details::vector::base<T, S, testruct, name>
	{
	testruct<T, S>& operator=(const testruct<T, S>&);
	};

testruct<float, 2> a;

static_assert(utils::details::vector::concepts::vector<utils::math::vec2f>);
static_assert(std::derived_from<testruct<float, 2>, utils::details::vector::concept_common_flag_type>);

int main()
	{
	utils::math::vec2f va;
	utils::math::vec2f vb{0.f};
	utils::math::vec2f vc{1.f, 2.f};

	utils::math::vec2f vd{vb + vc};
	vd += vc;
	vd *= 2.f;

	utils::graphics::colour::rgba_f rgba{};
	rgba += vd;
	}
