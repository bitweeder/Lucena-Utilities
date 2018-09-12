/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Variant.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of libc++’s <variant> implementation,
	used under the University of Illinois/NCSA Open Source License. See
	“license/libc++ License” for details.

	The notable differences from the libc++ version include:

	* namespace changes
	* removal of some unneeded preprocessor tests and attributes
	* renaming of various macros to use LUL versions
	* renaming of system-reserved symbols

	SEEME This header requires a source file to define bad_variant_access.

------------------------------------------------------------------------------*/


#pragma once

#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif


/*
   variant synopsis

namespace std {

  // 20.7.2, class template variant
  template <class... Types>
  class variant {
  public:

    // 20.7.2.1, constructors
    constexpr variant() noexcept(see below);
    variant(const variant&);
    variant(variant&&) noexcept(see below);

    template <class T> constexpr variant(T&&) noexcept(see below);

    template <class T, class... Args>
    constexpr explicit variant(in_place_type_t<T>, Args&&...);

    template <class T, class U, class... Args>
    constexpr explicit variant(
        in_place_type_t<T>, initializer_list<U>, Args&&...);

    template <size_t I, class... Args>
    constexpr explicit variant(in_place_index_t<I>, Args&&...);

    template <size_t I, class U, class... Args>
    constexpr explicit variant(
        in_place_index_t<I>, initializer_list<U>, Args&&...);

    // 20.7.2.2, destructor
    ~variant();

    // 20.7.2.3, assignment
    variant& operator=(const variant&);
    variant& operator=(variant&&) noexcept(see below);

    template <class T> variant& operator=(T&&) noexcept(see below);

    // 20.7.2.4, modifiers
    template <class T, class... Args>
    T& emplace(Args&&...);

    template <class T, class U, class... Args>
    T& emplace(initializer_list<U>, Args&&...);

    template <size_t I, class... Args>
    variant_alternative_t<I, variant>& emplace(Args&&...);

    template <size_t I, class U, class...  Args>
    variant_alternative_t<I, variant>& emplace(initializer_list<U>, Args&&...);

    // 20.7.2.5, value status
    constexpr bool valueless_by_exception() const noexcept;
    constexpr size_t index() const noexcept;

    // 20.7.2.6, swap
    void swap(variant&) noexcept(see below);
  };

  // 20.7.3, variant helper classes
  template <class T> struct variant_size; // undefined

  template <class T>
  inline constexpr size_t variant_size_v = variant_size<T>::value;

  template <class T> struct variant_size<const T>;
  template <class T> struct variant_size<volatile T>;
  template <class T> struct variant_size<const volatile T>;

  template <class... Types>
  struct variant_size<variant<Types...>>;

  template <size_t I, class T> struct variant_alternative; // undefined

  template <size_t I, class T>
  using variant_alternative_t = typename variant_alternative<I, T>::type;

  template <size_t I, class T> struct variant_alternative<I, const T>;
  template <size_t I, class T> struct variant_alternative<I, volatile T>;
  template <size_t I, class T> struct variant_alternative<I, const volatile T>;

  template <size_t I, class... Types>
  struct variant_alternative<I, variant<Types...>>;

  inline constexpr size_t variant_npos = -1;

  // 20.7.4, value access
  template <class T, class... Types>
  constexpr bool holds_alternative(const variant<Types...>&) noexcept;

  template <size_t I, class... Types>
  constexpr variant_alternative_t<I, variant<Types...>>&
  get(variant<Types...>&);

  template <size_t I, class... Types>
  constexpr variant_alternative_t<I, variant<Types...>>&&
  get(variant<Types...>&&);

  template <size_t I, class... Types>
  constexpr variant_alternative_t<I, variant<Types...>> const&
  get(const variant<Types...>&);

  template <size_t I, class... Types>
  constexpr variant_alternative_t<I, variant<Types...>> const&&
  get(const variant<Types...>&&);

  template <class T, class...  Types>
  constexpr T& get(variant<Types...>&);

  template <class T, class... Types>
  constexpr T&& get(variant<Types...>&&);

  template <class T, class... Types>
  constexpr const T& get(const variant<Types...>&);

  template <class T, class... Types>
  constexpr const T&& get(const variant<Types...>&&);

  template <size_t I, class... Types>
  constexpr add_pointer_t<variant_alternative_t<I, variant<Types...>>>
  get_if(variant<Types...>*) noexcept;

  template <size_t I, class... Types>
  constexpr add_pointer_t<const variant_alternative_t<I, variant<Types...>>>
  get_if(const variant<Types...>*) noexcept;

  template <class T, class... Types>
  constexpr add_pointer_t<T>
  get_if(variant<Types...>*) noexcept;

  template <class T, class... Types>
  constexpr add_pointer_t<const T>
  get_if(const variant<Types...>*) noexcept;

  // 20.7.5, relational operators
  template <class... Types>
  constexpr bool operator==(const variant<Types...>&, const variant<Types...>&);

  template <class... Types>
  constexpr bool operator!=(const variant<Types...>&, const variant<Types...>&);

  template <class... Types>
  constexpr bool operator<(const variant<Types...>&, const variant<Types...>&);

  template <class... Types>
  constexpr bool operator>(const variant<Types...>&, const variant<Types...>&);

  template <class... Types>
  constexpr bool operator<=(const variant<Types...>&, const variant<Types...>&);

  template <class... Types>
  constexpr bool operator>=(const variant<Types...>&, const variant<Types...>&);

  // 20.7.6, visitation
  template <class Visitor, class... Variants>
  constexpr see below visit(Visitor&&, Variants&&...);

  // 20.7.7, class monostate
  struct monostate;

  // 20.7.8, monostate relational operators
  constexpr bool operator<(monostate, monostate) noexcept;
  constexpr bool operator>(monostate, monostate) noexcept;
  constexpr bool operator<=(monostate, monostate) noexcept;
  constexpr bool operator>=(monostate, monostate) noexcept;
  constexpr bool operator==(monostate, monostate) noexcept;
  constexpr bool operator!=(monostate, monostate) noexcept;

  // 20.7.9, specialized algorithms
  template <class... Types>
  void swap(variant<Types...>&, variant<Types...>&) noexcept(see below);

  // 20.7.10, class bad_variant_access
  class bad_variant_access;

  // 20.7.11, hash support
  template <class T> struct hash;
  template <class... Types> struct hash<variant<Types...>>;
  template <> struct hash<monostate>;

} // namespace std

*/

//	std
#include <array>
#include <exception>
#include <functional>
#include <initializer_list>
#include <new>
#include <tuple>
#include <type_traits>
#include <utility>
#include <limits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulCompilerFlags.hpp>

#include <Lucena-Utilities/details/lulHelperTuple.hpp>
#include <Lucena-Utilities/details/lulHelperTypeTraits.hpp>
#include <Lucena-Utilities/details/lulHelperUtility.hpp>


//	purposefully avoiding versioned namespace
LUL_begin_namespace

namespace stdproxy {

class LUL_VIS_EXCEPTION_DEFINE bad_variant_access
	:	public std::exception {
public:
	virtual ~bad_variant_access() noexcept;
	virtual const char* what() const noexcept;
};

} // namespace stdproxy

LUL_end_namespace


LUL_begin_v_namespace

