/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Optional.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of libc++’s <optional> implementation,
	used under the University of Illinois/NCSA Open Source License. See
	“license/libc++ License” for details.

	The notable differences from the libc++ version include:

	* namespace changes
	* removal of some unneeded preprocessor tests and attributes
	* renaming of various macros to use LUL versions
	* renaming of system-reserved symbols

	SEEME This header requires a source file to define bad_optional_access.

------------------------------------------------------------------------------*/


#pragma once

#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif


/*
	optional synopsis

namespace std {

  // 23.6.3, optional for object types
  template <class T> class optional;

  // 23.6.4, no-value state indicator
  struct nullopt_t{see below };
  inline constexpr nullopt_t nullopt(unspecified );

  // 23.6.5, class bad_optional_access
  class bad_optional_access;

  // 23.6.6, relational operators
  template <class T, class U>
  constexpr bool operator==(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator!=(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator<(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator>(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator<=(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator>=(const optional<T>&, const optional<U>&);

  // 23.6.7 comparison with nullopt
  template <class T> constexpr bool operator==(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator==(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator!=(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator!=(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator<(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator<(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator<=(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator>(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator>(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator>=(nullopt_t, const optional<T>&) noexcept;

  // 23.6.8, comparison with T
  template <class T, class U> constexpr bool operator==(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator==(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator!=(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator!=(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator<(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator<(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator<=(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator<=(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator>(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator>(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator>=(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator>=(const T&, const optional<U>&);

  // 23.6.9, specialized algorithms
  template <class T> void swap(optional<T>&, optional<T>&) noexcept(see below );
  template <class T> constexpr optional<see below > make_optional(T&&);
  template <class T, class... Args>
	constexpr optional<T> make_optional(Args&&... args);
  template <class T, class U, class... Args>
	constexpr optional<T> make_optional(initializer_list<U> il, Args&&... args);

  // 23.6.10, hash support
  template <class T> struct hash;
  template <class T> struct hash<optional<T>>;

  template <class T> class optional {
  public:
	using value_type = T;

	// 23.6.3.1, constructors
	constexpr optional() noexcept;
	constexpr optional(nullopt_t) noexcept;
	optional(const optional &);
	optional(optional &&) noexcept(see below);
	template <class... Args> constexpr explicit optional(std::in_place_t, Args &&...);
	template <class U, class... Args>
	  constexpr explicit optional(std::in_place_t, std::initializer_list<U>, Args &&...);
	template <class U = T>
	  constexpr EXPLICIT optional(U &&);
	template <class U>
	  constexpr EXPLICIT optional(const optional<U> &);
	template <class U>
	  constexpr EXPLICIT optional(optional<U> &&);

	// 23.6.3.2, destructor
	~optional();

	// 23.6.3.3, assignment
	optional &operator=(nullopt_t) noexcept;
	optional &operator=(const optional &);
	optional &operator=(optional &&) noexcept(see below );
	template <class U = T> optional &operator=(U &&);
	template <class U> optional &operator=(const optional<U> &);
	template <class U> optional &operator=(optional<U> &&);
	template <class... Args> T& emplace(Args &&...);
	template <class U, class... Args>
	  T& emplace(initializer_list<U>, Args &&...);

	// 23.6.3.4, swap
	void swap(optional &) noexcept(see below );

	// 23.6.3.5, observers
	constexpr T const *operator->() const;
	constexpr T *operator->();
	constexpr T const &operator*() const &;
	constexpr T &operator*() &;
	constexpr T &&operator*() &&;
	constexpr const T &&operator*() const &&;
	constexpr explicit operator bool() const noexcept;
	constexpr bool has_value() const noexcept;
	constexpr T const &value() const &;
	constexpr T &value() &;
	constexpr T &&value() &&;
	constexpr const T &&value() const &&;
	template <class U> constexpr T value_or(U &&) const &;
	template <class U> constexpr T value_or(U &&) &&;

	// 23.6.3.6, modifiers
	void reset() noexcept;

  private:
	T *val; // exposition only
  };
  
} // namespace std

*/


//	std
#include <functional>
#include <initializer_list>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulHelperTuple.hpp>
#include <Lucena-Utilities/details/lulHelperTypeTraits.hpp>
#include <Lucena-Utilities/details/lulHelperUtility.hpp>
#include <Lucena-Utilities/details/lulVisibility.hpp>


//	purposefully avoiding versioned namespace
LUL_begin_namespace

namespace stdproxy {

class LUL_VIS_EXCEPTION bad_optional_access
	: public std::exception
{
public:
	virtual ~bad_optional_access() noexcept;
	virtual const char* what() const noexcept;
};

} // namespace stdproxy

LUL_end_namespace


LUL_begin_v_namespace

