/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Any.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of libc++’s <any> implementation,
	used under the University of Illinois/NCSA Open Source License. See
	“license/libc++ License” for details.

	The notable differences from the libc++ version include:

	* namespace changes
	* removal of some unneeded preprocessor tests and attributes
	* renaming of various macros to use LUL versions
	* renaming of system-reserved symbols

	SEEME This header requires a source file to define bad_any_cast.

------------------------------------------------------------------------------*/


#pragma once

#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif


/*
   any synopsis

namespace std {

  class bad_any_cast : public bad_cast
  {
  public:
    virtual const char* what() const noexcept;
  };

  class any
  {
  public:

    // 6.3.1 any construct/destruct
    any() noexcept;

    any(const any& other);
    any(any&& other) noexcept;

    template <class ValueType>
      any(ValueType&& value);

    ~any();

    // 6.3.2 any assignments
    any& operator=(const any& rhs);
    any& operator=(any&& rhs) noexcept;

    template <class ValueType>
      any& operator=(ValueType&& rhs);

    // 6.3.3 any modifiers
    template <class ValueType, class... Args>
      decay_t<ValueType>& emplace(Args&&... args);
    template <class ValueType, class U, class... Args>
      decay_t<ValueType>& emplace(initializer_list<U>, Args&&...);
    void reset() noexcept;
    void swap(any& rhs) noexcept;

    // 6.3.4 any observers
    bool has_value() const noexcept;
    const type_info& type() const noexcept;
  };

   // 6.4 Non-member functions
  void swap(any& x, any& y) noexcept;

  template <class T, class ...Args>
    any make_any(Args&& ...args);
  template <class T, class U, class ...Args>
    any make_any(initializer_list<U>, Args&& ...args);

  template<class ValueType>
    ValueType any_cast(const any& operand);
  template<class ValueType>
    ValueType any_cast(any& operand);
  template<class ValueType>
    ValueType any_cast(any&& operand);

  template<class ValueType>
    const ValueType* any_cast(const any* operand) noexcept;
  template<class ValueType>
    ValueType* any_cast(any* operand) noexcept;

} // namespace std

*/


//	std
#include <initializer_list>
#include <memory>
#include <new>
#include <typeinfo>
#include <type_traits>
#include <cstdlib>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulHelperMemory.hpp>
#include <Lucena-Utilities/details/lulHelperTypeTraits.hpp>
#include <Lucena-Utilities/details/lulHelperUtility.hpp>


//	purposefully avoiding versioned namespace
LUL_begin_namespace

namespace stdproxy {

class LUL_VIS_EXCEPTION_DEFINE bad_any_cast
	:	public std::bad_cast
{
	public:
		virtual const char* what() const noexcept;
};

} // namespace stdproxy

LUL_end_namespace

LUL_begin_v_namespace

namespace stdproxy {

[[noreturn]] inline LUL_VIS_ALWAYS_INLINE
void __throw_bad_any_cast()
{
    throw bad_any_cast();
}

// Forward declarations
class LUL_VIS_TYPE_DEFINE any;

template <class _ValueType>
LUL_VIS_INLINE
std::add_pointer_t<std::add_const_t<_ValueType>>
any_cast(any const *) noexcept;

template <class _ValueType>
LUL_VIS_INLINE
std::add_pointer_t<_ValueType> any_cast(any *) noexcept;

namespace __any_imp
{
  using _Buffer = std::aligned_storage_t<3*sizeof(void*), std::alignment_of<void*>::value>;

  template <class _Tp>
  using _IsSmallObject = std::bool_constant<
  		sizeof(_Tp) <= sizeof(_Buffer)
          && std::alignment_of<_Buffer>::value
             % std::alignment_of<_Tp>::value == 0
          && std::is_nothrow_move_constructible<_Tp>::value
        >;

  enum class _Action {
    _Destroy,
    _Copy,
    _Move,
    _Get,
    _TypeInfo
  };

  template <class _Tp> struct _SmallHandler;
  template <class _Tp> struct _LargeHandler;

  template <class _Tp>
  struct  LUL_VIS_TYPE_DEFINE __unique_typeinfo { static constexpr int __id = 0; };
  template <class _Tp> constexpr int __unique_typeinfo<_Tp>::__id;

