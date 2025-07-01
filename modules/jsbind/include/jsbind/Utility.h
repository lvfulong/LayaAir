#ifndef __JSBIND_UTILITY_H__
#define __JSBIND_UTILITY_H__

#include <assert.h>
#include <functional>
#include <map>
#include <string>
#include <utils/Log.h>

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

namespace internal
{
template <typename T> class value_object_base
{
};
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
template<typename T, typename U = void>
struct is_mapping_impl : std::false_type
{
};

template<typename T>
struct is_mapping_impl<T, std::void_t<typename T::key_type, typename T::mapped_type,
    decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type
{
};

template<typename T>
using is_mapping = is_mapping_impl<T>;
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

template<typename T>
struct is_shared_ptr : std::false_type
{
};

template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
{
};
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

namespace internal
{
extern void addDeinitializer(std::function<void()> func);
template <typename ClassType , typename Traits> static void destructor(jsvm_env env, void *nativeObject, void * /*finalize_hint*/);
template <typename ClassType> void raw_destructor(ClassType *pointer)
{
    delete pointer;
}

template <typename T> struct remove_class;
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...)>
{
    using type = R(A...);
};
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...) const>
{
    using type = R(A...);
};
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...) volatile>
{
    using type = R(A...);
};
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...) const volatile>
{
    using type = R(A...);
};

template <typename LambdaType> using LambdaSignature = typename remove_class<decltype(&LambdaType::operator())>::type;
} // end namespace internal

// requires captureless lambda because implicitly coerces to function pointer
template <typename LambdaType> internal::LambdaSignature<LambdaType> *optional_override(const LambdaType &fp)
{
    return fp;
}

template <typename Signature> Signature *select_overload(Signature *fn)
{
    return fn;
}

template <typename Signature, typename ClassType> auto select_overload(Signature(ClassType::*fn)) -> decltype(fn)
{
    return fn;
}

template <typename ClassType, typename ReturnType, typename... Args>
auto select_const(ReturnType (ClassType::*method)(Args...) const) -> decltype(method)
{
    return method;
}

struct raw_ptr_traits
{
	using pointer_type = void*;
	using const_pointer_type = void const*;

	template<typename T>
	using object_pointer_type = T*;
	template<typename T>
	using object_const_pointer_type = T const*;

	using object_id = void*;

	static object_id pointer_id(void* ptr) { return ptr; }
	static pointer_type to_pointer_type(object_id id) { return id; }
	static pointer_type const_pointer_cast(const_pointer_type ptr) { return const_cast<void*>(ptr); }
	template<typename T, typename U>
	static T* static_pointer_cast(U* ptr) { return static_cast<T*>(ptr); }

	//template<typename T>
	//using convert_ptr = convert<T*>;

	//template<typename T>
	//using convert_ref = convert<T&>;

	template<typename T, typename... Args>
	static object_pointer_type<T> create(Args&&... args)
	{
		return new T(std::forward<Args>(args)...);
	}

	template<typename T>
	static object_pointer_type<T> clone(T const& src)
	{
		return new T(src);
	}

	template<typename T>
	static void destroy(object_pointer_type<T> const& ptr)
	{
		delete ptr;
	}

	template<typename T>
	static size_t object_size(object_pointer_type<T> const&)
	{
		return sizeof(T);
	}
};

struct ref_from_shared_ptr
{
};

struct shared_ptr_traits
{
	using pointer_type = std::shared_ptr<void>;
	using const_pointer_type = std::shared_ptr<void const>;

	template<typename T>
	using object_pointer_type = std::shared_ptr<T>;
	template<typename T>
	using object_const_pointer_type = std::shared_ptr<T const>;

	using object_id = void*;

	static object_id pointer_id(pointer_type const& ptr) { return ptr.get(); }
	static pointer_type to_pointer_type(object_id id) { return std::shared_ptr<void>(id, [](void*) {}); }
	static pointer_type const_pointer_cast(const_pointer_type const& ptr) { return std::const_pointer_cast<void>(ptr); }
	template<typename T, typename U>
	static std::shared_ptr<T> static_pointer_cast(std::shared_ptr<U> const& ptr) { return std::static_pointer_cast<T>(ptr); }

	//template<typename T>
	//using convert_ptr = convert<std::shared_ptr<T>>;

	//template<typename T>
	//using convert_ref = convert<T, ref_from_shared_ptr>;

	template<typename T, typename... Args>
	static object_pointer_type<T> create(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	static object_pointer_type<T> clone(T const& src)
	{
		return std::make_shared<T>(src);
	}

	template<typename T>
	static void destroy(object_pointer_type<T> const&)
	{
	}

	template<typename T>
	static size_t object_size(object_pointer_type<T> const&)
	{
		return sizeof(T);
	}
};
} // namespace jsbind

#endif