namespace stdproxy {

[[noreturn]]
inline LUL_VIS_INLINE
void __throw_bad_variant_access() {
        throw bad_variant_access();
}

template <class... _Types>
class LUL_VIS_TYPE_ONLY variant;

template <class _Tp>
struct LUL_VIS_TYPE_ONLY variant_size;

template <class _Tp>
inline constexpr size_t variant_size_v = variant_size<_Tp>::value;

template <class _Tp>
struct LUL_VIS_TYPE_DEFINE variant_size<const _Tp> : variant_size<_Tp> {};

template <class _Tp>
struct LUL_VIS_TYPE_DEFINE variant_size<volatile _Tp> : variant_size<_Tp> {};

template <class _Tp>
struct LUL_VIS_TYPE_DEFINE variant_size<const volatile _Tp>
    : variant_size<_Tp> {};

template <class... _Types>
struct LUL_VIS_TYPE_DEFINE variant_size<variant<_Types...>>
    : std::integral_constant<size_t, sizeof...(_Types)> {};

template <size_t _Ip, class _Tp>
struct LUL_VIS_TYPE_ONLY variant_alternative;

template <size_t _Ip, class _Tp>
using variant_alternative_t = typename variant_alternative<_Ip, _Tp>::type;

template <size_t _Ip, class _Tp>
struct LUL_VIS_TYPE_DEFINE variant_alternative<_Ip, const _Tp>
    : std::add_const<variant_alternative_t<_Ip, _Tp>> {};

template <size_t _Ip, class _Tp>
struct LUL_VIS_TYPE_DEFINE variant_alternative<_Ip, volatile _Tp>
    : std::add_volatile<variant_alternative_t<_Ip, _Tp>> {};

template <size_t _Ip, class _Tp>
struct LUL_VIS_TYPE_DEFINE variant_alternative<_Ip, const volatile _Tp>
    : std::add_cv<variant_alternative_t<_Ip, _Tp>> {};

template <size_t _Ip, class... _Types>
struct LUL_VIS_TYPE_DEFINE variant_alternative<_Ip, variant<_Types...>> {
  static_assert(_Ip < sizeof...(_Types), "Index out of bounds in std::variant_alternative<>");
  using type = __type_pack_element<_Ip, _Types...>;
};

inline constexpr size_t variant_npos = static_cast<size_t>(-1);

constexpr int __choose_index_type(unsigned int __num_elem) {
  if (__num_elem < std::numeric_limits<unsigned char>::max())
    return 0;
  if (__num_elem < std::numeric_limits<unsigned short>::max())
    return 1;
  return 2;
}

template <size_t _NumAlts>
using __variant_index_t =
  std::tuple_element_t<
      __choose_index_type(_NumAlts),
      std::tuple<unsigned char, unsigned short, unsigned int>
  >;

template <class _IndexType>
constexpr _IndexType __variant_npos = static_cast<_IndexType>(-1);

namespace __find_detail {

static constexpr size_t __not_found = -1;
static constexpr size_t __ambiguous = __not_found - 1;

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr size_t __find_index() {
  constexpr bool __matches[] = {std::is_same_v<_Tp, _Types>...};
  size_t __result = __not_found;
  for (size_t __i = 0; __i < sizeof...(_Types); ++__i) {
    if (__matches[__i]) {
      if (__result != __not_found) {
        return __ambiguous;
      }
      __result = __i;
    }
  }
  return __result;
}

template <size_t _Index>
struct __find_unambiguous_index_sfinae_impl
    : std::integral_constant<size_t, _Index> {};

template <>
struct __find_unambiguous_index_sfinae_impl<__not_found> {};

template <>
struct __find_unambiguous_index_sfinae_impl<__ambiguous> {};

template <class _Tp, class... _Types>
struct __find_unambiguous_index_sfinae
    : __find_unambiguous_index_sfinae_impl<__find_index<_Tp, _Types...>()> {};

} // namespace __find_detail

namespace __variant_detail {

struct __valueless_t {};

enum class _Trait { _TriviallyAvailable, _Available, _Unavailable };

template <typename _Tp,
          template <typename> class _IsTriviallyAvailable,
          template <typename> class _IsAvailable>
constexpr _Trait __trait =
    _IsTriviallyAvailable<_Tp>::value
        ? _Trait::_TriviallyAvailable
        : _IsAvailable<_Tp>::value ? _Trait::_Available : _Trait::_Unavailable;

inline LUL_VIS_INLINE
constexpr _Trait __common_trait(std::initializer_list<_Trait> __traits) {
  _Trait __result = _Trait::_TriviallyAvailable;
  for (_Trait __t : __traits) {
    if (static_cast<int>(__t) > static_cast<int>(__result)) {
      __result = __t;
    }
  }
  return __result;
}

template <typename... _Types>
struct __traits {
  static constexpr _Trait __copy_constructible_trait =
      __common_trait({__trait<_Types,
                              std::is_trivially_copy_constructible,
                              std::is_copy_constructible>...});

  static constexpr _Trait __move_constructible_trait =
      __common_trait({__trait<_Types,
                              std::is_trivially_move_constructible,
                              std::is_move_constructible>...});

  static constexpr _Trait __copy_assignable_trait = __common_trait(
      {__copy_constructible_trait,
       __trait<_Types, std::is_trivially_copy_assignable, std::is_copy_assignable>...});

  static constexpr _Trait __move_assignable_trait = __common_trait(
      {__move_constructible_trait,
       __trait<_Types, std::is_trivially_move_assignable, std::is_move_assignable>...});

  static constexpr _Trait __destructible_trait = __common_trait(
      {__trait<_Types, std::is_trivially_destructible, std::is_destructible>...});
};

namespace __access {

struct __union {
  template <class _Vp>
  inline LUL_VIS_INLINE
  static constexpr auto&& __get_alt(_Vp&& __v, std::in_place_index_t<0>) {
    return std::forward<_Vp>(__v).__head;
  }

  template <class _Vp, size_t _Ip>
  inline LUL_VIS_INLINE
  static constexpr auto&& __get_alt(_Vp&& __v, std::in_place_index_t<_Ip>) {
    return __get_alt(std::forward<_Vp>(__v).__tail, std::in_place_index<_Ip - 1>);
  }
};

struct __base {
  template <size_t _Ip, class _Vp>
  inline LUL_VIS_INLINE
  static constexpr auto&& __get_alt(_Vp&& __v) {
    return __union::__get_alt(std::forward<_Vp>(__v).__data,
                              std::in_place_index<_Ip>);
  }
};

struct __variant {
  template <size_t _Ip, class _Vp>
  inline LUL_VIS_INLINE
  static constexpr auto&& __get_alt(_Vp&& __v) {
    return __base::__get_alt<_Ip>(std::forward<_Vp>(__v).__impl);
  }
};

} // namespace __access

namespace __visitation {

struct __base {
  template <class _Visitor, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr decltype(auto)
  __visit_alt_at(size_t __index, _Visitor&& __visitor, _Vs&&... __vs) {
    constexpr auto __fdiagonal =
        __make_fdiagonal<_Visitor&&,
                         decltype(std::forward<_Vs>(__vs).__as_base())...>();
    return __fdiagonal[__index](std::forward<_Visitor>(__visitor),
                                std::forward<_Vs>(__vs).__as_base()...);
  }

  template <class _Visitor, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr decltype(auto) __visit_alt(_Visitor&& __visitor,
                                              _Vs&&... __vs) {
    constexpr auto __fmatrix =
        __make_fmatrix<_Visitor&&,
                       decltype(std::forward<_Vs>(__vs).__as_base())...>();
    return __at(__fmatrix, __vs.index()...)(
        std::forward<_Visitor>(__visitor),
        std::forward<_Vs>(__vs).__as_base()...);
  }

private:
  template <class _Tp>
  inline LUL_VIS_INLINE
  static constexpr const _Tp& __at(const _Tp& __elem) { return __elem; }

