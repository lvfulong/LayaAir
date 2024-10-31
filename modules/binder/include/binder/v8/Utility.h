#ifndef __JSBIND_UTILITY_H__
#define __JSBIND_UTILITY_H__

#include <assert.h>
#include <map>
#include <string>

namespace jsbind
{

namespace internal
{

template <typename Func> struct function_traits;

template <typename R, typename... Args> struct function_traits<R(Args...)>
{
    using return_type = R;
    using arguments = std::tuple<Args...>;
};

// function pointer
template <typename R, typename... Args> struct function_traits<R (*)(Args...)> : function_traits<R(Args...)>
{
};

// member function pointer
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...)> : function_traits<R(C &, Args...)>
{
};

// const member function pointer
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const> : function_traits<R(const C &, Args...)>
{
};

// volatile member function pointer
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) volatile> : function_traits<R(volatile C &, Args...)>
{
};

// const volatile member function pointer
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const volatile> : function_traits<R(const volatile C &, Args...)>
{
};

// member object pointer
template <typename C, typename R> struct function_traits<R(C::*)> : function_traits<R(C &)>
{
};

///////////////////////////////////////////////////////////////////////////

template <typename F> using is_void_return = std::is_same<void, typename function_traits<F>::return_type>;
} // namespace internal
#define ABORT(msg)                                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        /*todo LOGE("%s:%d: fatal error: \"%s\"\n", __FILE__, __LINE__, #msg); */                                               \
        ::abort();                                                                                                     \
    } while (false)

#define CHECK(assertion)                                                                                               \
    if (!(assertion))                                                                                                  \
    {                                                                                                                  \
        ABORT(#assertion);                                                                                             \
    }
#if _DEBUG

#define DEBUG_CHECK(assertion) CHECK(assertion)

#else

#define DEBUG_CHECK(assertion)

#endif
template <class T> inline void UNUSED(T const &)
{
}
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
    std::string name;
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

class type_info
{
  public:
    constexpr std::string_view name() const
    {
        return name_;
    }
    constexpr bool operator==(type_info const &other) const
    {
        return name_ == other.name_;
    }
    constexpr bool operator!=(type_info const &other) const
    {
        return name_ != other.name_;
    }

  private:
    template <typename T> constexpr friend type_info type_id();

    constexpr explicit type_info(std::string_view name) : name_(name)
    {
    }

    std::string_view name_;
};

/// Get type information for type T
/// The idea is borrowed from https://github.com/Manu343726/ctti
template <typename T> constexpr type_info type_id()
{
#if defined(_MSC_VER) && !defined(__clang__)
    std::string_view name = __FUNCSIG__;
    const std::initializer_list<std::string_view> all_prefixes{"type_id<", "struct ", "class "};
    const std::initializer_list<std::string_view> any_suffixes{">"};
#elif defined(__clang__) || defined(__GNUC__)
    std::string_view name = __PRETTY_FUNCTION__;
    const std::initializer_list<std::string_view> all_prefixes{"T = "};
    const std::initializer_list<std::string_view> any_suffixes{";", "]"};
#else
#error "Unknown compiler"
#endif
    for (auto &&prefix : all_prefixes)
    {
        const auto p = name.find(prefix);
        if (p != name.npos)
        {
            name.remove_prefix(p + prefix.size());
        }
    }

    for (auto &&suffix : any_suffixes)
    {
        const auto p = name.rfind(suffix);
        if (p != name.npos)
        {
            name.remove_suffix(name.size() - p);
            break;
        }
    }

    return type_info(name);
}
} // namespace jsbind

#endif