namespace stdproxy {

struct nullopt_t
{
	struct LUL_secret_tag { LUL_VIS_INLINE_FUNC explicit LUL_secret_tag() = default; };
	LUL_VIS_INLINE_FUNC constexpr explicit nullopt_t(LUL_secret_tag, LUL_secret_tag) noexcept {}
};

inline constexpr nullopt_t nullopt{nullopt_t::LUL_secret_tag{}, nullopt_t::LUL_secret_tag{}};


template <class _Tp, bool = std::is_trivially_destructible<_Tp>::value>
struct __optional_destruct_base;

template <class _Tp>
struct __optional_destruct_base<_Tp, false>
{
	typedef _Tp value_type;
	static_assert(std::is_object_v<value_type>,
		"instantiation of optional with a non-object type is undefined behavior");
	union
	{
		char __null_state_;
		value_type __val_;
	};
	bool __engaged_;

	LUL_VIS_INLINE_FUNC
	~__optional_destruct_base()
	{
		if (__engaged_)
			__val_.~value_type();
	}

	LUL_VIS_INLINE_FUNC
	constexpr __optional_destruct_base() noexcept
		:  __null_state_(),
		   __engaged_(false) {}

	template <class... _Args>
	LUL_VIS_INLINE_FUNC
	constexpr explicit __optional_destruct_base(std::in_place_t, _Args&&... __args)
		:  __val_(std::forward<_Args>(__args)...),
		   __engaged_(true) {}

	LUL_VIS_INLINE_FUNC
	void reset() noexcept
	{
		if (__engaged_)
		{
			__val_.~value_type();
			__engaged_ = false;
		}
	}
};

template <class _Tp>
struct __optional_destruct_base<_Tp, true>
{
	typedef _Tp value_type;
	static_assert(std::is_object_v<value_type>,
		"instantiation of optional with a non-object type is undefined behavior");
	union
	{
		char __null_state_;
		value_type __val_;
	};
	bool __engaged_;

	LUL_VIS_INLINE_FUNC
	constexpr __optional_destruct_base() noexcept
		:  __null_state_(),
		   __engaged_(false) {}

	template <class... _Args>
	LUL_VIS_INLINE_FUNC
	constexpr explicit __optional_destruct_base(std::in_place_t, _Args&&... __args)
		:  __val_(std::forward<_Args>(__args)...),
		   __engaged_(true) {}

	LUL_VIS_INLINE_FUNC
	void reset() noexcept
	{
		if (__engaged_)
		{
			__engaged_ = false;
		}
	}
};

template <class _Tp, bool = std::is_reference<_Tp>::value>
struct __optional_storage_base : __optional_destruct_base<_Tp>
{
	using __base = __optional_destruct_base<_Tp>;
	using value_type = _Tp;
	using __base::__base;

	LUL_VIS_INLINE_FUNC
	constexpr bool has_value() const noexcept
	{
		return this->__engaged_;
	}

	LUL_VIS_INLINE_FUNC
	constexpr value_type& __get() & noexcept
	{
		return this->__val_;
	}
	LUL_VIS_INLINE_FUNC
	constexpr const value_type& __get() const& noexcept
	{
		return this->__val_;
	}
	LUL_VIS_INLINE_FUNC
	constexpr value_type&& __get() && noexcept
	{
		return std::move(this->__val_);
	}
	LUL_VIS_INLINE_FUNC
	constexpr const value_type&& __get() const&& noexcept
	{
		return std::move(this->__val_);
	}

	template <class... _Args>
	LUL_VIS_INLINE_FUNC
	void __construct(_Args&&... __args)
	{
		//	C++2a
//		[[assert: !has_value()]];
		::new((void*)std::addressof(this->__val_)) value_type(std::forward<_Args>(__args)...);
		this->__engaged_ = true;
	}

	template <class _That>
	LUL_VIS_INLINE_FUNC
	void __construct_from(_That&& __opt)
	{
		if (__opt.has_value())
			__construct(std::forward<_That>(__opt).__get());
	}

	template <class _That>
	LUL_VIS_INLINE_FUNC
	void __assign_from(_That&& __opt)
	{
		if (this->__engaged_ == __opt.has_value())
		{
			if (this->__engaged_)
				this->__val_ = std::forward<_That>(__opt).__get();
		}
		else
		{
			if (this->__engaged_)
				this->reset();
			else
				__construct(std::forward<_That>(__opt).__get());
		}
	}
};

// optional<T&> is currently required ill-formed, however it may to be in the
// future. For this reason it has already been implemented to ensure we can
// make the change in an ABI compatible manner.
template <class _Tp>
struct __optional_storage_base<_Tp, true>
{
	using value_type = _Tp;
	using __raw_type = std::remove_reference_t<_Tp>;
	__raw_type* __value_;

