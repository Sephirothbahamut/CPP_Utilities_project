#pragma once
#include <array>

namespace utils::containers::array
	{
	template <typename in_t, size_t in_size, typename out_t, size_t out_size>
	std::array<out_t, out_size>& copy(const std::array<in_t, in_size>& in, std::array<out_t, out_size>& out, const out_t& default_value = {})
		{
		size_t i = 0;
		if constexpr (out_size <= in_size)
			{
			for (; i < out_size; i++) { out[i] = static_cast<out_t>(in[i]); }
			}
		else if constexpr (out_size > in_size)
			{
			for (; i < in_size ; i++) { out[i] = static_cast<out_t>(in[i]); }
			for (; i < out_size; i++) { out[i] = default_value; }
			}
		return out;
		}
	}