#ifndef DEF_HPP
#define DEF_HPP

template <std::size_t Index, class First, class... Rest>
struct TypeAt
{
    static_assert(Index >= 0, "Index must not be smaller than 0.");
    static_assert(Index < 1 + sizeof...(Rest), "Index must not be bigger than number of types.");

    using CurrentType = TypeAt<Index - 1, Rest...>::CurrentType;
    template <template <class...> class TargetTemplate>
    using RestType = TypeAt<Index - 1, Rest...>::template RestType<TargetTemplate>;
    template <template <class...> class TargetTemplate>
    using AllType = TypeAt<Index - 1, Rest...>::template AllType<TargetTemplate>;
};

template <class First, class... Rest>
struct TypeAt<0, First, Rest...>
{
    using CurrentType = First;
    template <template <class...> class TargetTemplate>
    using RestType = TargetTemplate<Rest...>;
    template <template <class...> class TargetTemplate>
    using AllType = TargetTemplate<First, Rest...>;
};

#endif // DEF_HPP