	template <class _Up>
	static constexpr bool __can_bind_reference() {
		using _RawUp = typename std::remove_reference<_Up>::type;
		using _UpPtr = _RawUp*;
		using _RawTp = typename std::remove_reference<_Tp>::type;
		using _TpPtr = _RawTp*;
		using _CheckLValueArg = std::bool_constant<
			(std::is_lvalue_reference<_Up>::value && std::is_convertible<_UpPtr, _TpPtr>::value)
		||	std::is_same<_RawUp, std::reference_wrapper<_RawTp>>::value
		||	std::is_same<_RawUp, std::reference_wrapper<typename std::remove_const<_RawTp>::type>>::value
		>;
		return (std::is_lvalue_reference<_Tp>::value && _CheckLValueArg::value)
			|| (std::is_rvalue_reference<_Tp>::value && !std::is_lvalue_reference<_Up>::value &&
				std::is_convertible<_UpPtr, _TpPtr>::value);
	}

	LUL_VIS_INLINE_FUNC
	constexpr __optional_storage_base() noexcept
		:  __value_(nullptr) {}

	template <class _UArg>
	LUL_VIS_INLINE_FUNC
	constexpr explicit __optional_storage_base(std::in_place_t, _UArg&& __uarg)
		:  __value_(std::addressof(__uarg))
	{
	  static_assert(__can_bind_reference<_UArg>(),
		"Attempted to construct a reference element in tuple from a "
		"possible temporary");
	}

	LUL_VIS_INLINE_FUNC
	void reset() noexcept { __value_ = nullptr; }

	LUL_VIS_INLINE_FUNC
	constexpr bool has_value() const noexcept
	  { return __value_ != nullptr; }

	LUL_VIS_INLINE_FUNC
	constexpr value_type& __get() const& noexcept
	  { return *__value_; }

	LUL_VIS_INLINE_FUNC
	constexpr value_type&& __get() const&& noexcept
	  { return std::forward<value_type>(*__value_); }

	template <class _UArg>
	LUL_VIS_INLINE_FUNC
	void __construct(_UArg&& __val)
	{
		assert (!has_value());
		static_assert(__can_bind_reference<_UArg>(),
			"Attempted to construct a reference element in tuple from a "
			"possible temporary");
		__value_ = std::addressof(__val);
	}

	template <class _That>
	LUL_VIS_INLINE_FUNC
	void __construct_from(_That&& __opt)
	{
		if (__opt.has_value())
			__construct(std::forward<_That>(__opt).__get());
	}

	template <class _That>
	LUL_VIS_INLINE_FUNC
	void __assign_from(_That&& __opt)
	{
		if (has_value() == __opt.has_value())
		{
			if (has_value())
				*__value_ = std::forward<_That>(__opt).__get();
		}
		else
		{
			if (has_value())
				reset();
			else
				__construct(std::forward<_That>(__opt).__get());
		}
	}
};

template <class _Tp, bool = std::is_trivially_copy_constructible<_Tp>::value>
struct __optional_copy_base : __optional_storage_base<_Tp>
{
	using __optional_storage_base<_Tp>::__optional_storage_base;
};

template <class _Tp>
struct __optional_copy_base<_Tp, false> : __optional_storage_base<_Tp>
{
	using __optional_storage_base<_Tp>::__optional_storage_base;

	LUL_VIS_INLINE_FUNC
	__optional_copy_base() = default;

	LUL_VIS_INLINE_FUNC
	__optional_copy_base(const __optional_copy_base& __opt)
	{
		this->__construct_from(__opt);
	}

	LUL_VIS_INLINE_FUNC
	__optional_copy_base(__optional_copy_base&&) = default;
	LUL_VIS_INLINE_FUNC
	__optional_copy_base& operator=(const __optional_copy_base&) = default;
	LUL_VIS_INLINE_FUNC
	__optional_copy_base& operator=(__optional_copy_base&&) = default;
};

template <class _Tp, bool = std::is_trivially_move_constructible<_Tp>::value>
struct __optional_move_base : __optional_copy_base<_Tp>
{
	using __optional_copy_base<_Tp>::__optional_copy_base;
};

template <class _Tp>
struct __optional_move_base<_Tp, false> : __optional_copy_base<_Tp>
{
	using value_type = _Tp;
	using __optional_copy_base<_Tp>::__optional_copy_base;

	LUL_VIS_INLINE_FUNC
	__optional_move_base() = default;
	LUL_VIS_INLINE_FUNC
	__optional_move_base(const __optional_move_base&) = default;

	LUL_VIS_INLINE_FUNC
	__optional_move_base(__optional_move_base&& __opt)
		noexcept(std::is_nothrow_move_constructible_v<value_type>)
	{
		this->__construct_from(std::move(__opt));
	}

