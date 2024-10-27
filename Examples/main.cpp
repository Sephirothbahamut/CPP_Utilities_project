#include "geometry_sdf_and_normal_texture.h"


extern void name                           ();
extern void flags                          ();
extern void logger                         ();
extern void object_pool                    ();
extern void tracking_ptr                   ();
extern void geometry_text_sdf_texture      ();
int main()
	{
	std::filesystem::create_directories("./output");

	utils::graphics::colour::rgba_f rgbaf{1.f, 2.f, 3.f, 4.f};
	utils::math::vec2f              v2f   {static_cast<utils::math::vec2f>(rgbaf)};
	utils::math::vec3f              v3f   {static_cast<utils::math::vec3f>(v2f)};
	utils::math::vec2s              v2s   {v2f};
	utils::graphics::colour::rgba_f rgbaf2{v3f};


	//name                           ();
	//flags                          ();
	//object_pool                    ();
	//tracking_ptr                   ();
	geometry_sdf_and_normal_texture geometry_sdf_and_normal_texture;
	//geometry_sdf_and_normal_texture.foreach_pixel();
	geometry_sdf_and_normal_texture.full_manual();
	geometry_text_sdf_texture();
	//logger                         ();
	}