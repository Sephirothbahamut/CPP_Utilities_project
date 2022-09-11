#pragma once

namespace utils::algorithm
	{
	template <class Bidirectional_iterator, class Unary_predicate>
	Bidirectional_iterator move_to_end_if(Bidirectional_iterator first, Bidirectional_iterator end, Unary_predicate predicate)
		{
		Bidirectional_iterator last = end; last--;
		Bidirectional_iterator current = first;

		while (current != last)
			{
			if (predicate(*current))
				{
				while (last != current)
					{
					if (predicate(*last)) { last--; }
					else { break; }
					}

				if (last != current)
					{
					auto tmp{std::move(*last)};
					*last = std::move(*current);
					*current = std::move(tmp);
					last--;
					}
				continue;
				}
			current++;
			}
		return current;
		}
	}