  template <class _Tp>
  inline LUL_VIS_INLINE
  constexpr const void* __get_fallback_typeid() {
      return &__unique_typeinfo<std::decay_t<_Tp>>::__id;
  }

  template <class _Tp>
  inline LUL_VIS_INLINE
  bool __compare_typeid(std::type_info const* __id, const void* __fallback_id)
  {
      if (__id && *__id == typeid(_Tp))
          return true;

      if (!__id && __fallback_id == __any_imp::__get_fallback_typeid<_Tp>())
          return true;
      return false;
  }

  template <class _Tp>
  using _Handler = std::conditional_t<
    _IsSmallObject<_Tp>::value, _SmallHandler<_Tp>, _LargeHandler<_Tp>>;

} // namespace __any_imp

class LUL_VIS_TYPE_DEFINE any
{
public:
  // construct/destruct
  LUL_VIS_INLINE
  constexpr any() noexcept : __h(nullptr) {}

  LUL_VIS_INLINE
  any(any const & __other) : __h(nullptr)
  {
    if (__other.__h) __other.__call(_Action::_Copy, this);
  }

  LUL_VIS_INLINE
  any(any && __other) noexcept : __h(nullptr)
  {
    if (__other.__h) __other.__call(_Action::_Move, this);
  }

  template <
      class _ValueType
    , class _Tp = std::decay_t<_ValueType>
    , class = std::enable_if_t<
        !std::is_same_v<_Tp, any> &&
        !details::__is_inplace_type_v<_ValueType> &&
        std::is_copy_constructible_v<_Tp>>
    >
  LUL_VIS_INLINE
  any(_ValueType && __value);

  template <class _ValueType, class ..._Args,
    class _Tp = std::decay_t<_ValueType>,
    class = std::enable_if_t<
        std::is_constructible<_Tp, _Args...>::value &&
        std::is_copy_constructible<_Tp>::value
    >
  >
  LUL_VIS_INLINE
  explicit any(std::in_place_type_t<_ValueType>, _Args&&... __args);

  template <class _ValueType, class _Up, class ..._Args,
    class _Tp = std::decay_t<_ValueType>,
    class = std::enable_if_t<
        std::is_constructible_v<_Tp, std::initializer_list<_Up>&, _Args...> &&
        std::is_copy_constructible_v<_Tp>>
  >
  LUL_VIS_INLINE
  explicit any(std::in_place_type_t<_ValueType>, std::initializer_list<_Up>, _Args&&... __args);

  LUL_VIS_INLINE
  ~any() { this->reset(); }

  // assignments
  LUL_VIS_INLINE
  any & operator=(any const & __rhs) {
    any(__rhs).swap(*this);
    return *this;
  }

  LUL_VIS_INLINE
  any & operator=(any && __rhs) noexcept {
    any(std::move(__rhs)).swap(*this);
    return *this;
  }

  template <
      class _ValueType
    , class _Tp = std::decay_t<_ValueType>
    , class = std::enable_if_t<
          !std::is_same<_Tp, any>::value
          && std::is_copy_constructible<_Tp>::value>
    >
  LUL_VIS_INLINE
  any & operator=(_ValueType && __rhs);

  template <class _ValueType, class ..._Args,
    class _Tp = std::decay_t<_ValueType>,
    class = std::enable_if_t<
        std::is_constructible<_Tp, _Args...>::value &&
        std::is_copy_constructible<_Tp>::value>
    >
  LUL_VIS_INLINE
  _Tp& emplace(_Args&&... args);

  template <class _ValueType, class _Up, class ..._Args,
    class _Tp = std::decay_t<_ValueType>,
    class = std::enable_if_t<
        std::is_constructible<_Tp, std::initializer_list<_Up>&, _Args...>::value &&
        std::is_copy_constructible<_Tp>::value>
  >
  LUL_VIS_INLINE
  _Tp& emplace(std::initializer_list<_Up>, _Args&&...);

  // 6.3.3 any modifiers
  LUL_VIS_INLINE
  void reset() noexcept { if (__h) this->__call(_Action::_Destroy); }

  LUL_VIS_INLINE
  void swap(any & __rhs) noexcept;