  template <class _Tp, size_t _Np, typename... _Indices>
  inline LUL_VIS_INLINE
  static constexpr auto&& __at(const std::array<_Tp, _Np>& __elems,
                               size_t __index, _Indices... __indices) {
    return __at(__elems[__index], __indices...);
  }

  template <class _Fp, class... _Fs>
  static constexpr void __std_visit_visitor_return_type_check() {
    static_assert(
        details::__all<std::is_same_v<_Fp, _Fs>...>::value,
        "`std::visit` requires the visitor to have a single return type.");
  }

  template <class... _Fs>
  inline LUL_VIS_INLINE
  static constexpr auto __make_farray(_Fs&&... __fs) {
    __std_visit_visitor_return_type_check<std::decay_t<_Fs>...>();
    using __result = std::array<std::common_type_t<std::decay_t<_Fs>...>, sizeof...(_Fs)>;
    return __result{{std::forward<_Fs>(__fs)...}};
  }

  template <std::size_t... _Is>
  struct __dispatcher {
    template <class _Fp, class... _Vs>
    inline LUL_VIS_INLINE
    static constexpr decltype(auto) __dispatch(_Fp __f, _Vs... __vs) {
        return __invoke_constexpr(
            static_cast<_Fp>(__f),
            __access::__base::__get_alt<_Is>(static_cast<_Vs>(__vs))...);
    }
  };

  template <class _Fp, class... _Vs, size_t... _Is>
  inline LUL_VIS_INLINE
  static constexpr auto __make_dispatch(std::index_sequence<_Is...>) {
    return __dispatcher<_Is...>::template __dispatch<_Fp, _Vs...>;
  }

  template <size_t _Ip, class _Fp, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr auto __make_fdiagonal_impl() {
    return __make_dispatch<_Fp, _Vs...>(
        std::index_sequence<(details::__identity<_Vs>{}, _Ip)...>{});
  }

  template <class _Fp, class... _Vs, size_t... _Is>
  inline LUL_VIS_INLINE
  static constexpr auto __make_fdiagonal_impl(std::index_sequence<_Is...>) {
    return __base::__make_farray(__make_fdiagonal_impl<_Is, _Fp, _Vs...>()...);
  }

  template <class _Fp, class _Vp, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr auto __make_fdiagonal() {
    constexpr size_t _Np = std::decay_t<_Vp>::__size();
    static_assert(details::__all<(_Np == std::decay_t<_Vs>::__size())...>::value);
    return __make_fdiagonal_impl<_Fp, _Vp, _Vs...>(std::make_index_sequence<_Np>{});
  }

  template <class _Fp, class... _Vs, size_t... _Is>
  inline LUL_VIS_INLINE
  static constexpr auto __make_fmatrix_impl(std::index_sequence<_Is...> __is) {
    return __make_dispatch<_Fp, _Vs...>(__is);
  }

  template <class _Fp, class... _Vs, size_t... _Is, size_t... _Js, class... _Ls>
  inline LUL_VIS_INLINE
  static constexpr auto __make_fmatrix_impl(std::index_sequence<_Is...>,
                                            std::index_sequence<_Js...>,
                                            _Ls... __ls) {
    return __base::__make_farray(__make_fmatrix_impl<_Fp, _Vs...>(
        std::index_sequence<_Is..., _Js>{}, __ls...)...);
  }

  template <class _Fp, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr auto __make_fmatrix() {
    return __make_fmatrix_impl<_Fp, _Vs...>(
        std::index_sequence<>{}, std::make_index_sequence<std::decay_t<_Vs>::__size()>{}...);
  }
};

struct __variant {
  template <class _Visitor, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr decltype(auto)
  __visit_alt_at(size_t __index, _Visitor&& __visitor, _Vs&&... __vs) {
    return __base::__visit_alt_at(__index,
                                  std::forward<_Visitor>(__visitor),
                                  std::forward<_Vs>(__vs).__impl...);
  }

  template <class _Visitor, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr decltype(auto) __visit_alt(_Visitor&& __visitor,
                                              _Vs&&... __vs) {
    return __base::__visit_alt(std::forward<_Visitor>(__visitor),
                               std::forward<_Vs>(__vs).__impl...);
  }

  template <class _Visitor, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr decltype(auto)
  __visit_value_at(size_t __index, _Visitor&& __visitor, _Vs&&... __vs) {
    return __visit_alt_at(
        __index,
        __make_value_visitor(std::forward<_Visitor>(__visitor)),
        std::forward<_Vs>(__vs)...);
  }

  template <class _Visitor, class... _Vs>
  inline LUL_VIS_INLINE
  static constexpr decltype(auto) __visit_value(_Visitor&& __visitor,
                                                _Vs&&... __vs) {
    return __visit_alt(
        __make_value_visitor(std::forward<_Visitor>(__visitor)),
        std::forward<_Vs>(__vs)...);
  }

private:
  template <class _Visitor, class... _Values>
  static constexpr void __std_visit_exhaustive_visitor_check() {
    //	FIXME This will fail under Xcode 10+ as is_callable has been replaced
    //	with the correct is_invocable.
    static_assert(std::is_callable_v<_Visitor, _Values...>,
                  "`std::visit` requires the visitor to be exhaustive.");
  }

  template <class _Visitor>
  struct __value_visitor {
    template <class... _Alts>
    inline LUL_VIS_INLINE
    constexpr decltype(auto) operator()(_Alts&&... __alts) const {
      __std_visit_exhaustive_visitor_check<
          _Visitor,
          decltype((std::forward<_Alts>(__alts).__value))...>();
      return __invoke_constexpr(std::forward<_Visitor>(__visitor),
                                std::forward<_Alts>(__alts).__value...);
    }
    _Visitor&& __visitor;
  };

  template <class _Visitor>
  inline LUL_VIS_INLINE
  static constexpr auto __make_value_visitor(_Visitor&& __visitor) {
    return __value_visitor<_Visitor>{std::forward<_Visitor>(__visitor)};
  }
};

} // namespace __visitation

template <size_t _Index, class _Tp>
struct LUL_VIS_TYPE_DEFINE __alt {
  using __value_type = _Tp;

  template <class... _Args>
  inline LUL_VIS_INLINE
  explicit constexpr __alt(std::in_place_t, _Args&&... __args)
      : __value(std::forward<_Args>(__args)...) {}