	LUL_VIS_INLINE_FUNC
	__optional_move_base& operator=(const __optional_move_base&) = default;
	LUL_VIS_INLINE_FUNC
	__optional_move_base& operator=(__optional_move_base&&) = default;
};

template <class _Tp, bool =
	std::is_trivially_destructible<_Tp>::value &&
	std::is_trivially_copy_constructible<_Tp>::value &&
	std::is_trivially_copy_assignable<_Tp>::value>
struct __optional_copy_assign_base : __optional_move_base<_Tp>
{
	using __optional_move_base<_Tp>::__optional_move_base;
};

template <class _Tp>
struct __optional_copy_assign_base<_Tp, false> : __optional_move_base<_Tp>
{
	using __optional_move_base<_Tp>::__optional_move_base;

	LUL_VIS_INLINE_FUNC
	__optional_copy_assign_base() = default;
	LUL_VIS_INLINE_FUNC
	__optional_copy_assign_base(const __optional_copy_assign_base&) = default;
	LUL_VIS_INLINE_FUNC
	__optional_copy_assign_base(__optional_copy_assign_base&&) = default;

	LUL_VIS_INLINE_FUNC
	__optional_copy_assign_base& operator=(const __optional_copy_assign_base& __opt)
	{
		this->__assign_from(__opt);
		return *this;
	}

	LUL_VIS_INLINE_FUNC
	__optional_copy_assign_base& operator=(__optional_copy_assign_base&&) = default;
};

template <class _Tp, bool =
	std::is_trivially_destructible<_Tp>::value &&
	std::is_trivially_move_constructible<_Tp>::value &&
	std::is_trivially_move_assignable<_Tp>::value>
struct __optional_move_assign_base : __optional_copy_assign_base<_Tp>
{
	using __optional_copy_assign_base<_Tp>::__optional_copy_assign_base;
};

template <class _Tp>
struct __optional_move_assign_base<_Tp, false> : __optional_copy_assign_base<_Tp>
{
	using value_type = _Tp;
	using __optional_copy_assign_base<_Tp>::__optional_copy_assign_base;

	LUL_VIS_INLINE_FUNC
	__optional_move_assign_base() = default;
	LUL_VIS_INLINE_FUNC
	__optional_move_assign_base(const __optional_move_assign_base& __opt) = default;
	LUL_VIS_INLINE_FUNC
	__optional_move_assign_base(__optional_move_assign_base&&) = default;
	LUL_VIS_INLINE_FUNC
	__optional_move_assign_base& operator=(const __optional_move_assign_base&) = default;