  // 6.3.4 any observers
  LUL_VIS_INLINE
  bool has_value() const noexcept { return __h != nullptr; }

  LUL_VIS_INLINE
  const std::type_info & type() const noexcept {
    if (__h) {
        return *static_cast<std::type_info const *>(this->__call(_Action::_TypeInfo));
    } else {
        return typeid(void);
    }
  }

private:
    typedef __any_imp::_Action _Action;
    using _HandleFuncPtr =  void* (*)(_Action, any const *, any *, const std::type_info *,
      const void* __fallback_info);

    union _Storage {
        constexpr _Storage() : __ptr(nullptr) {}
        void *  __ptr;
        __any_imp::_Buffer __buf;
    };

    LUL_VIS_ALWAYS_INLINE
    void * __call(_Action __a, any * __other = nullptr,
                  std::type_info const * __info = nullptr,
                   const void* __fallback_info = nullptr) const
    {
        return __h(__a, this, __other, __info, __fallback_info);
    }

    LUL_VIS_ALWAYS_INLINE
    void * __call(_Action __a, any * __other = nullptr,
                  std::type_info const * __info = nullptr,
                  const void* __fallback_info = nullptr)
    {
        return __h(__a, this, __other, __info, __fallback_info);
    }

    template <class>
    friend struct __any_imp::_SmallHandler;
    template <class>
    friend struct __any_imp::_LargeHandler;

    template <class _ValueType>
    friend std::add_pointer_t<std::add_const_t<_ValueType>>
    any_cast(any const *) noexcept;

    template <class _ValueType>
    friend std::add_pointer_t<_ValueType>
    any_cast(any *) noexcept;

    _HandleFuncPtr __h = nullptr;
    _Storage __s;
};

namespace __any_imp
{
  template <class _Tp>
  struct LUL_VIS_TYPE_DEFINE _SmallHandler
  {
     LUL_VIS_INLINE
     static void* __handle(_Action __act, any const * __this, any * __other,
                           std::type_info const * __info, const void* __fallback_info)
     {
        switch (__act)
        {
        case _Action::_Destroy:
          __destroy(const_cast<any &>(*__this));
          return nullptr;
        case _Action::_Copy:
            __copy(*__this, *__other);
            return nullptr;
        case _Action::_Move:
          __move(const_cast<any &>(*__this), *__other);
          return nullptr;
        case _Action::_Get:
            return __get(const_cast<any &>(*__this), __info, __fallback_info);
        case _Action::_TypeInfo:
          return __type_info();
        }
    }

    template <class ..._Args>
    LUL_VIS_INLINE
    static _Tp& __create(any & __dest, _Args&&... __args) {
        _Tp* __ret = ::new (static_cast<void*>(&__dest.__s.__buf)) _Tp(std::forward<_Args>(__args)...);
        __dest.__h = &_SmallHandler::__handle;
        return *__ret;
    }

  private:
    LUL_VIS_INLINE
    static void __destroy(any & __this) {
        _Tp & __value = *static_cast<_Tp *>(static_cast<void*>(&__this.__s.__buf));
        __value.~_Tp();
        __this.__h = nullptr;
    }

    LUL_VIS_INLINE
    static void __copy(any const & __this, any & __dest) {
        _SmallHandler::__create(__dest, *static_cast<_Tp const *>(
            static_cast<void const *>(&__this.__s.__buf)));
    }

    LUL_VIS_INLINE
    static void __move(any & __this, any & __dest) {
        _SmallHandler::__create(__dest, std::move(
            *static_cast<_Tp*>(static_cast<void*>(&__this.__s.__buf))));
        __destroy(__this);
    }

    LUL_VIS_INLINE
    static void* __get(any & __this,
                       std::type_info const * __info,
                       const void* __fallback_id)
    {
        if (__any_imp::__compare_typeid<_Tp>(__info, __fallback_id))
            return static_cast<void*>(&__this.__s.__buf);
        return nullptr;
    }

    LUL_VIS_INLINE
    static void* __type_info()
    {
        return const_cast<void*>(static_cast<void const *>(&typeid(_Tp)));
    }
  };

