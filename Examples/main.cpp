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

	//name                           ();
	//flags                          ();
	//object_pool                    ();
	//tracking_ptr                   ();
	//geometry_sdf_and_normal_texture geometry_sdf_and_normal_texture;
	//geometry_sdf_and_normal_texture.foreach_pixel();
	//geometry_sdf_and_normal_texture.full_manual();
	geometry_text_sdf_texture();
	//logger                         ();
	}