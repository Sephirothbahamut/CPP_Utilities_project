#pragma once
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <vector>
#include <cassert>


namespace//private
    {
    class A { virtual void a() const = 0; };
    class B : public A { virtual void a() const override {}; };
    class C : public A { virtual void a() const override {}; };
    }

namespace utils
	{
    template<typename Base, typename ... Children>
    struct polymorphic_storage
        {
        static_assert(std::is_polymorphic_v<Base>);
        static_assert((std::is_base_of_v<Base, Children> && ...));

        constexpr static auto size = std::max({sizeof(Children) ...});
        constexpr static auto align = std::max({alignof(Children) ...});

        bool active = false;
        typename std::aligned_storage<size, align>::type data;

        polymorphic_storage() = default;

        template<typename T>
        polymorphic_storage(const T& arg)
            {
            emplace<T>(arg);
            }

        template<typename T, typename ... Args >
        void emplace(Args&& ... args)
            {
            static_assert((std::is_same_v<T, Children> || ...));
            destroy();
            active = true;
            new (&data) T(std::forward<Args>(args) ...);
            }

        Base* get_base() { assert(active); return reinterpret_cast<Base*>(&data); }
        const Base* get_base() const { assert(active); return reinterpret_cast<const Base*>(&data); }

        Base* operator->() { return get_base(); }
        const Base* operator->() const { return get_base(); }
        Base* operator*() { return get_base(); }
        const Base* operator*() const { return get_base(); }

        void destroy()
            {
            if (active)
                {
                std::destroy_at(get_base());
                }
            }

        ~polymorphic_storage()
            {
            destroy();
            }
        };
	}