	LUL_VIS_INLINE_FUNC
	__optional_move_assign_base& operator=(__optional_move_assign_base&& __opt)
		noexcept(std::is_nothrow_move_assignable_v<value_type> &&
				 std::is_nothrow_move_constructible_v<value_type>)
	{
		this->__assign_from(std::move(__opt));
		return *this;
	}
};

template <class _Tp>
using __optional_sfinae_ctor_base_t = details::__sfinae_ctor_base<
	std::is_copy_constructible<_Tp>::value,
	std::is_move_constructible<_Tp>::value
>;

template <class _Tp>
using __optional_sfinae_assign_base_t = details::__sfinae_assign_base<
	(std::is_copy_constructible<_Tp>::value && std::is_copy_assignable<_Tp>::value),
	(std::is_move_constructible<_Tp>::value && std::is_move_assignable<_Tp>::value)
>;

template <class _Tp>
class optional
	: private __optional_move_assign_base<_Tp>
	, private __optional_sfinae_ctor_base_t<_Tp>
	, private __optional_sfinae_assign_base_t<_Tp>
{
	using __base = __optional_move_assign_base<_Tp>;
public:
	using value_type = _Tp;

private:
	 // Disable the reference extension using this static assert.
	static_assert(!std::is_same_v<value_type, std::in_place_t>,
		"instantiation of optional with in_place_t is ill-formed");
	static_assert(!std::is_same_v<details::__uncvref_t<value_type>, nullopt_t>,
		"instantiation of optional with nullopt_t is ill-formed");
	static_assert(!std::is_reference_v<value_type>,
		"instantiation of optional with a reference type is ill-formed");
	static_assert(std::is_destructible_v<value_type>,
		"instantiation of optional with a non-destructible type is ill-formed");

	// LWG2756: conditionally explicit conversion from _Up
	struct _CheckOptionalArgsConstructor {
	  template <class _Up>
	  static constexpr bool __enable_implicit() {
		  return std::is_constructible_v<_Tp, _Up&&> &&
				 std::is_convertible_v<_Up&&, _Tp>;
	  }

	  template <class _Up>
	  static constexpr bool __enable_explicit() {
		  return std::is_constructible_v<_Tp, _Up&&> &&
				 !std::is_convertible_v<_Up&&, _Tp>;
	  }
	};
	template <class _Up>
	using _CheckOptionalArgsCtor = std::conditional_t<
		!std::is_same_v<details::__uncvref_t<_Up>, std::in_place_t> &&
		!std::is_same_v<details::__uncvref_t<_Up>, optional>,
		_CheckOptionalArgsConstructor,
		details::__check_tuple_constructor_fail
	>;
	template <class _QualUp>
	struct _CheckOptionalLikeConstructor {
	  template <class _Up, class _Opt = optional<_Up>>
	  using __check_constructible_from_opt = details::__lazy_or<
		  std::is_constructible<_Tp, _Opt&>,
		  std::is_constructible<_Tp, _Opt const&>,
		  std::is_constructible<_Tp, _Opt&&>,
		  std::is_constructible<_Tp, _Opt const&&>,
		  std::is_convertible<_Opt&, _Tp>,
		  std::is_convertible<_Opt const&, _Tp>,
		  std::is_convertible<_Opt&&, _Tp>,
		  std::is_convertible<_Opt const&&, _Tp>
	  >;
	  template <class _Up, class _Opt = optional<_Up>>
	  using __check_assignable_from_opt = details::__lazy_or<
		  std::is_assignable<_Tp&, _Opt&>,
		  std::is_assignable<_Tp&, _Opt const&>,
		  std::is_assignable<_Tp&, _Opt&&>,
		  std::is_assignable<_Tp&, _Opt const&&>
	  >;
	  template <class _Up, class _QUp = _QualUp>
	  static constexpr bool __enable_implicit() {
		  return std::is_convertible<_QUp, _Tp>::value &&
			  !__check_constructible_from_opt<_Up>::value;
	  }
	  template <class _Up, class _QUp = _QualUp>
	  static constexpr bool __enable_explicit() {
		  return !std::is_convertible<_QUp, _Tp>::value &&
			  !__check_constructible_from_opt<_Up>::value;
	  }
	  template <class _Up, class _QUp = _QualUp>
	  static constexpr bool __enable_assign() {
		  // Construction and assignability of _Qup to _Tp has already been
		  // checked.
		  return !__check_constructible_from_opt<_Up>::value &&
			  !__check_assignable_from_opt<_Up>::value;
	  }
	};

	template <class _Up, class _QualUp>
	using _CheckOptionalLikeCtor = std::conditional_t<
	  details::__lazy_and<
		  details::__lazy_not<std::is_same<_Up, _Tp>>,
		  std::is_constructible<_Tp, _QualUp>
	  >::value,
	  _CheckOptionalLikeConstructor<_QualUp>,
	  details::__check_tuple_constructor_fail
	>;
	template <class _Up, class _QualUp>
	using _CheckOptionalLikeAssign = std::conditional_t<
	  details::__lazy_and<
		  details::__lazy_not<std::is_same<_Up, _Tp>>,
		  std::is_constructible<_Tp, _QualUp>,
		  std::is_assignable<_Tp&, _QualUp>
	  >::value,
	  _CheckOptionalLikeConstructor<_QualUp>,
	  details::__check_tuple_constructor_fail
	>;
public:

	LUL_VIS_INLINE_FUNC constexpr optional() noexcept {}
	LUL_VIS_INLINE_FUNC constexpr optional(const optional&) = default;
	LUL_VIS_INLINE_FUNC constexpr optional(optional&&) = default;
	LUL_VIS_INLINE_FUNC constexpr optional(nullopt_t) noexcept {}

	template <class... _Args, class = std::enable_if_t<
		std::is_constructible_v<value_type, _Args...>>
	>
	LUL_VIS_INLINE_FUNC
	constexpr explicit optional(std::in_place_t, _Args&&... __args)
		: __base(std::in_place, std::forward<_Args>(__args)...) {}

	template <class _Up, class... _Args, class = std::enable_if_t<
		std::is_constructible_v<value_type, std::initializer_list<_Up>&, _Args...>>
	>
	LUL_VIS_INLINE_FUNC
	constexpr explicit optional(std::in_place_t, std::initializer_list<_Up> __il, _Args&&... __args)
		: __base(std::in_place, __il, std::forward<_Args>(__args)...) {}

	template <class _Up = value_type, std::enable_if_t<
		_CheckOptionalArgsCtor<_Up>::template __enable_implicit<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	constexpr optional(_Up&& __v)
		: __base(std::in_place, std::forward<_Up>(__v)) {}

	template <class _Up, std::enable_if_t<
		_CheckOptionalArgsCtor<_Up>::template __enable_explicit<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	constexpr explicit optional(_Up&& __v)
		: __base(std::in_place, std::forward<_Up>(__v)) {}

	// LWG2756: conditionally explicit conversion from const optional<_Up>&
	template <class _Up, std::enable_if_t<
		_CheckOptionalLikeCtor<_Up, _Up const&>::template __enable_implicit<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	optional(const optional<_Up>& __v)
	{
		this->__construct_from(__v);
	}
	template <class _Up, std::enable_if_t<
		_CheckOptionalLikeCtor<_Up, _Up const&>::template __enable_explicit<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	explicit optional(const optional<_Up>& __v)
	{
		this->__construct_from(__v);
	}

	// LWG2756: conditionally explicit conversion from optional<_Up>&&
	template <class _Up, std::enable_if_t<
		_CheckOptionalLikeCtor<_Up, _Up &&>::template __enable_implicit<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	optional(optional<_Up>&& __v)
	{
		this->__construct_from(std::move(__v));
	}
	template <class _Up, std::enable_if_t<
		_CheckOptionalLikeCtor<_Up, _Up &&>::template __enable_explicit<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	explicit optional(optional<_Up>&& __v)
	{
		this->__construct_from(std::move(__v));
	}

	LUL_VIS_INLINE_FUNC
	optional& operator=(nullopt_t) noexcept
	{
		reset();
		return *this;
	}

	LUL_VIS_INLINE_FUNC optional& operator=(const optional&) = default;
	LUL_VIS_INLINE_FUNC optional& operator=(optional&&) = default;

	// LWG2756
	template <class _Up = value_type,
			  class = std::enable_if_t
					  <details::__lazy_and<
						  std::bool_constant<
							  !std::is_same_v<details::__uncvref_t<_Up>, optional> &&
							  !(std::is_same_v<_Up, value_type> && std::is_scalar_v<value_type>)
						  >,
						  std::is_constructible<value_type, _Up>,
						  std::is_assignable<value_type&, _Up>
					  >::value>
			 >
	LUL_VIS_INLINE_FUNC
	optional&
	operator=(_Up&& __v)
	{
		if (this->has_value())
			this->__get() = std::forward<_Up>(__v);
		else
			this->__construct(std::forward<_Up>(__v));
		return *this;
	}

	// LWG2756
	template <class _Up, std::enable_if_t<
		_CheckOptionalLikeAssign<_Up, _Up const&>::template __enable_assign<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	optional&
	operator=(const optional<_Up>& __v)
	{
		this->__assign_from(__v);
		return *this;
	}

	// LWG2756
	template <class _Up, std::enable_if_t<
		_CheckOptionalLikeCtor<_Up, _Up &&>::template __enable_assign<_Up>()
	, int> = 0>
	LUL_VIS_INLINE_FUNC
	optional&
	operator=(optional<_Up>&& __v)
	{
		this->__assign_from(std::move(__v));
		return *this;
	}

	template <class... _Args,
			  class = std::enable_if_t
					  <
						  std::is_constructible_v<value_type, _Args...>
					  >
			 >
	LUL_VIS_INLINE_FUNC
	_Tp &
	emplace(_Args&&... __args)
	{
		reset();
		this->__construct(std::forward<_Args>(__args)...);
		return this->__get();
	}

	template <class _Up, class... _Args,
			  class = std::enable_if_t
					  <
						  std::is_constructible_v<value_type, std::initializer_list<_Up>&, _Args...>
					  >
			 >
	LUL_VIS_INLINE_FUNC
	_Tp &
	emplace(std::initializer_list<_Up> __il, _Args&&... __args)
	{
		reset();
		this->__construct(__il, std::forward<_Args>(__args)...);
		return this->__get();
	}

	LUL_VIS_INLINE_FUNC
	void swap(optional& __opt)
		noexcept(std::is_nothrow_move_constructible_v<value_type> &&
				 std::is_nothrow_swappable_v<value_type>)
	{
		if (this->has_value() == __opt.has_value())
		{
			using std::swap;
			if (this->has_value())
				swap(this->__get(), __opt.__get());
		}
		else
		{
			if (this->has_value())
			{
				__opt.__construct(std::move(this->__get()));
				reset();
			}
			else
			{
				this->__construct(std::move(__opt.__get()));
				__opt.reset();
			}
		}
	}

	LUL_VIS_INLINE_FUNC
	constexpr
	std::add_pointer_t<value_type const>
	operator->() const
	{
		assert (this->has_value());

		return std::addressof(this->__get());
	}

	LUL_VIS_INLINE_FUNC
	constexpr
	std::add_pointer_t<value_type>
	operator->()
	{
		assert (this->has_value());

		return std::addressof(this->__get());
	}

	LUL_VIS_INLINE_FUNC
	constexpr
	const value_type&
	operator*() const&
	{
	   assert (this->has_value());
	   return this->__get();
	}

	LUL_VIS_INLINE_FUNC
	constexpr
	value_type&
	operator*() &
	{
		assert (this->has_value());
		return this->__get();
	}

	LUL_VIS_INLINE_FUNC
	constexpr
	value_type&&
	operator*() &&
	{
		assert (this->has_value());
		return std::move(this->__get());
	}

	LUL_VIS_INLINE_FUNC
	constexpr
	const value_type&&
	operator*() const&&
	{
		assert (this->has_value());
		return std::move(this->__get());
	}

	LUL_VIS_INLINE_FUNC
	constexpr explicit operator bool() const noexcept { return has_value(); }

	using __base::has_value;
	using __base::__get;

	LUL_VIS_INLINE_FUNC
	constexpr value_type const& value() const&
	{
		if (!this->has_value())
			throw bad_optional_access();
		return this->__get();
	}

	LUL_VIS_INLINE_FUNC
	constexpr value_type& value() &
	{
		if (!this->has_value())
			throw bad_optional_access();
		return this->__get();
	}

	LUL_VIS_INLINE_FUNC
	constexpr value_type&& value() &&
	{
		if (!this->has_value())
			throw bad_optional_access();
		return std::move(this->__get());
	}

	LUL_VIS_INLINE_FUNC
	constexpr value_type const&& value() const&&
	{
		if (!this->has_value())
			throw bad_optional_access();
		return std::move(this->__get());
	}

	template <class _Up>
	LUL_VIS_INLINE_FUNC
	constexpr value_type value_or(_Up&& __v) const&
	{
		static_assert(std::is_copy_constructible_v<value_type>,
					  "optional<T>::value_or: T must be copy constructible");
		static_assert(std::is_convertible_v<_Up, value_type>,
					  "optional<T>::value_or: U must be convertible to T");
		return this->has_value() ? this->__get() :
								  static_cast<value_type>(std::forward<_Up>(__v));
	}

	template <class _Up>
	LUL_VIS_INLINE_FUNC
	constexpr value_type value_or(_Up&& __v) &&
	{
		static_assert(std::is_move_constructible_v<value_type>,
					  "optional<T>::value_or: T must be move constructible");
		static_assert(std::is_convertible_v<_Up, value_type>,
					  "optional<T>::value_or: U must be convertible to T");
		return this->has_value() ? std::move(this->__get()) :
								  static_cast<value_type>(std::forward<_Up>(__v));
	}

	using __base::reset;

private:
	template <class _Up>
	LUL_VIS_INLINE_FUNC
	static _Up*
	__operator_arrow(std::true_type, _Up& __x)
	{
		return std::addressof(__x);
	}

	template <class _Up>
	LUL_VIS_INLINE_FUNC
	static constexpr _Up*
	__operator_arrow(std::false_type, _Up& __x)
	{
		return &__x;
	}
};

// Comparisons between optionals
template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() ==
		std::declval<const _Up&>()), bool>,
	bool
