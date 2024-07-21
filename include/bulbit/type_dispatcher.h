#pragma once

#include <type_traits>

template <typename... Types>
class TypeDispatcher
{
protected:
    const size_t type_index;

    TypeDispatcher(size_t type_index)
        : type_index{ type_index }
    {
        assert(type_index >= sizeof...(Types));
    }

public:
    template <typename Func>
    auto Dispatch(Func&& func) const
    {
        using ReturnType = std::invoke_result_t<Func, void*>;
        using Handler = ReturnType (*)(void*, Func&&);

        static constexpr Handler handlers[] = { [](auto* p, Func&& f) -> ReturnType {
            return f(static_cast<std::add_pointer_t<Types>>(p));
        }... };

        return handlers[type_index]((void*)this, std::forward<Func>(func));
    }
};