  __value_type __value;
};

template <_Trait _DestructibleTrait, size_t _Index, class... _Types>
union LUL_VIS_TYPE_ONLY __union;

template <_Trait _DestructibleTrait, size_t _Index>
union LUL_VIS_TYPE_DEFINE __union<_DestructibleTrait, _Index> {};

#define LUL_VARIANT_UNION(destructible_trait, destructor)                      \
  template <size_t _Index, class _Tp, class... _Types>                         \
  union LUL_VIS_TYPE_DEFINE __union<destructible_trait,                        \
                                      _Index,                                  \
                                      _Tp,                                     \
                                      _Types...> {                             \
  public:                                                                      \
    inline LUL_VIS_INLINE                                                      \
    explicit constexpr __union(__valueless_t) noexcept : __dummy{} {}          \
                                                                               \
    template <class... _Args>                                                  \
    inline LUL_VIS_INLINE                                                      \
    explicit constexpr __union(std::in_place_index_t<0>, _Args&&... __args)    \
        : __head(std::in_place, std::forward<_Args>(__args)...) {}             \
                                                                               \
    template <size_t _Ip, class... _Args>                                      \
    inline LUL_VIS_INLINE                                                      \
    explicit constexpr __union(std::in_place_index_t<_Ip>, _Args&&... __args)  \
        : __tail(std::in_place_index<_Ip - 1>, std::forward<_Args>(__args)...) {}   \
                                                                               \
    __union(const __union&) = default;                                         \
    __union(__union&&) = default;                                              \
                                                                               \
    destructor                                                                 \
                                                                               \
    __union& operator=(const __union&) = default;                              \
    __union& operator=(__union&&) = default;                                   \
                                                                               \
  private:                                                                     \
    char __dummy;                                                              \
    __alt<_Index, _Tp> __head;                                                 \
    __union<destructible_trait, _Index + 1, _Types...> __tail;                 \
                                                                               \
    friend struct __access::__union;                                           \
  }

LUL_VARIANT_UNION(_Trait::_TriviallyAvailable, ~__union() = default;);
LUL_VARIANT_UNION(_Trait::_Available, ~__union() {});
LUL_VARIANT_UNION(_Trait::_Unavailable, ~__union() = delete;);

#undef LUL_VARIANT_UNION

template <_Trait _DestructibleTrait, class... _Types>
class LUL_VIS_TYPE_DEFINE __base {
public:
  using __index_t = __variant_index_t<sizeof...(_Types)>;

  inline LUL_VIS_INLINE
  explicit constexpr __base(__valueless_t tag) noexcept
      : __data(tag), __index(__variant_npos<__index_t>) {}

  template <size_t _Ip, class... _Args>
  inline LUL_VIS_INLINE
  explicit constexpr __base(std::in_place_index_t<_Ip>, _Args&&... __args)
      :
        __data(std::in_place_index<_Ip>, std::forward<_Args>(__args)...),
        __index(_Ip) {}

  inline LUL_VIS_INLINE
  constexpr bool valueless_by_exception() const noexcept {
    return index() == variant_npos;
  }

  inline LUL_VIS_INLINE
  constexpr size_t index() const noexcept {
    return __index == __variant_npos<__index_t> ? variant_npos : __index;
  }

protected:
  inline LUL_VIS_INLINE
  constexpr auto&& __as_base() & { return *this; }

  inline LUL_VIS_INLINE
  constexpr auto&& __as_base() && { return std::move(*this); }

  inline LUL_VIS_INLINE
  constexpr auto&& __as_base() const & { return *this; }

  inline LUL_VIS_INLINE
  constexpr auto&& __as_base() const && { return std::move(*this); }

  inline LUL_VIS_INLINE
  static constexpr size_t __size() { return sizeof...(_Types); }

  __union<_DestructibleTrait, 0, _Types...> __data;
  __index_t __index;

  friend struct __access::__base;
  friend struct __visitation::__base;
};

template <class _Traits, _Trait = _Traits::__destructible_trait>
class LUL_VIS_TYPE_ONLY __destructor;

#define LUL_VARIANT_DESTRUCTOR(destructible_trait, destructor, destroy)        \
  template <class... _Types>                                                   \
  class LUL_VIS_TYPE_DEFINE __destructor<__traits<_Types...>,                  \
                                           destructible_trait>                 \
      : public __base<destructible_trait, _Types...> {                         \
    using __base_type = __base<destructible_trait, _Types...>;                 \
    using __index_t = typename __base_type::__index_t;                         \
                                                                               \
  public:                                                                      \
    using __base_type::__base_type;                                            \
    using __base_type::operator=;                                              \
                                                                               \
    __destructor(const __destructor&) = default;                               \
    __destructor(__destructor&&) = default;                                    \
    destructor                                                                 \
    __destructor& operator=(const __destructor&) = default;                    \
    __destructor& operator=(__destructor&&) = default;                         \
                                                                               \
  protected:                                                                   \
    inline LUL_VIS_INLINE                                                      \
    destroy                                                                    \
  }

LUL_VARIANT_DESTRUCTOR(
    _Trait::_TriviallyAvailable,
    ~__destructor() = default;,
    void __destroy() noexcept { this->__index = __variant_npos<__index_t>; });

LUL_VARIANT_DESTRUCTOR(
    _Trait::_Available,
    ~__destructor() { __destroy(); },
    void __destroy() noexcept {
      if (!this->valueless_by_exception()) {
        __visitation::__base::__visit_alt(
            [](auto& __alt) noexcept {
              using __alt_type = std::decay_t<decltype(__alt)>;
              __alt.~__alt_type();
            },
            *this);
      }
      this->__index = __variant_npos<__index_t>;
    });

LUL_VARIANT_DESTRUCTOR(
    _Trait::_Unavailable,
    ~__destructor() = delete;,
    void __destroy() noexcept = delete;);

#undef LUL_VARIANT_DESTRUCTOR

template <class _Traits>
class LUL_VIS_TYPE_DEFINE __constructor : public __destructor<_Traits> {
  using __base_type = __destructor<_Traits>;

public:
  using __base_type::__base_type;
  using __base_type::operator=;

protected:
  template <size_t _Ip, class _Tp, class... _Args>
  inline LUL_VIS_INLINE
  static _Tp& __construct_alt(__alt<_Ip, _Tp>& __a, _Args&&... __args) {
    ::new ((void*)std::addressof(__a))
        __alt<_Ip, _Tp>(std::in_place, std::forward<_Args>(__args)...);
    return __a.__value;
  }

  template <class _Rhs>
  inline LUL_VIS_INLINE
  static void __generic_construct(__constructor& __lhs, _Rhs&& __rhs) {
    __lhs.__destroy();
    if (!__rhs.valueless_by_exception()) {
      __visitation::__base::__visit_alt_at(
          __rhs.index(),
          [](auto& __lhs_alt, auto&& __rhs_alt) {
            __construct_alt(
                __lhs_alt,
                std::forward<decltype(__rhs_alt)>(__rhs_alt).__value);
          },
          __lhs, std::forward<_Rhs>(__rhs));
      __lhs.__index = __rhs.index();
    }
  }
};

template <class _Traits, _Trait = _Traits::__move_constructible_trait>
class LUL_VIS_TYPE_ONLY __move_constructor;

#define LUL_VARIANT_MOVE_CONSTRUCTOR(move_constructible_trait,                 \
                                         move_constructor)                     \
  template <class... _Types>                                                   \
  class LUL_VIS_TYPE_DEFINE __move_constructor<__traits<_Types...>,            \
                                                 move_constructible_trait>     \
      : public __constructor<__traits<_Types...>> {                            \
    using __base_type = __constructor<__traits<_Types...>>;                    \
                                                                               \
  public:                                                                      \
    using __base_type::__base_type;                                            \
    using __base_type::operator=;                                              \
                                                                               \
    __move_constructor(const __move_constructor&) = default;                   \
    move_constructor                                                           \
    ~__move_constructor() = default;                                           \
    __move_constructor& operator=(const __move_constructor&) = default;        \
    __move_constructor& operator=(__move_constructor&&) = default;             \
  }

LUL_VARIANT_MOVE_CONSTRUCTOR(
    _Trait::_TriviallyAvailable,
    __move_constructor(__move_constructor&& __that) = default;);

LUL_VARIANT_MOVE_CONSTRUCTOR(
    _Trait::_Available,
    __move_constructor(__move_constructor&& __that) noexcept(
        details::__all<std::is_nothrow_move_constructible_v<_Types>...>::value)
        : __move_constructor(__valueless_t{}) {
      this->__generic_construct(*this, std::move(__that));
    });

LUL_VARIANT_MOVE_CONSTRUCTOR(
    _Trait::_Unavailable,
    __move_constructor(__move_constructor&&) = delete;);

#undef LUL_VARIANT_MOVE_CONSTRUCTOR

template <class _Traits, _Trait = _Traits::__copy_constructible_trait>
class LUL_VIS_TYPE_ONLY __copy_constructor;

#define LUL_VARIANT_COPY_CONSTRUCTOR(copy_constructible_trait,                 \
                                         copy_constructor)                     \
  template <class... _Types>                                                   \
  class LUL_VIS_TYPE_DEFINE __copy_constructor<__traits<_Types...>,            \
                                                 copy_constructible_trait>     \
      : public __move_constructor<__traits<_Types...>> {                       \
    using __base_type = __move_constructor<__traits<_Types...>>;               \
                                                                               \
  public:                                                                      \
    using __base_type::__base_type;                                            \
    using __base_type::operator=;                                              \
                                                                               \
    copy_constructor                                                           \
    __copy_constructor(__copy_constructor&&) = default;                        \
    ~__copy_constructor() = default;                                           \
    __copy_constructor& operator=(const __copy_constructor&) = default;        \
    __copy_constructor& operator=(__copy_constructor&&) = default;             \
  }

