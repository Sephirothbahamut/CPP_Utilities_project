#include "all.h"

namespace utils::math::geometry
	{
	template <bool VIEW>
	struct group
		{
		inline static constexpr bool view{VIEW};

		struct indices_t
			{
			size_t index_vertices;
			size_t index_shape_metadata;
			};

		struct element_metadata
			{
			geometry::shapes_enum shape_type;
			indices_t indices;
			geometry::shape::aabb bounding_box;
			};

		template <typename T>
		using container_t = std::conditional_t<view, std::span<const T>, std::vector<T>>;

		container_t<utils::math::vec2f> vertices;

		struct shapes_metadata_t
			{
			struct mixed_t
				{
				size_t pieces_metadata_begin;
				size_t pieces_metadata_count;
				size_t vertices_count;
				};
			struct circle_t
				{
				float radius;
				};

			container_t<mixed_t> mixed;
			container_t<geometry::shape::details::pieces_metadata_t> mixed_pieces;
			container_t<circle_t> circles;
			};
		shapes_metadata_t shapes_metadata;
		container_t<element_metadata> elements_metadata;

		template <typename callback_t>
		utils_gpu_available void for_each(callback_t callback) const noexcept
			{
			for (const auto& element : elements_metadata)
				{
				switch (element.shape_type)
					{
					case shapes_enum::mixed: if (true)
						{
						const shapes_metadata_t::mixed_t& mixed_metadata_indices{shapes_metadata.mixed[element.indices.index_shape_metadata]};

						const shape::view::mixed<true>::span_t          vertices{vertices.begin() + element.indices.index_vertices, mixed_metadata_indices.vertices_count};
						const shape::view::mixed<true>::span_metadata_t metadata{shapes_metadata.mixed_pieces.begin() + mixed_metadata_indices.pieces_metadata_begin, mixed_metadata_indices.pieces_metadata_count};
						const shape::view::mixed<true> mixed{vertices, metadata};

						callback(mixed);
						} break;
					case shapes_enum::circle: if (true)
						{
						const shape::circle circle{vertices[element.indices.index_vertices], shapes_metadata.circles[element.indices.index_shape_metadata].radius};
						callback(circle);
						} break;
					case shapes_enum::segment: if (true)
						{
						const shape::segment segment{vertices[element.indices.index_vertices], vertices[element.indices.index_vertices + 1]};
						callback(segment);
						} break;
					}
				}
			}

		void add(const utils::math::geometry::shape::concepts::mixed auto& mixed)
			requires(!view)
			{
			element_metadata element_metadata
				{
				.shape_type{shapes_enum::mixed},
				.indices
					{
					.index_vertices{vertices.size()},
					.index_shape_metadata{shapes_metadata.mixed.size()}
					}
				};
			shapes_metadata.mixed.push_back(typename shapes_metadata_t::mixed_t
				{
				.pieces_metadata_begin{shapes_metadata.mixed_pieces.size()},
				.pieces_metadata_count{mixed.metadata.size()},
				.vertices_count{mixed.vertices.size()}
				});

			vertices.insert(vertices.begin(), mixed.vertices.begin(), mixed.vertices.end());
			shapes_metadata.mixed_pieces.insert(shapes_metadata.mixed_pieces.begin(), mixed.metadata.begin(), mixed.metadata.end());

			//element_metadata.bounding_box = mixed.bounding_box();
			elements_metadata.push_back(element_metadata);
			}

		void add(const utils::math::geometry::shape::concepts::circle auto& circle)
			requires(!view)
			{
			element_metadata element_metadata
				{
				.shape_type{shapes_enum::circle},
				.indices
					{
					.index_vertices{vertices.size()},
					.index_shape_metadata{shapes_metadata.circles.size()}
					}
				};
			shapes_metadata.circles.push_back({circle.radius});

			vertices.push_back(circle.center);

			//element_metadata.bounding_box = mixed.bounding_box();
			elements_metadata.push_back(element_metadata);
			}

		void add(const utils::math::geometry::shape::concepts::aabb auto& aabb)
			requires(!view)
			{
			element_metadata element_metadata
				{
				.shape_type{shapes_enum::aabb},
				.indices{.index_vertices{vertices.size()}},
				.bounding_box{aabb}
				};

			vertices.push_back(aabb.ul());
			vertices.push_back(aabb.dr());

			elements_metadata.push_back(element_metadata);
			}

		void add(const utils::math::geometry::shape::concepts::segment auto& segment)
			requires(!view)
			{
			element_metadata element_metadata
				{
				.shape_type{shapes_enum::segment},
				.indices{.index_vertices{vertices.size()}},
				//.bounding_box{segment}
				};

			vertices.push_back(segment.a);
			vertices.push_back(segment.b);

			elements_metadata.push_back(element_metadata);
			}
		};
	}