  template <class _Tp>
  struct LUL_VIS_TYPE_DEFINE _LargeHandler
  {
    LUL_VIS_INLINE
    static void* __handle(_Action __act, any const * __this,
                          any * __other, std::type_info const * __info,
                          void const* __fallback_info)
    {
        switch (__act)
        {
        case _Action::_Destroy:
          __destroy(const_cast<any &>(*__this));
          return nullptr;
        case _Action::_Copy:
          __copy(*__this, *__other);
          return nullptr;
        case _Action::_Move:
          __move(const_cast<any &>(*__this), *__other);
          return nullptr;
        case _Action::_Get:
            return __get(const_cast<any &>(*__this), __info, __fallback_info);
        case _Action::_TypeInfo:
          return __type_info();
        }
    }

    template <class ..._Args>
    LUL_VIS_INLINE
    static _Tp& __create(any & __dest, _Args&&... __args) {
        typedef std::allocator<_Tp> _Alloc;
        typedef details::__allocator_destructor<_Alloc> _Dp;
        _Alloc __a;
        std::unique_ptr<_Tp, _Dp> __hold(__a.allocate(1), _Dp(__a, 1));
        _Tp* __ret = ::new ((void*)__hold.get()) _Tp(std::forward<_Args>(__args)...);
        __dest.__s.__ptr = __hold.release();
        __dest.__h = &_LargeHandler::__handle;
        return *__ret;
    }

  private:

    LUL_VIS_INLINE
    static void __destroy(any & __this){
        delete static_cast<_Tp*>(__this.__s.__ptr);
        __this.__h = nullptr;
    }

    LUL_VIS_INLINE
    static void __copy(any const & __this, any & __dest) {
        _LargeHandler::__create(__dest, *static_cast<_Tp const *>(__this.__s.__ptr));
    }

    LUL_VIS_INLINE
    static void __move(any & __this, any & __dest) {
      __dest.__s.__ptr = __this.__s.__ptr;
      __dest.__h = &_LargeHandler::__handle;
      __this.__h = nullptr;
    }

    LUL_VIS_INLINE
    static void* __get(any & __this, std::type_info const * __info,
                       void const* __fallback_info)
    {
        if (__any_imp::__compare_typeid<_Tp>(__info, __fallback_info))
            return static_cast<void*>(__this.__s.__ptr);
        return nullptr;

    }

