#include "geometry_sdf_and_normal_texture.h"

#include <utils/math/vec.h>

extern void name                           ();
extern void flags                          ();
extern void logger                         ();
extern void object_pool                    ();
extern void tracking_ptr                   ();
extern void geometry_text_sdf_texture      ();
int main()
	{

	utils::math::vec2f v{1.f, 2.f};
	utils::math::vec2f w{2.f, 3.f};
	w = v;

	std::filesystem::create_directories("./output");

	//name                           ();
	//flags                          ();
	//object_pool                    ();
	//tracking_ptr                   ();
	//geometry_sdf_and_normal_texture geometry_sdf_and_normal_texture;
	//geometry_sdf_and_normal_texture.foreach_pixel();
	//geometry_sdf_and_normal_texture.full_manual();
	//geometry_text_sdf_texture();
	logger                         ();
	}