>
operator==(const optional<_Tp>& __x, const optional<_Up>& __y)
{
	if (static_cast<bool>(__x) != static_cast<bool>(__y))
		return false;
	if (!static_cast<bool>(__x))
		return true;
	return *__x == *__y;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() !=
		std::declval<const _Up&>()), bool>,
	bool
>
operator!=(const optional<_Tp>& __x, const optional<_Up>& __y)
{
	if (static_cast<bool>(__x) != static_cast<bool>(__y))
		return true;
	if (!static_cast<bool>(__x))
		return false;
	return *__x != *__y;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() <
		std::declval<const _Up&>()), bool>,
	bool
>
operator<(const optional<_Tp>& __x, const optional<_Up>& __y)
{
	if (!static_cast<bool>(__y))
		return false;
	if (!static_cast<bool>(__x))
		return true;
	return *__x < *__y;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() >
		std::declval<const _Up&>()), bool>,
	bool
>
operator>(const optional<_Tp>& __x, const optional<_Up>& __y)
{
	if (!static_cast<bool>(__x))
		return false;
	if (!static_cast<bool>(__y))
		return true;
	return *__x > *__y;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() <=
		std::declval<const _Up&>()), bool>,
	bool
>
operator<=(const optional<_Tp>& __x, const optional<_Up>& __y)
{
	if (!static_cast<bool>(__x))
		return true;
	if (!static_cast<bool>(__y))
		return false;
	return *__x <= *__y;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() >=
		std::declval<const _Up&>()), bool>,
	bool
