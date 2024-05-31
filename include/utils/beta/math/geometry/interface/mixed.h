#pragma once

#include <span>
#include <variant>

#include "../details/base_types.h"
#include "../../../../compilation/debug.h"

#include "ab.h"
#include "bezier.h"
#include "polyline.h"

namespace utils::math::geometry::shape::details
	{
	using piece_variant_t = std::variant<shape::view::segment, shape::view::bezier<2>, shape::view::bezier<3>>;
	struct pieces_metadata_t
		{
		enum class type_t : uint8_t { segment, bezier_3pt, bezier_4pt, bezier } type;
		uint8_t count;
		};

	template <typename derived_T, ends_t ends>
	struct mixed : utils::math::geometry::shape::details::base<derived_T, shapes_enum::mixed>
		{
		protected:
			using derived_t = derived_T;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:

			inline static constexpr ends_t static_ends{ends};

			template <typename callback_t>
			utils_gpu_available constexpr void for_each(callback_t callback) const noexcept
				{
				size_t index_vertex{0};
				const auto& self{derived()};
				const auto& vertices{self.vertices};
				const auto& metadata{self.metadata};

				for (const pieces_metadata_t& piece_metadata : metadata)
					{
					switch (piece_metadata.type)
						{
						case pieces_metadata_t::type_t::segment:
							for (size_t i{0}; i < piece_metadata.count; i++)
								{
								const shape::segment segment{vertices[index_vertex], vertices[index_next(index_vertex)]};
								callback(segment);
								index_advance(index_vertex);
								}
							break;
						case pieces_metadata_t::type_t::bezier_3pt:
							for (size_t i{0}; i < piece_metadata.count; i++)
								{
								const std::span<const utils::math::vec2f, 3> bezier_control_points{vertices.data() + index_vertex, size_t{3}};
								const shape::view::bezier<true, 3> bezier{bezier_control_points};
								callback(bezier);
								index_advance(index_vertex, size_t{3 - 1});
								}
							break;
						case pieces_metadata_t::type_t::bezier_4pt:
							if (true)
								{
								const std::span<const utils::math::vec2f, 4> bezier_control_points{vertices.data() + index_vertex, size_t{4}};
								const shape::view::bezier<true, 4> bezier{bezier_control_points};
								callback(bezier);
								index_advance(index_vertex, size_t{4 - 1});
								}
							break;
						case pieces_metadata_t::type_t::bezier:
							if (true)
								{
								const std::span bezier_control_points{vertices.data() + index_vertex, size_t{piece_metadata.count}};
								const shape::view::bezier<true> bezier{bezier_control_points};
								callback(bezier);
								index_advance(index_vertex, piece_metadata.count - 1);
								}
							break;
						}
					}
				};

			void clear() noexcept
				{
				auto& self{derived()};
				self.vertices.clear();
				self.metadata.clear();
				}

			void add_first_vertex(const utils::math::vec2f& first_vertex)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (!vertices.empty()) { throw std::runtime_error("add_first_vertex should be only called on empty shapes"); }
				vertices.emplace_back(first_vertex);
				}

			void add_segment(const utils::math::vec2f& point2)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }

				vertices.emplace_back(point2);
				add_metadata(pieces_metadata_t::type_t::segment);
				}

			void add_segments(const std::span<utils::math::vec2f>& points)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }

				for (const auto& point : points) { vertices.emplace_back(point); }
				add_metadata(pieces_metadata_t::type_t::segment, points.size());
				}

			void add_bezier_3pt(const std::span<utils::math::vec2f, 2>& points)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }

				for (const auto& point : points) { vertices.emplace_back(point); }
				add_metadata(pieces_metadata_t::type_t::bezier_3pt);
				}

			void add_beziers_3pt(const std::span<utils::math::vec2f>& points)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }
				if (points.size() % 2 != 0) { throw std::runtime_error{"Adding multiple 3 points beziers you must pass vertices in multiples of 2"}; }

				for (const auto point : points) { vertices.emplace_back(point); }
				add_metadata(pieces_metadata_t::type_t::bezier_3pt);
				}

			void add_bezier_4pt(const std::span<utils::math::vec2f, 3>& points)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }

				for (const auto point : points) { vertices.emplace_back(point); }
				add_metadata(pieces_metadata_t::type_t::bezier_4pt);
				}

			void add_beziers_4pt(const std::span<utils::math::vec2f>& points)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }
				if (points.size() % 3 != 0) { throw std::runtime_error{"Adding multiple 4 points beziers you must pass vertices in multiples of 3"}; }

				for (const auto point : points) { vertices.emplace_back(point); }
				add_metadata(pieces_metadata_t::type_t::bezier_4pt);
				}

			void add_bezier(const std::span<utils::math::vec2f>& points)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};
				if (vertices.empty()) { throw std::runtime_error{"Specific add functions that imply the presence of a first vertex must only be called after add_first_vertex"}; }
				if (points.size() != 0) { throw std::runtime_error{"Adding a generic bezier it must have at least one point"}; }

				for (const auto point : points) { vertices.emplace_back(point); }
				add_metadata(pieces_metadata_t::type_t::bezier, points.size());
				}

			void add(const shape::concepts::ab auto& segment)
				{
				auto& self{derived()};
				auto& vertices{self.vertices};

				if (vertices.empty()) { vertices.emplace_back(segment.a); }
				else if (vertices[vertices.size() - 1] != segment.a)
					{
					throw std::runtime_error("Trying to add a segment to a mixed shape which A vertex doesn't match the last vertex of the shape.");
					}
				vertices.emplace_back(segment.b);
				add_metadata(pieces_metadata_t::type_t::segment);
				}
		
		private:
			void add_metadata(pieces_metadata_t::type_t type, size_t count = 1) noexcept
				{
				auto& self{derived()};
				auto& metadata{self.metadata};

				if (!metadata.empty())
					{
					auto& last_metadata{metadata[metadata.size() - 1]};
					if (last_metadata.type == type && (
						type == pieces_metadata_t::type_t::segment ||
						type == pieces_metadata_t::type_t::bezier_3pt ||
						type == pieces_metadata_t::type_t::bezier_4pt))
						{
						last_metadata.count += count;
						return;
						}
					}
				metadata.emplace_back(type, count);
				}

			utils_gpu_available size_t index_next(size_t index, size_t count = 1) const noexcept
				{
				if constexpr (static_ends.is_closed()) { return (index + 1) % derived().vertices.size(); }
				else { return index + 1; }
				}
			utils_gpu_available size_t& index_advance(size_t& index, size_t count = 1) const noexcept
				{
				return index = index_next(index, count);
				}
		};
	}

namespace utils::math::geometry::shape
	{
	template <ends_t ends = ends_t::create::closed()>
	struct mixed : utils::math::geometry::shape::details::mixed<mixed<ends>, ends>
		{
		std::vector<vec2f> vertices;
		std::vector<details::pieces_metadata_t> metadata;
		};

	namespace view
		{
		template <bool const_vertices = false, ends_t ends = ends_t::create::closed()>
		struct mixed : utils::math::geometry::shape::details::mixed<mixed<const_vertices, ends>, ends>
			{
			template <typename T> 
			using constness = std::conditional_t<const_vertices, const T, T>;

			using vertex_t = constness<vec2f>;
			using span_t   = std::span<vertex_t>;
			using span_metadata_t = std::span<constness<details::pieces_metadata_t>>;
			utils_gpu_available mixed(span_t vertices, span_metadata_t metadata) : vertices(vertices), metadata(metadata) {}

			span_t vertices;
			span_metadata_t metadata;
			};
		}
	}