LUL_VARIANT_COPY_CONSTRUCTOR(
    _Trait::_TriviallyAvailable,
    __copy_constructor(const __copy_constructor& __that) = default;);

LUL_VARIANT_COPY_CONSTRUCTOR(
    _Trait::_Available,
    __copy_constructor(const __copy_constructor& __that)
        : __copy_constructor(__valueless_t{}) {
      this->__generic_construct(*this, __that);
    });

LUL_VARIANT_COPY_CONSTRUCTOR(
    _Trait::_Unavailable,
    __copy_constructor(const __copy_constructor&) = delete;);

#undef LUL_VARIANT_COPY_CONSTRUCTOR

template <class _Traits>
class LUL_VIS_TYPE_DEFINE __assignment : public __copy_constructor<_Traits> {
  using __base_type = __copy_constructor<_Traits>;

public:
  using __base_type::__base_type;
  using __base_type::operator=;

  template <size_t _Ip, class... _Args>
  inline LUL_VIS_INLINE
  auto& __emplace(_Args&&... __args) {
    this->__destroy();
    auto& __res = this->__construct_alt(__access::__base::__get_alt<_Ip>(*this),
                          std::forward<_Args>(__args)...);
    this->__index = _Ip;
    return __res;
  }

protected:
  template <size_t _Ip, class _Tp, class _Arg>
  inline LUL_VIS_INLINE
  void __assign_alt(__alt<_Ip, _Tp>& __a, _Arg&& __arg) {
    if (this->index() == _Ip) {
      __a.__value = std::forward<_Arg>(__arg);
    } else {
      struct {
        void operator()(std::true_type) const {
          __this->__emplace<_Ip>(std::forward<_Arg>(__arg));
        }
        void operator()(std::false_type) const {
          __this->__emplace<_Ip>(_Tp(std::forward<_Arg>(__arg)));
        }
        __assignment* __this;
        _Arg&& __arg;
      } __impl{this, std::forward<_Arg>(__arg)};
      __impl(std::bool_constant<std::is_nothrow_constructible_v<_Tp, _Arg> ||
                           !std::is_nothrow_move_constructible_v<_Tp>>{});
    }
  }

  template <class _That>
  inline LUL_VIS_INLINE
  void __generic_assign(_That&& __that) {
    if (this->valueless_by_exception() && __that.valueless_by_exception()) {
      // do nothing.
    } else if (__that.valueless_by_exception()) {
      this->__destroy();
    } else {
      __visitation::__base::__visit_alt_at(
          __that.index(),
          [this](auto& __this_alt, auto&& __that_alt) {
            this->__assign_alt(
                __this_alt,
                std::forward<decltype(__that_alt)>(__that_alt).__value);
          },
          *this, std::forward<_That>(__that));
    }
  }
};

template <class _Traits, _Trait = _Traits::__move_assignable_trait>
class LUL_VIS_TYPE_ONLY __move_assignment;

#define LUL_VARIANT_MOVE_ASSIGNMENT(move_assignable_trait,                     \
                                        move_assignment)                       \
  template <class... _Types>                                                   \
  class LUL_VIS_TYPE_DEFINE __move_assignment<__traits<_Types...>,             \
                                                move_assignable_trait>         \
      : public __assignment<__traits<_Types...>> {                             \
    using __base_type = __assignment<__traits<_Types...>>;                     \
                                                                               \
  public:                                                                      \
    using __base_type::__base_type;                                            \
    using __base_type::operator=;                                              \
                                                                               \
    __move_assignment(const __move_assignment&) = default;                     \
    __move_assignment(__move_assignment&&) = default;                          \
    ~__move_assignment() = default;                                            \
    __move_assignment& operator=(const __move_assignment&) = default;          \
    move_assignment                                                            \
  }

LUL_VARIANT_MOVE_ASSIGNMENT(
    _Trait::_TriviallyAvailable,
    __move_assignment& operator=(__move_assignment&& __that) = default;);

LUL_VARIANT_MOVE_ASSIGNMENT(
    _Trait::_Available,
    __move_assignment& operator=(__move_assignment&& __that) noexcept(
        details::__all<(std::is_nothrow_move_constructible_v<_Types> &&
               std::is_nothrow_move_assignable_v<_Types>)...>::value) {
      this->__generic_assign(std::move(__that));
      return *this;
    });

LUL_VARIANT_MOVE_ASSIGNMENT(
    _Trait::_Unavailable,
    __move_assignment& operator=(__move_assignment&&) = delete;);

#undef LUL_VARIANT_MOVE_ASSIGNMENT

template <class _Traits, _Trait = _Traits::__copy_assignable_trait>
class LUL_VIS_TYPE_ONLY __copy_assignment;

#define LUL_VARIANT_COPY_ASSIGNMENT(copy_assignable_trait,                     \
                                        copy_assignment)                       \
  template <class... _Types>                                                   \
  class LUL_VIS_TYPE_DEFINE __copy_assignment<__traits<_Types...>,             \
                                                copy_assignable_trait>         \
      : public __move_assignment<__traits<_Types...>> {                        \
    using __base_type = __move_assignment<__traits<_Types...>>;                \
                                                                               \
  public:                                                                      \
    using __base_type::__base_type;                                            \
    using __base_type::operator=;                                              \
                                                                               \
    __copy_assignment(const __copy_assignment&) = default;                     \
    __copy_assignment(__copy_assignment&&) = default;                          \
    ~__copy_assignment() = default;                                            \
    copy_assignment                                                            \
    __copy_assignment& operator=(__copy_assignment&&) = default;               \
  }

LUL_VARIANT_COPY_ASSIGNMENT(
    _Trait::_TriviallyAvailable,
    __copy_assignment& operator=(const __copy_assignment& __that) = default;);

LUL_VARIANT_COPY_ASSIGNMENT(
    _Trait::_Available,
    __copy_assignment& operator=(const __copy_assignment& __that) {
      this->__generic_assign(__that);
      return *this;
    });

LUL_VARIANT_COPY_ASSIGNMENT(
    _Trait::_Unavailable,
    __copy_assignment& operator=(const __copy_assignment&) = delete;);