    LUL_VIS_INLINE
    static void* __type_info()
    {
        return const_cast<void*>(static_cast<void const *>(&typeid(_Tp)));
    }
  };

} // namespace __any_imp


template <class _ValueType, class _Tp, class>
any::any(_ValueType && __v) : __h(nullptr)
{
  __any_imp::_Handler<_Tp>::__create(*this, std::forward<_ValueType>(__v));
}

template <class _ValueType, class ..._Args, class _Tp, class>
any::any(std::in_place_type_t<_ValueType>, _Args&&... __args) {
  __any_imp::_Handler<_Tp>::__create(*this, std::forward<_Args>(__args)...);
};

template <class _ValueType, class _Up, class ..._Args, class _Tp, class>
any::any(std::in_place_type_t<_ValueType>, std::initializer_list<_Up> __il, _Args&&... __args) {
  __any_imp::_Handler<_Tp>::__create(*this, __il, std::forward<_Args>(__args)...);
}

template <class _ValueType, class, class>
inline LUL_VIS_INLINE
any & any::operator=(_ValueType && __v)
{
  any(std::forward<_ValueType>(__v)).swap(*this);
  return *this;
}

template <class _ValueType, class ..._Args, class _Tp, class>
inline LUL_VIS_INLINE
_Tp& any::emplace(_Args&&... __args) {
  reset();
  return __any_imp::_Handler<_Tp>::__create(*this, std::forward<_Args>(__args)...);
}

template <class _ValueType, class _Up, class ..._Args, class _Tp, class>
inline LUL_VIS_INLINE
_Tp& any::emplace(std::initializer_list<_Up> __il, _Args&&... __args) {
  reset();
  return __any_imp::_Handler<_Tp>::__create(*this, __il, std::forward<_Args>(__args)...);
}

inline LUL_VIS_INLINE
void any::swap(any & __rhs) noexcept
{
    if (this == &__rhs)
      return;
    if (__h && __rhs.__h) {
        any __tmp;
        __rhs.__call(_Action::_Move, &__tmp);
        this->__call(_Action::_Move, &__rhs);
        __tmp.__call(_Action::_Move, this);
    }
    else if (__h) {
        this->__call(_Action::_Move, &__rhs);
    }
    else if (__rhs.__h) {
        __rhs.__call(_Action::_Move, this);
    }
}

// 6.4 Non-member functions

inline LUL_VIS_INLINE
void swap(any & __lhs, any & __rhs) noexcept
{
    __lhs.swap(__rhs);
}

template <class _Tp, class ..._Args>
inline LUL_VIS_INLINE
any make_any(_Args&&... __args) {
    return any(std::in_place_type<_Tp>, std::forward<_Args>(__args)...);
}

template <class _Tp, class _Up, class ..._Args>
inline LUL_VIS_INLINE
any make_any(std::initializer_list<_Up> __il, _Args&&... __args) {
    return any(std::in_place_type<_Tp>, __il, std::forward<_Args>(__args)...);
}

template <class _ValueType>
inline LUL_VIS_INLINE
_ValueType any_cast(any const & __v)
{
    using _RawValueType = details::__uncvref_t<_ValueType>;
    static_assert(std::is_constructible<_ValueType, _RawValueType const &>::value,
                  "ValueType is required to be a const lvalue reference "
                  "or a CopyConstructible type");
    auto __tmp = any_cast<std::add_const_t<_RawValueType>>(&__v);
    if (__tmp == nullptr)
        __throw_bad_any_cast();
    return static_cast<_ValueType>(*__tmp);
}

template <class _ValueType>
inline LUL_VIS_INLINE
_ValueType any_cast(any & __v)
{
    using _RawValueType = details::__uncvref_t<_ValueType>;
    static_assert(std::is_constructible_v<_ValueType, _RawValueType &>,
                  "ValueType is required to be an lvalue reference "
                  "or a CopyConstructible type");
    auto __tmp = any_cast<_RawValueType>(&__v);
    if (__tmp == nullptr)
        __throw_bad_any_cast();
    return static_cast<_ValueType>(*__tmp);
}

template <class _ValueType>
inline LUL_VIS_INLINE
_ValueType any_cast(any && __v)
{
    using _RawValueType = details::__uncvref_t<_ValueType>;
    static_assert(std::is_constructible<_ValueType, _RawValueType>::value,
                  "ValueType is required to be an rvalue reference "
                  "or a CopyConstructible type");
    auto __tmp = any_cast<_RawValueType>(&__v);
    if (__tmp == nullptr)
        __throw_bad_any_cast();
    return static_cast<_ValueType>(std::move(*__tmp));
}

template <class _ValueType>
inline LUL_VIS_INLINE
std::add_pointer_t<std::add_const_t<_ValueType>>
any_cast(any const * __any) noexcept
{
    static_assert(!std::is_reference<_ValueType>::value,
                  "_ValueType may not be a reference.");
    return any_cast<_ValueType>(const_cast<any *>(__any));
}

template <class _RetType>
inline LUL_VIS_INLINE
_RetType __pointer_or_func_cast(void* __p, /*IsFunction*/std::false_type) noexcept {
  return static_cast<_RetType>(__p);
}

template <class _RetType>
inline LUL_VIS_INLINE
_RetType __pointer_or_func_cast(void*, /*IsFunction*/std::true_type) noexcept {
  return nullptr;
}

template <class _ValueType>
std::add_pointer_t<_ValueType>
any_cast(any * __any) noexcept
{
    using __any_imp::_Action;
    static_assert(!std::is_reference<_ValueType>::value,
                  "_ValueType may not be a reference.");
    typedef std::add_pointer_t<_ValueType> _ReturnType;
    if (__any && __any->__h) {
      void *__p = __any->__call(_Action::_Get, nullptr,
                          &typeid(_ValueType),
                          __any_imp::__get_fallback_typeid<_ValueType>());
        return __pointer_or_func_cast<_ReturnType>(
            __p, std::is_function<_ValueType>{});
    }
    return nullptr;
}

} // namespace stdproxy

LUL_end_v_namespace


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