>
operator>=(const optional<_Tp>& __x, const optional<_Up>& __y)
{
	if (!static_cast<bool>(__y))
		return true;
	if (!static_cast<bool>(__x))
		return false;
	return *__x >= *__y;
}

// Comparisons with nullopt
template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator==(const optional<_Tp>& __x, nullopt_t) noexcept
{
	return !static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator==(nullopt_t, const optional<_Tp>& __x) noexcept
{
	return !static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator!=(const optional<_Tp>& __x, nullopt_t) noexcept
{
	return static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator!=(nullopt_t, const optional<_Tp>& __x) noexcept
{
	return static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator<(const optional<_Tp>&, nullopt_t) noexcept
{
	return false;
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator<(nullopt_t, const optional<_Tp>& __x) noexcept
{
	return static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator<=(const optional<_Tp>& __x, nullopt_t) noexcept
{
	return !static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator<=(nullopt_t, const optional<_Tp>&) noexcept
{
	return true;
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator>(const optional<_Tp>& __x, nullopt_t) noexcept
{
	return static_cast<bool>(__x);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator>(nullopt_t, const optional<_Tp>&) noexcept
{
	return false;
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator>=(const optional<_Tp>&, nullopt_t) noexcept
{
	return true;
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
bool
operator>=(nullopt_t, const optional<_Tp>& __x) noexcept
{
	return !static_cast<bool>(__x);
}

// Comparisons with T
template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() ==
		std::declval<const _Up&>()), bool>,
	bool
>
operator==(const optional<_Tp>& __x, const _Up& __v)
{
	return static_cast<bool>(__x) ? *__x == __v : false;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() ==
		std::declval<const _Up&>()), bool>,
	bool
>
operator==(const _Tp& __v, const optional<_Up>& __x)
{
	return static_cast<bool>(__x) ? __v == *__x : false;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() !=
		std::declval<const _Up&>()), bool>,
	bool
>
operator!=(const optional<_Tp>& __x, const _Up& __v)
{
	return static_cast<bool>(__x) ? *__x != __v : true;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() !=
		std::declval<const _Up&>()), bool>,
	bool
>
operator!=(const _Tp& __v, const optional<_Up>& __x)
{
	return static_cast<bool>(__x) ? __v != *__x : true;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() <
		std::declval<const _Up&>()), bool>,
	bool
>
operator<(const optional<_Tp>& __x, const _Up& __v)
{
	return static_cast<bool>(__x) ? *__x < __v : true;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() <
		std::declval<const _Up&>()), bool>,
	bool
>
operator<(const _Tp& __v, const optional<_Up>& __x)
{
	return static_cast<bool>(__x) ? __v < *__x : false;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() <=
		std::declval<const _Up&>()), bool>,
	bool
>
operator<=(const optional<_Tp>& __x, const _Up& __v)
{
	return static_cast<bool>(__x) ? *__x <= __v : true;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() <=
		std::declval<const _Up&>()), bool>,
	bool
>
operator<=(const _Tp& __v, const optional<_Up>& __x)
{
	return static_cast<bool>(__x) ? __v <= *__x : false;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() >
		std::declval<const _Up&>()), bool>,
	bool
>
operator>(const optional<_Tp>& __x, const _Up& __v)
{
	return static_cast<bool>(__x) ? *__x > __v : false;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() >
		std::declval<const _Up&>()), bool>,
	bool
>
operator>(const _Tp& __v, const optional<_Up>& __x)
{
	return static_cast<bool>(__x) ? __v > *__x : true;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() >=
		std::declval<const _Up&>()), bool>,
	bool
>
operator>=(const optional<_Tp>& __x, const _Up& __v)
{
	return static_cast<bool>(__x) ? *__x >= __v : false;
}

template <class _Tp, class _Up>
LUL_VIS_INLINE_FUNC constexpr
std::enable_if_t<
	std::is_convertible_v<decltype(std::declval<const _Tp&>() >=
		std::declval<const _Up&>()), bool>,
	bool
>
operator>=(const _Tp& __v, const optional<_Up>& __x)
{
	return static_cast<bool>(__x) ? __v >= *__x : true;
}


template <class _Tp>
inline LUL_VIS_INLINE_FUNC
std::enable_if_t<
	std::is_move_constructible_v<_Tp> && std::is_swappable_v<_Tp>,
	void
>
swap(optional<_Tp>& __x, optional<_Tp>& __y) noexcept(noexcept(__x.swap(__y)))
{
	__x.swap(__y);
}

template <class _Tp>
LUL_VIS_INLINE_FUNC constexpr
optional<std::decay_t<_Tp>> make_optional(_Tp&& __v)
{
	return optional<std::decay_t<_Tp>>(std::forward<_Tp>(__v));
}

template <class _Tp, class... _Args>
LUL_VIS_INLINE_FUNC constexpr
optional<_Tp> make_optional(_Args&&... __args)
{
	return optional<_Tp>(std::in_place, std::forward<_Args>(__args)...);
}

template <class _Tp, class _Up, class... _Args>
LUL_VIS_INLINE_FUNC constexpr
optional<_Tp> make_optional(std::initializer_list<_Up> __il,  _Args&&... __args)
{
	return optional<_Tp>(std::in_place, __il, std::forward<_Args>(__args)...);
}

} // namespace stdproxy

LUL_end_v_namespace


//	Intentionally (and legally) provide a hash specialization in std
namespace std {

//	Note the explicit use of a versioned namespace when referring to optional;
//	this is needed to avoid potential collisions.
template <class _Tp>
struct LUL_VIS_CLASS_TEMPLATE hash<
	LUL_::stdproxy::details::__enable_hash_helper<LUL_v_::stdproxy::optional<_Tp>, remove_const_t<_Tp>>
>
{
	typedef LUL_v_::stdproxy::optional<_Tp> argument_type;
	typedef size_t		  result_type;

	LUL_VIS_INLINE_FUNC
	result_type operator()(const argument_type& __opt) const
	{
		return static_cast<bool>(__opt) ? hash<remove_const_t<_Tp>>()(*__opt) : 0;
	}
};

}	//	namespace std


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