#undef LUL_VARIANT_COPY_ASSIGNMENT

template <class... _Types>
class LUL_VIS_TYPE_DEFINE __impl
    : public __copy_assignment<__traits<_Types...>> {
  using __base_type = __copy_assignment<__traits<_Types...>>;

public:
  using __base_type::__base_type;
  using __base_type::operator=;

  template <size_t _Ip, class _Arg>
  inline LUL_VIS_INLINE
  void __assign(_Arg&& __arg) {
    this->__assign_alt(__access::__base::__get_alt<_Ip>(*this),
                       std::forward<_Arg>(__arg));
  }

  inline LUL_VIS_INLINE
  void __swap(__impl& __that)  {
    if (this->valueless_by_exception() && __that.valueless_by_exception()) {
      // do nothing.
    } else if (this->index() == __that.index()) {
      __visitation::__base::__visit_alt_at(
          this->index(),
          [](auto& __this_alt, auto& __that_alt) {
            using std::swap;
            swap(__this_alt.__value, __that_alt.__value);
          },
          *this,
          __that);
    } else {
      __impl* __lhs = this;
      __impl* __rhs = std::addressof(__that);
      if (__lhs->__move_nothrow() && !__rhs->__move_nothrow()) {
        std::swap(__lhs, __rhs);
      }
      __impl __tmp(std::move(*__rhs));
      // EXTENSION: When the move construction of `__lhs` into `__rhs` throws
      // and `__tmp` is nothrow move constructible then we move `__tmp` back
      // into `__rhs` and provide the strong exception safety guarentee.
      try {
        this->__generic_construct(*__rhs, std::move(*__lhs));
      } catch (...) {
        if (__tmp.__move_nothrow()) {
          this->__generic_construct(*__rhs, std::move(__tmp));
        }
        throw;
      }
      this->__generic_construct(*__lhs, std::move(__tmp));
    }
  }

private:
  inline LUL_VIS_INLINE
  bool __move_nothrow() const {
    constexpr bool __results[] = {std::is_nothrow_move_constructible_v<_Types>...};
    return this->valueless_by_exception() || __results[this->index()];
  }
};

template <class... _Types>
struct __overload;

template <>
struct __overload<> { void operator()() const; };

template <class _Tp, class... _Types>
struct __overload<_Tp, _Types...> : __overload<_Types...> {
  using __overload<_Types...>::operator();
  details::__identity<_Tp> operator()(_Tp) const;
};

template <class _Tp, class... _Types>
using __best_match_t = typename std::result_of_t<__overload<_Types...>(_Tp&&)>::type;

} // __variant_detail

