#pragma once

#include <span>
#include <variant>

#include "../details/base_types.h"
#include "../../../oop/disable_move_copy.h"

#include "ab.h"
#include "bezier.h"
#include "vertices.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		// TODO use extent for piece metadata's size, and calculate the vertices extent based on it

		/// <summary> Only use dynamic extent, static extent not supported (yet)</summary>
		template <storage::type storage_type, size_t extent = std::dynamic_extent>
		struct mixed : vertices<storage_type, ENDS, extent>
			{
			using vertices_t = vertices<storage_type, ENDS, extent>;
			using vertices_t::size;
			using vertices_t::ends;
			using vertices_t::vertices;
			using vertices_t::operator[];
			using typename vertices_t::value_type;
			using typename vertices_t::const_aware_value_type;

			using self_t = polyline<storage_type, ends, extent>;
			using nonref_self_t = polyline<storage::type::create::owner(), ends, extent>;

			struct piece_metadata_t
				{
				enum class type_t : uint8_t { segment, bezier_3pt, bezier_4pt, bezier } type;
				uint8_t index;
				};

			utils::storage::multiple<storage::storage_type_for<piece_metadata_t, storage_type>, extent, true> pieces_metadata;

			template <bool is_view_const>
			struct pieces_view : std::ranges::view_interface<pieces_view<is_view_const>>, ::utils::oop::non_copyable, ::utils::oop::non_movable
				{
				using const_aware_mixed_t = std::conditional_t<is_view_const, const self_t, self_t>;
				const_aware_mixed_t& mixed_ref;

				inline static constexpr auto pieces_storage_type{storage::type::create::observer(is_view_const)};
				using edge_t       = utils::math::geometry::shape::generic::segment <pieces_storage_type                     >;
				using bezier_3pt_t = utils::math::geometry::shape::generic::bezier  <pieces_storage_type, 3                  >;
				using bezier_4pt_t = utils::math::geometry::shape::generic::bezier  <pieces_storage_type, 4                  >;
				using bezier_t     = utils::math::geometry::shape::generic::bezier  <pieces_storage_type, std::dynamic_extent>;

				//TODO iterator on variant_t
				using variant_t = std::variant<edge_t, bezier_3pt_t, bezier_4pt_t, bezier_t>;

				template <typename callback_t>
				call_at(size_t index_piece, callback) noexcept
					{
					const piece_metadata_t piece_metadata{mixed_ref.pieces_metadata[index_piece]};
					const size_t index_vertex_begin{piece_metadata.index};
					const size_t index_vertex_end  {index_piece < (mixed_ref.pieces_metadata.size() - 1) ? mixed_ref.pieces_metadata[mixed_ref.pieces_metadata.size() - 1] : mixed_ref.size()};

					switch (piece_metadata.type)
						{
						case piece_metadata_t::type_t::segment   : call_segment   (index_vertex_begin, index_vertex_end, callback);
						case piece_metadata_t::type_t::bezier_3pt: call_bezier_3pt(index_vertex_begin, index_vertex_end, callback);
						case piece_metadata_t::type_t::bezier_4pt: call_bezier_4pt(index_vertex_begin, index_vertex_end, callback);
						case piece_metadata_t::type_t::bezier    : call_bezier    (index_vertex_begin, index_vertex_end, callback);
						}
					}

				template <typename callback_t>
				void call_segments(size_t index_vertex_begin, size_t index_vertex_end, callback_t callback) noexcept
					{
					for (size_t i = 0; i < length; i++)
						{

						}

					utils::math::geometry::shape::observer::segment segment{mixed_ref[index_vertex_begin]};
					}
				};

			utils_gpu_available constexpr auto get_pieces() const noexcept { return pieces_view<true                   >{vertices_t::storage.begin(), vertices_t::storage.size()}; }
			utils_gpu_available constexpr auto get_pieces()       noexcept { return pieces_view<storage_type.is_const()>{vertices_t::storage.begin(), vertices_t::storage.size()}; }

			};
	}
