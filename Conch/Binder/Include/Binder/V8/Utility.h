#ifndef __BIND_CORE_H__
#define __BIND_CORE_H__
#include <assert.h>
#include <map>
#include <string>
#include <v8.h>
namespace laya
{

template <class G, class S> struct PropFuncInfo
{
    G fGet;
    S fSet;
    PropFuncInfo(G g, S s)
    {
        fGet = g;
        fSet = s;
    }
    PropFuncInfo(G g)
    {
        fGet = g;
        fSet = 0; 
    }
};

template <typename T> struct FuncInfo
{
    T func;
#if 0
    std::string name;
#endif
    FuncInfo(T f)
    {
        func = f;
    }
};
template <typename T> class value_object_base
{
};
namespace internal
{

template <typename T> union ptr_cast {
    static_assert(sizeof(T) <= sizeof(void *), "size of object in ptr_cast must not be greater than ptr size");
    T value;
    void *ptr;

    explicit ptr_cast(void *p) : ptr(p)
    {
    }

    explicit ptr_cast(T v) : value(v)
    {
    }

    operator void *() const
    {
        return ptr;
    }
    operator T() const
    {
        return value;
    }
};

template <typename T> struct is_string : std::false_type
{
};

template <typename Char, typename Traits, typename Alloc>
struct is_string<std::basic_string<Char, Traits, Alloc>> : std::true_type
{
};

template <typename Char, typename Traits> struct is_string<std::basic_string_view<Char, Traits>> : std::true_type
{
};

template <> struct is_string<char const *> : std::true_type
{
};

template <> struct is_string<char16_t const *> : std::true_type
{
};

template <> struct is_string<char32_t const *> : std::true_type
{
};

template <> struct is_string<wchar_t const *> : std::true_type
{
};


template <typename T, typename U = void> struct is_sequence_impl : std::false_type
{
};

template <typename T>
struct is_sequence_impl<
    T, std::void_t<typename T::value_type, decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()),
                   decltype(std::declval<T>().emplace_back(std::declval<typename T::value_type>()))>>
    : std::negation<is_string<T>>
{
};

template <typename T> using is_sequence = is_sequence_impl<T>;
} // namespace internal
} // namespace laya

#endif