template <class... _Types>
class LUL_VIS_TYPE_DEFINE variant
    : private details::__sfinae_ctor_base<
          details::__all<std::is_copy_constructible_v<_Types>...>::value,
           details::__all<std::is_move_constructible_v<_Types>...>::value>,
      private details::__sfinae_assign_base<
           details::__all<(std::is_copy_constructible_v<_Types> &&
                 std::is_copy_assignable_v<_Types>)...>::value,
           details::__all<(std::is_move_constructible_v<_Types> &&
                 std::is_move_assignable_v<_Types>)...>::value> {
  static_assert(0 < sizeof...(_Types),
                "variant must consist of at least one alternative.");

  static_assert( details::__all<!std::is_array_v<_Types>...>::value,
                "variant can not have an array type as an alternative.");

  static_assert( details::__all<!std::is_reference_v<_Types>...>::value,
                "variant can not have a reference type as an alternative.");

  static_assert( details::__all<!std::is_void_v<_Types>...>::value,
                "variant can not have a void type as an alternative.");

  using __first_type = variant_alternative_t<0, variant>;

public:
  template <bool _Dummy = true,
            std::enable_if_t<details::__dependent_type<std::is_default_constructible<__first_type>,
                                         _Dummy>::value,
                        int> = 0>
  inline LUL_VIS_INLINE
  constexpr variant() noexcept(std::is_nothrow_default_constructible_v<__first_type>)
      : __impl(std::in_place_index<0>) {}

  variant(const variant&) = default;
  variant(variant&&) = default;

  template <
      class _Arg,
      std::enable_if_t<!std::is_same_v<std::decay_t<_Arg>, variant>, int> = 0,
      std::enable_if_t<!details::__is_inplace_type<std::decay_t<_Arg>>::value, int> = 0,
      std::enable_if_t<!details::__is_inplace_index<std::decay_t<_Arg>>::value, int> = 0,
      class _Tp = __variant_detail::__best_match_t<_Arg, _Types...>,
      size_t _Ip =
          __find_detail::__find_unambiguous_index_sfinae<_Tp, _Types...>::value,
      std::enable_if_t<std::is_constructible_v<_Tp, _Arg>, int> = 0>
  inline LUL_VIS_INLINE
  constexpr variant(_Arg&& __arg) noexcept(
      std::is_nothrow_constructible_v<_Tp, _Arg>)
      : __impl(std::in_place_index<_Ip>, std::forward<_Arg>(__arg)) {}

  template <size_t _Ip, class... _Args,
            class = std::enable_if_t<(_Ip < sizeof...(_Types)), int>,
            class _Tp = variant_alternative_t<_Ip, variant<_Types...>>,
            std::enable_if_t<std::is_constructible_v<_Tp, _Args...>, int> = 0>
  inline LUL_VIS_INLINE
  explicit constexpr variant(
      std::in_place_index_t<_Ip>,
      _Args&&... __args) noexcept(std::is_nothrow_constructible_v<_Tp, _Args...>)
      : __impl(std::in_place_index<_Ip>, std::forward<_Args>(__args)...) {}

  template <
      size_t _Ip,
      class _Up,
      class... _Args,
      std::enable_if_t<(_Ip < sizeof...(_Types)), int> = 0,
      class _Tp = variant_alternative_t<_Ip, variant<_Types...>>,
      std::enable_if_t<std::is_constructible_v<_Tp, std::initializer_list<_Up>&, _Args...>,
                  int> = 0>
  inline LUL_VIS_INLINE
  explicit constexpr variant(
      std::in_place_index_t<_Ip>,
      std::initializer_list<_Up> __il,
      _Args&&... __args) noexcept(
      std::is_nothrow_constructible_v<_Tp, std::initializer_list<_Up>&, _Args...>)
      : __impl(std::in_place_index<_Ip>, __il, std::forward<_Args>(__args)...) {}

  template <
      class _Tp,
      class... _Args,
      size_t _Ip =
          __find_detail::__find_unambiguous_index_sfinae<_Tp, _Types...>::value,
      std::enable_if_t<std::is_constructible_v<_Tp, _Args...>, int> = 0>
  inline LUL_VIS_INLINE
  explicit constexpr variant(std::in_place_type_t<_Tp>, _Args&&... __args) noexcept(
      std::is_nothrow_constructible_v<_Tp, _Args...>)
      : __impl(std::in_place_index<_Ip>, std::forward<_Args>(__args)...) {}

  template <
      class _Tp,
      class _Up,
      class... _Args,
      size_t _Ip =
          __find_detail::__find_unambiguous_index_sfinae<_Tp, _Types...>::value,
      std::enable_if_t<std::is_constructible_v<_Tp, std::initializer_list<_Up>&, _Args...>,
                  int> = 0>
  inline LUL_VIS_INLINE
  explicit constexpr variant(
      std::in_place_type_t<_Tp>,
      std::initializer_list<_Up> __il,
      _Args&&... __args) noexcept(
      std::is_nothrow_constructible_v<_Tp, std::initializer_list< _Up>&, _Args...>)
      : __impl(std::in_place_index<_Ip>, __il, std::forward<_Args>(__args)...) {}

  ~variant() = default;

  variant& operator=(const variant&) = default;
  variant& operator=(variant&&) = default;

  template <
      class _Arg,
      std::enable_if_t<!std::is_same_v<std::decay_t<_Arg>, variant>, int> = 0,
      class _Tp = __variant_detail::__best_match_t<_Arg, _Types...>,
      size_t _Ip =
          __find_detail::__find_unambiguous_index_sfinae<_Tp, _Types...>::value,
      std::enable_if_t<std::is_assignable_v<_Tp&, _Arg> && std::is_constructible_v<_Tp, _Arg>,
                  int> = 0>
  inline LUL_VIS_INLINE
  variant& operator=(_Arg&& __arg) noexcept(
      std::is_nothrow_assignable_v<_Tp&, _Arg> &&
      std::is_nothrow_constructible_v<_Tp, _Arg>) {
    __impl.template __assign<_Ip>(std::forward<_Arg>(__arg));
    return *this;
  }

  template <
      size_t _Ip,
      class... _Args,
      std::enable_if_t<(_Ip < sizeof...(_Types)), int> = 0,
      class _Tp = variant_alternative_t<_Ip, variant<_Types...>>,
      std::enable_if_t<std::is_constructible_v<_Tp, _Args...>, int> = 0>
  inline LUL_VIS_INLINE
  _Tp& emplace(_Args&&... __args) {
    return __impl.template __emplace<_Ip>(std::forward<_Args>(__args)...);
  }

  template <
      size_t _Ip,
      class _Up,
      class... _Args,
      std::enable_if_t<(_Ip < sizeof...(_Types)), int> = 0,
      class _Tp = variant_alternative_t<_Ip, variant<_Types...>>,
      std::enable_if_t<std::is_constructible_v<_Tp, std::initializer_list<_Up>&, _Args...>,
                  int> = 0>
  inline LUL_VIS_INLINE
  _Tp& emplace(std::initializer_list<_Up> __il, _Args&&... __args) {
    return __impl.template __emplace<_Ip>(__il, std::forward<_Args>(__args)...);
  }

  template <
      class _Tp,
      class... _Args,
      size_t _Ip =
          __find_detail::__find_unambiguous_index_sfinae<_Tp, _Types...>::value,
      std::enable_if_t<std::is_constructible_v<_Tp, _Args...>, int> = 0>
  inline LUL_VIS_INLINE
  _Tp& emplace(_Args&&... __args) {
    return __impl.template __emplace<_Ip>(std::forward<_Args>(__args)...);
  }

  template <
      class _Tp,
      class _Up,
      class... _Args,
      size_t _Ip =
          __find_detail::__find_unambiguous_index_sfinae<_Tp, _Types...>::value,
      std::enable_if_t<std::is_constructible_v<_Tp, std::initializer_list<_Up>&, _Args...>,
                  int> = 0>
  inline LUL_VIS_INLINE
  _Tp& emplace(std::initializer_list<_Up> __il, _Args&&... __args) {
    return __impl.template __emplace<_Ip>(__il, std::forward<_Args>(__args)...);
  }

  inline LUL_VIS_INLINE
  constexpr bool valueless_by_exception() const noexcept {
    return __impl.valueless_by_exception();
  }

  inline LUL_VIS_INLINE
  constexpr size_t index() const noexcept { return __impl.index(); }

  template <
      bool _Dummy = true,
      std::enable_if_t<
          details::__all<(
              details::__dependent_type<std::is_move_constructible<_Types>, _Dummy>::value &&
              details::__dependent_type<std::is_swappable<_Types>, _Dummy>::value)...>::value,
          int> = 0>
  inline LUL_VIS_INLINE
  void swap(variant& __that) noexcept(
      details::__all<(std::is_nothrow_move_constructible_v<_Types> &&
             std::is_nothrow_swappable_v<_Types>)...>::value) {
    __impl.__swap(__that.__impl);
  }

private:
  __variant_detail::__impl<_Types...> __impl;

  friend struct __variant_detail::__access::__variant;
  friend struct __variant_detail::__visitation::__variant;
};

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr bool __holds_alternative(const variant<_Types...>& __v) noexcept {
  return __v.index() == _Ip;
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr bool holds_alternative(const variant<_Types...>& __v) noexcept {
  return __holds_alternative<details::__find_exactly_one_t<_Tp, _Types...>::value>(__v);
}

template <size_t _Ip, class _Vp>
inline LUL_VIS_INLINE
static constexpr auto&& __generic_get(_Vp&& __v) {
  using __variant_detail::__access::__variant;
  if (!__holds_alternative<_Ip>(__v)) {
    __throw_bad_variant_access();
  }
  return __variant::__get_alt<_Ip>(std::forward<_Vp>(__v)).__value;
}

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr variant_alternative_t<_Ip, variant<_Types...>>& get(
    variant<_Types...>& __v) {
  static_assert(_Ip < sizeof...(_Types));
  static_assert(!std::is_void_v<variant_alternative_t<_Ip, variant<_Types...>>>);
  return __generic_get<_Ip>(__v);
}

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr variant_alternative_t<_Ip, variant<_Types...>>&& get(
    variant<_Types...>&& __v) {
  static_assert(_Ip < sizeof...(_Types));
  static_assert(!std::is_void_v<variant_alternative_t<_Ip, variant<_Types...>>>);
  return __generic_get<_Ip>(std::move(__v));
}

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr const variant_alternative_t<_Ip, variant<_Types...>>& get(
    const variant<_Types...>& __v) {
  static_assert(_Ip < sizeof...(_Types));
  static_assert(!std::is_void_v<variant_alternative_t<_Ip, variant<_Types...>>>);
  return __generic_get<_Ip>(__v);
}

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr const variant_alternative_t<_Ip, variant<_Types...>>&& get(
    const variant<_Types...>&& __v) {
  static_assert(_Ip < sizeof...(_Types));
  static_assert(!std::is_void_v<variant_alternative_t<_Ip, variant<_Types...>>>);
  return __generic_get<_Ip>(std::move(__v));
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr _Tp& get(variant<_Types...>& __v) {
  static_assert(!std::is_void_v<_Tp>);
  return std::get<details::__find_exactly_one_t<_Tp, _Types...>::value>(__v);
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr _Tp&& get(variant<_Types...>&& __v) {
  static_assert(!std::is_void_v<_Tp>);
  return std::get<details::__find_exactly_one_t<_Tp, _Types...>::value>(
      std::move(__v));
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr const _Tp& get(const variant<_Types...>& __v) {
  static_assert(!std::is_void_v<_Tp>);
  return std::get<details::__find_exactly_one_t<_Tp, _Types...>::value>(__v);
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr const _Tp&& get(const variant<_Types...>&& __v) {
  static_assert(!std::is_void_v<_Tp>);
  return std::get<details::__find_exactly_one_t<_Tp, _Types...>::value>(
      std::move(__v));
}

template <size_t _Ip, class _Vp>
inline LUL_VIS_INLINE
constexpr auto* __generic_get_if(_Vp* __v) noexcept {
  using __variant_detail::__access::__variant;
  return __v && __holds_alternative<_Ip>(*__v)
             ? std::addressof(__variant::__get_alt<_Ip>(*__v).__value)
             : nullptr;
}

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr std::add_pointer_t<variant_alternative_t<_Ip, variant<_Types...>>>
get_if(variant<_Types...>* __v) noexcept {
  static_assert(_Ip < sizeof...(_Types));
  static_assert(!std::is_void_v<variant_alternative_t<_Ip, variant<_Types...>>>);
  return __generic_get_if<_Ip>(__v);
}

template <size_t _Ip, class... _Types>
inline LUL_VIS_INLINE
constexpr std::add_pointer_t<const variant_alternative_t<_Ip, variant<_Types...>>>
get_if(const variant<_Types...>* __v) noexcept {
  static_assert(_Ip < sizeof...(_Types));
  static_assert(!std::is_void_v<variant_alternative_t<_Ip, variant<_Types...>>>);
  return __generic_get_if<_Ip>(__v);
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr std::add_pointer_t<_Tp>
get_if(variant<_Types...>* __v) noexcept {
  static_assert(!std::is_void_v<_Tp>);
  return get_if<details::__find_exactly_one_t<_Tp, _Types...>::value>(__v);
}

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE
constexpr std::add_pointer_t<const _Tp>
get_if(const variant<_Types...>* __v) noexcept {
  static_assert(!std::is_void_v<_Tp>);
  return get_if<details::__find_exactly_one_t<_Tp, _Types...>::value>(__v);
}

template <class... _Types>
inline LUL_VIS_INLINE
constexpr bool operator==(const variant<_Types...>& __lhs,
                          const variant<_Types...>& __rhs) {
  using __variant_detail::__visitation::__variant;
  if (__lhs.index() != __rhs.index()) return false;
  if (__lhs.valueless_by_exception()) return true;
  return __variant::__visit_value_at(__lhs.index(), std::equal_to<>{}, __lhs, __rhs);
}

template <class... _Types>
inline LUL_VIS_INLINE
constexpr bool operator!=(const variant<_Types...>& __lhs,
                          const variant<_Types...>& __rhs) {
  using __variant_detail::__visitation::__variant;
  if (__lhs.index() != __rhs.index()) return true;
  if (__lhs.valueless_by_exception()) return false;
  return __variant::__visit_value_at(
      __lhs.index(), std::not_equal_to<>{}, __lhs, __rhs);
}

template <class... _Types>
inline LUL_VIS_INLINE
constexpr bool operator<(const variant<_Types...>& __lhs,
                         const variant<_Types...>& __rhs) {
  using __variant_detail::__visitation::__variant;
  if (__rhs.valueless_by_exception()) return false;
  if (__lhs.valueless_by_exception()) return true;
  if (__lhs.index() < __rhs.index()) return true;
  if (__lhs.index() > __rhs.index()) return false;
  return __variant::__visit_value_at(__lhs.index(), std::less<>{}, __lhs, __rhs);
}

template <class... _Types>
inline LUL_VIS_INLINE
constexpr bool operator>(const variant<_Types...>& __lhs,
                         const variant<_Types...>& __rhs) {
  using __variant_detail::__visitation::__variant;
  if (__lhs.valueless_by_exception()) return false;
  if (__rhs.valueless_by_exception()) return true;
  if (__lhs.index() > __rhs.index()) return true;
  if (__lhs.index() < __rhs.index()) return false;
  return __variant::__visit_value_at(__lhs.index(), std::greater<>{}, __lhs, __rhs);
}

template <class... _Types>
inline LUL_VIS_INLINE
constexpr bool operator<=(const variant<_Types...>& __lhs,
                          const variant<_Types...>& __rhs) {
  using __variant_detail::__visitation::__variant;
  if (__lhs.valueless_by_exception()) return true;
  if (__rhs.valueless_by_exception()) return false;
  if (__lhs.index() < __rhs.index()) return true;
  if (__lhs.index() > __rhs.index()) return false;
  return __variant::__visit_value_at(
      __lhs.index(), std::less_equal<>{}, __lhs, __rhs);
}

template <class... _Types>
inline LUL_VIS_INLINE
constexpr bool operator>=(const variant<_Types...>& __lhs,
                          const variant<_Types...>& __rhs) {
  using __variant_detail::__visitation::__variant;
  if (__rhs.valueless_by_exception()) return true;
  if (__lhs.valueless_by_exception()) return false;
  if (__lhs.index() > __rhs.index()) return true;
  if (__lhs.index() < __rhs.index()) return false;
  return __variant::__visit_value_at(
      __lhs.index(), std::greater_equal<>{}, __lhs, __rhs);
}

template <class _Visitor, class... _Vs>
inline LUL_VIS_INLINE
constexpr decltype(auto) visit(_Visitor&& __visitor, _Vs&&... __vs) {
  using __variant_detail::__visitation::__variant;
  bool __results[] = {__vs.valueless_by_exception()...};
  for (bool __result : __results) {
    if (__result) {
      __throw_bad_variant_access();
    }
  }
  return __variant::__visit_value(std::forward<_Visitor>(__visitor),
                                  std::forward<_Vs>(__vs)...);
}

struct LUL_VIS_TYPE_DEFINE monostate {};

inline LUL_VIS_INLINE
constexpr bool operator<(monostate, monostate) noexcept { return false; }

inline LUL_VIS_INLINE
constexpr bool operator>(monostate, monostate) noexcept { return false; }

inline LUL_VIS_INLINE
constexpr bool operator<=(monostate, monostate) noexcept { return true; }

inline LUL_VIS_INLINE
constexpr bool operator>=(monostate, monostate) noexcept { return true; }

inline LUL_VIS_INLINE
constexpr bool operator==(monostate, monostate) noexcept { return true; }

inline LUL_VIS_INLINE
constexpr bool operator!=(monostate, monostate) noexcept { return false; }

template <class... _Types>
inline LUL_VIS_INLINE
auto swap(variant<_Types...>& __lhs,
          variant<_Types...>& __rhs) noexcept(noexcept(__lhs.swap(__rhs)))
    -> decltype(__lhs.swap(__rhs)) {
  __lhs.swap(__rhs);
}

} // namespace stdproxy

LUL_end_v_namespace


//	Intentionally (and legally) provide a hash specialization in std
namespace std {

template <class... _Types>
struct LUL_VIS_TYPE_DEFINE hash<
    LUL_::stdproxy::details::__enable_hash_helper<LUL_::stdproxy::variant<_Types...>, remove_const_t<_Types>...>> {
  using argument_type = LUL_::stdproxy::variant<_Types...>;
  using result_type = size_t;

  inline LUL_VIS_INLINE
  result_type operator()(const argument_type& __v) const {
    using LUL_::stdproxy::__variant_detail::__visitation::__variant;
    size_t __res =
        __v.valueless_by_exception()
               ? 299792458 // Random value chosen by the universe upon creation
               : LUL_::stdproxy::__variant_detail::__visitation::__variant::__visit_alt(
                     [](const auto& __alt) {
                       using __alt_type = decay_t<decltype(__alt)>;
                       using __value_type = remove_const_t<
                         typename __alt_type::__value_type>;
                       return hash<__value_type>{}(__alt.__value);
                     },
                     __v);
    return __hash_combine(__res, hash<size_t>{}(__v.index()));
  }
};

template <>
struct LUL_VIS_TYPE_DEFINE hash<LUL_::stdproxy::monostate> {
  using argument_type = LUL_::stdproxy::monostate;
  using result_type = size_t;

  inline LUL_VIS_INLINE
  result_type operator()(const argument_type&) const _NOEXCEPT {
    return 66740831; // return a fundamentally attractive random value.
  }
};

}	//	namespace std


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
