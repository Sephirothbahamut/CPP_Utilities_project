#pragma once
#include <utility>

namespace utils::oop
	{
	struct counting_invalidating_move
		{
		public:
			counting_invalidating_move () noexcept { _count++; }
			~counting_invalidating_move() noexcept { if (_valid) { _count--; } }

			counting_invalidating_move(const counting_invalidating_move& copy) noexcept { *this = copy; }
			counting_invalidating_move& operator=(const counting_invalidating_move& copy) noexcept
				{
				if (!_valid &&  copy._valid) { _count++; }
				if ( _valid && !copy._valid) { _count--; }
				_valid = copy._valid;

				return *this;
				}

			counting_invalidating_move(counting_invalidating_move&& move) noexcept { *this = std::move(move); }
			counting_invalidating_move& operator=(counting_invalidating_move&& move) noexcept
				{
				if (!_valid && move._valid) { _valid = true; move._valid = false; }
				else if (_valid)
					{
					if (move._valid) { move._valid = false; }
					_count--;
					}

				return *this;
				}

			static void reset() { _count = 0; }

			inline static size_t count() noexcept { return _count; }

			inline bool valid() const noexcept { return _valid; }

		private:
			bool _valid{true};

			inline static size_t _count{0};
		};

	struct counting
		{
		public:
			counting() noexcept { _count++; }
			~counting() noexcept { _count--; }

			counting(const counting&) noexcept { _count++; }
			counting& operator=(const counting&) noexcept = default;

			counting(counting&&) noexcept { _count++; }
			counting& operator=(counting&&) noexcept = default;

			static void reset() { _count = 0; }

			inline static size_t count() noexcept { return _count; }

		private:
			inline static size_t _count{0};
		};
	}