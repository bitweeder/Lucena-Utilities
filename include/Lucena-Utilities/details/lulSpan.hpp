/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Span.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of the libc++ 7.0 <span> implementation,
	used under the University of Illinois/NCSA Open Source License. See
	“license/libc++ License” for details.

	The notable differences from the libc++ version include:

	* namespace changes
	* removal of some unneeded preprocessor tests and attributes
	* renaming of various macros to use LUL versions
	* renaming of system-reserved symbols

------------------------------------------------------------------------------*/


#pragma once

#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif


/*
    span synopsis

	namespace stdproxy {

	// constants
	inline constexpr ptrdiff_t dynamic_extent = -1;

	// [views.span], class template span
	template <class ElementType, ptrdiff_t Extent = dynamic_extent>
		class span;

	// [span.comparison], span comparison operators
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
		constexpr bool operator==(span<T, X> l, span<U, Y> r);
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
		constexpr bool operator!=(span<T, X> l, span<U, Y> r);
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
		constexpr bool operator<(span<T, X> l, span<U, Y> r);
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
		constexpr bool operator<=(span<T, X> l, span<U, Y> r);
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
		constexpr bool operator>(span<T, X> l, span<U, Y> r);
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
		constexpr bool operator>=(span<T, X> l, span<U, Y> r);

	// [span.objectrep], views of object representation
	template <class ElementType, ptrdiff_t Extent>
		span<const byte, ((Extent == dynamic_extent) ? dynamic_extent :
			(static_cast<ptrdiff_t>(sizeof(ElementType)) * Extent))> as_bytes(span<ElementType, Extent> s) noexcept;

	template <class ElementType, ptrdiff_t Extent>
		span<      byte, ((Extent == dynamic_extent) ? dynamic_extent :
			(static_cast<ptrdiff_t>(sizeof(ElementType)) * Extent))> as_writable_bytes(span<ElementType, Extent> s) noexcept;


	template <class ElementType, ptrdiff_t Extent = dynamic_extent>
	class span {
	public:
		// constants and types
		using element_type = ElementType;
		using value_type = remove_cv_t<ElementType>;
		using index_type = ptrdiff_t;
		using difference_type = ptrdiff_t;
		using pointer = element_type*;
		using reference = element_type&;
		using iterator = implementation-defined;
		using const_iterator = implementation-defined;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		static constexpr index_type extent = Extent;

		// [span.cons], span constructors, copy, assignment, and destructor
		constexpr span() noexcept;
		constexpr span(pointer ptr, index_type count);
		constexpr span(pointer firstElem, pointer lastElem);
		template <size_t N>
			constexpr span(element_type (&arr)[N]) noexcept;
		template <size_t N>
			constexpr span(array<value_type, N>& arr) noexcept;
		template <size_t N>
			constexpr span(const array<value_type, N>& arr) noexcept;
		template <class Container>
			constexpr span(Container& cont);
		template <class Container>
			constexpr span(const Container& cont);
		constexpr span(const span& other) noexcept = default;
		template <class OtherElementType, ptrdiff_t OtherExtent>
			constexpr span(const span<OtherElementType, OtherExtent>& s) noexcept;
		~span() noexcept = default;
		constexpr span& operator=(const span& other) noexcept = default;

		// [span.sub], span subviews
		template <ptrdiff_t Count>
			constexpr span<element_type, Count> first() const;
		template <ptrdiff_t Count>
			constexpr span<element_type, Count> last() const;
		template <ptrdiff_t Offset, ptrdiff_t Count = dynamic_extent>
			constexpr span<element_type, see below> subspan() const;

		constexpr span<element_type, dynamic_extent> first(index_type count) const;
		constexpr span<element_type, dynamic_extent> last(index_type count) const;
		constexpr span<element_type, dynamic_extent> subspan(index_type offset, index_type count = dynamic_extent) const;

		// [span.obs], span observers
		constexpr index_type size() const noexcept;
		constexpr index_type size_bytes() const noexcept;
		constexpr bool empty() const noexcept;

		// [span.elem], span element access
		constexpr reference operator[](index_type idx) const;
		constexpr reference operator()(index_type idx) const;
		constexpr pointer data() const noexcept;

		// [span.iterators], span iterator support
		constexpr iterator begin() const noexcept;
		constexpr iterator end() const noexcept;
		constexpr const_iterator cbegin() const noexcept;
		constexpr const_iterator cend() const noexcept;
		constexpr reverse_iterator rbegin() const noexcept;
		constexpr reverse_iterator rend() const noexcept;
		constexpr const_reverse_iterator crbegin() const noexcept;
		constexpr const_reverse_iterator crend() const noexcept;

	private:
		pointer data_;     // exposition only
		index_type size_;  // exposition only
	};

	template<class T, size_t N>
		span(T (&)[N]) -> span<T, N>;

	template<class T, size_t N>
		span(array<T, N>&) -> span<T, N>;

	template<class T, size_t N>
		span(const array<T, N>&) -> span<const T, N>;

	template<class Container>
		span(Container&) -> span<typename Container::value_type>;

	template<class Container>
		span(const Container&) -> span<const typename Container::value_type>;

	} // namespace stdproxy
*/


//	std
#include <cstddef>
#include <iterator>
#include <array>
#include <type_traits>
#include <cstddef>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulHelperIterator.hpp>
#include <Lucena-Utilities/details/lulVisibility.hpp>


LUL_begin_v_namespace

namespace stdproxy {

inline constexpr ptrdiff_t dynamic_extent = -1;
template <typename _Tp, ptrdiff_t _Extent = dynamic_extent> class span;


template <class _Tp>
struct __is_span_impl : public std::false_type {};

template <class _Tp, ptrdiff_t _Extent>
struct __is_span_impl<span<_Tp, _Extent>> : public std::true_type {};

template <class _Tp>
struct __is_span : public __is_span_impl<std::remove_cv_t<_Tp>> {};

template <class _Tp>
struct __is_std_array_impl : public std::false_type {};

template <class _Tp, size_t _Sz>
struct __is_std_array_impl<std::array<_Tp, _Sz>> : public std::true_type {};

template <class _Tp>
struct __is_std_array : public __is_std_array_impl<std::remove_cv_t<_Tp>> {};

template <class _Tp, class _ElementType, class = void>
struct __is_span_compatible_container : public std::false_type {};

template <class _Tp, class _ElementType>
struct __is_span_compatible_container<_Tp, _ElementType,
        std::void_t<
        // is not a specialization of span
            typename std::enable_if<!__is_span<_Tp>::value, std::nullptr_t>::type,
        // is not a specialization of array
            typename std::enable_if<!__is_std_array<_Tp>::value, std::nullptr_t>::type,
        // is_array_v<Container> is false,
            typename std::enable_if<!std::is_array_v<_Tp>, std::nullptr_t>::type,
        // data(cont) and size(cont) are well formed
            decltype(data(std::declval<_Tp>())),
            decltype(size(std::declval<_Tp>())),
        // remove_pointer_t<decltype(data(cont))>(*)[] is convertible to ElementType(*)[]
            typename std::enable_if<
				std::is_convertible_v<std::remove_pointer_t<decltype(data(std::declval<_Tp &>()))>(*)[],
				                 _ElementType(*)[]>,
                std::nullptr_t>::type
        >>
    : public std::true_type {};


template <typename _Tp, ptrdiff_t _Extent>
class LUL_VIS_CLASS_TEMPLATE span {
public:
//  constants and types
    using element_type           = _Tp;
    using value_type             = std::remove_cv_t<_Tp>;
    using index_type             = ptrdiff_t;
    using difference_type        = ptrdiff_t;
    using pointer                = _Tp *;
    using const_pointer          = const _Tp *; // not in standard
    using reference              = _Tp &;
    using const_reference        = const _Tp &; // not in standard
    using iterator               =  details::__wrap_iter<pointer>;
    using const_iterator         =  details::__wrap_iter<const_pointer>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr index_type extent = _Extent;
    static_assert (_Extent >= 0, "Can't have a span with an extent < 0");

// [span.cons], span constructors, copy, assignment, and destructor
    LUL_VIS_INLINE_FUNC constexpr span() noexcept : __data{nullptr}
    { static_assert(_Extent == 0, "Can't default construct a statically sized span with size > 0"); }

    constexpr span           (const span&) noexcept = default;
    constexpr span& operator=(const span&) noexcept = default;

    LUL_VIS_INLINE_FUNC constexpr span(pointer __ptr, index_type __count) : __data{__ptr}
        { (void)__count;
        //	C++2a
//      [[assert: _Extent == __count]];
		}
    LUL_VIS_INLINE_FUNC constexpr span(pointer __f, pointer __l) : __data{__f}
        { (void)__l;
        //	C++2a
//      [[assert: _Extent == std::distance(__f, __l)]];
		}

    LUL_VIS_INLINE_FUNC constexpr span(element_type (&__arr)[_Extent])          noexcept : __data{__arr} {}
    LUL_VIS_INLINE_FUNC constexpr span(      std::array<value_type, _Extent>& __arr) noexcept : __data{__arr.data()} {}
    LUL_VIS_INLINE_FUNC constexpr span(const std::array<value_type, _Extent>& __arr) noexcept : __data{__arr.data()} {}

    template <class _Container>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(      _Container& __c,
            std::enable_if_t<__is_span_compatible_container<_Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}
        {
        //	FIXME C++2a illegal unless size(__c) is constexpr
//      [[assert: _Extent == std::size(__c)]];
		}

    template <class _Container>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(const _Container& __c,
            std::enable_if_t<__is_span_compatible_container<const _Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}
        {
        //	FIXME C++2a illegal unless size(__c) is constexpr
//      [[assert: _Extent == std::size(__c)]];
		}

    template <class _OtherElementType>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(const span<_OtherElementType, _Extent>& __other,
                       std::enable_if_t<
                          std::is_convertible_v<_OtherElementType(*)[], element_type (*)[]>,
                          std::nullptr_t> = nullptr)
        : __data{__other.data()} {}

    template <class _OtherElementType>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(const span<_OtherElementType, dynamic_extent>& __other,
                       std::enable_if_t<
                          std::is_convertible_v<_OtherElementType(*)[], element_type (*)[]>,
                          std::nullptr_t> = nullptr) noexcept
        : __data{__other.data()}
        {
        //	C++2a
//      [[assert: _Extent == __other.size()]];
		}


//  ~span() noexcept = default;

    template <ptrdiff_t _Count>
    inline LUL_VIS_INLINE_FUNC
        constexpr span<element_type, _Count> first() const noexcept
    {
        static_assert(_Count >= 0, "Count must be >= 0 in span::first()");
        static_assert(_Count <= _Extent, "Count out of range in span::first()");
        return {data(), _Count};
    }

    template <ptrdiff_t _Count>
    inline LUL_VIS_INLINE_FUNC
        constexpr span<element_type, _Count> last() const noexcept
    {
        static_assert(_Count >= 0, "Count must be >= 0 in span::last()");
        static_assert(_Count <= _Extent, "Count out of range in span::last()");
        return {data() + size() - _Count, _Count};
    }

    LUL_VIS_INLINE_FUNC
    constexpr span<element_type, dynamic_extent> first(index_type __count) const noexcept
    {
		//	C++2a
//      [[assert: __count >= 0 && __count <= size()]];
        return {data(), __count};
    }

    LUL_VIS_INLINE_FUNC
    constexpr span<element_type, dynamic_extent> last(index_type __count) const noexcept
    {
		//	C++2a
//      [[assert: __count >= 0 && __count <= size()]];
        return {data() + size() - __count, __count};
    }

    template <ptrdiff_t _Offset, ptrdiff_t _Count = dynamic_extent>
    inline LUL_VIS_INLINE_FUNC
        constexpr auto subspan() const noexcept
        -> span<element_type, _Count != dynamic_extent ? _Count : _Extent - _Offset>
    {
		//	C++2a
//      [[assert: _Offset >= 0 && _Offset <= size()]];
        return {data() + _Offset, _Count == dynamic_extent ? size() - _Offset : _Count};
    }


    inline LUL_VIS_INLINE_FUNC
    constexpr span<element_type, dynamic_extent>
       subspan(index_type __offset, index_type __count = dynamic_extent) const noexcept
    {
		//	C++2a
//      [[assert: _Offset >= 0 && _Offset <= size()]];
//      [[assert: __count >= 0 && __count <= size()]];
        if (__count == dynamic_extent)
            return {data() + __offset, size() - __offset};
		//	C++2a
//      [[assert: __offset + __count <= size()]];
        return {data() + __offset, __count};
    }

    LUL_VIS_INLINE_FUNC constexpr index_type size()       const noexcept { return _Extent; }
    LUL_VIS_INLINE_FUNC constexpr index_type size_bytes() const noexcept { return _Extent * sizeof(element_type); }
    LUL_VIS_INLINE_FUNC constexpr bool empty()            const noexcept { return _Extent == 0; }

    LUL_VIS_INLINE_FUNC constexpr reference operator[](index_type __idx) const noexcept
    {
		//	C++2a
//      [[assert: __idx >= 0 && __idx < size()]];
		return __data[__idx];
	}

    LUL_VIS_INLINE_FUNC constexpr reference operator()(index_type __idx) const noexcept
    {
		//	C++2a
//      [[assert: __idx >= 0 && __idx < size()]];
		return __data[__idx];
	}

    LUL_VIS_INLINE_FUNC constexpr pointer data()                         const noexcept { return __data; }

// [span.iter], span iterator support
    LUL_VIS_INLINE_FUNC constexpr iterator                 begin() const noexcept { return iterator(data()); }
    LUL_VIS_INLINE_FUNC constexpr iterator                   end() const noexcept { return iterator(data() + size()); }
    LUL_VIS_INLINE_FUNC constexpr const_iterator          cbegin() const noexcept { return const_iterator(data()); }
    LUL_VIS_INLINE_FUNC constexpr const_iterator            cend() const noexcept { return const_iterator(data() + size()); }
    LUL_VIS_INLINE_FUNC constexpr reverse_iterator        rbegin() const noexcept { return reverse_iterator(end()); }
    LUL_VIS_INLINE_FUNC constexpr reverse_iterator          rend() const noexcept { return reverse_iterator(begin()); }
    LUL_VIS_INLINE_FUNC constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    LUL_VIS_INLINE_FUNC constexpr const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(cbegin()); }

    LUL_VIS_INLINE_FUNC constexpr void swap(span &__other) noexcept
    {
        pointer __p = __data;
        __data = __other.__data;
        __other.__data = __p;
    }

    LUL_VIS_INLINE_FUNC span<const std::byte, _Extent * sizeof(element_type)> __as_bytes() const noexcept
    { return {reinterpret_cast<const std::byte *>(data()), size_bytes()}; }

    LUL_VIS_INLINE_FUNC span<std::byte, _Extent * sizeof(element_type)> __as_writeable_bytes() const noexcept
    { return {reinterpret_cast<std::byte *>(data()), size_bytes()}; }

private:
    pointer    __data;

};


template <typename _Tp>
class LUL_VIS_CLASS_TEMPLATE span<_Tp, dynamic_extent> {
private:

public:
//  constants and types
    using element_type           = _Tp;
    using value_type             = std::remove_cv_t<_Tp>;
    using index_type             = ptrdiff_t;
    using difference_type        = ptrdiff_t;
    using pointer                = _Tp *;
    using const_pointer          = const _Tp *; // not in standard
    using reference              = _Tp &;
    using const_reference        = const _Tp &; // not in standard
    using iterator               =  details::__wrap_iter<pointer>;
    using const_iterator         =  details::__wrap_iter<const_pointer>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr index_type extent = dynamic_extent;

// [span.cons], span constructors, copy, assignment, and destructor
    LUL_VIS_INLINE_FUNC constexpr span() noexcept : __data{nullptr}, __size{0} {}

    constexpr span           (const span&) noexcept = default;
    constexpr span& operator=(const span&) noexcept = default;

    LUL_VIS_INLINE_FUNC constexpr span(pointer __ptr, index_type __count) : __data{__ptr}, __size{__count} {}
    LUL_VIS_INLINE_FUNC constexpr span(pointer __f, pointer __l) : __data{__f}, __size{distance(__f, __l)} {}

    template <size_t _Sz>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(element_type (&__arr)[_Sz]) noexcept : __data{__arr}, __size{_Sz} {}

    template <size_t _Sz>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(std::array<value_type, _Sz>& __arr)       noexcept : __data{__arr.data()}, __size{_Sz} {}

    template <size_t _Sz>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(const std::array<value_type, _Sz>& __arr) noexcept : __data{__arr.data()}, __size{_Sz} {}

    template <class _Container>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(      _Container& __c,
            std::enable_if_t<__is_span_compatible_container<_Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}, __size{(index_type) std::size(__c)} {}

    template <class _Container>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(const _Container& __c,
            std::enable_if_t<__is_span_compatible_container<const _Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}, __size{(index_type) std::size(__c)} {}


    template <class _OtherElementType, ptrdiff_t _OtherExtent>
    inline LUL_VIS_INLINE_FUNC
        constexpr span(const span<_OtherElementType, _OtherExtent>& __other,
                       std::enable_if_t<
                          std::is_convertible_v<_OtherElementType(*)[], element_type (*)[]>,
                          std::nullptr_t> = nullptr) noexcept
        : __data{__other.data()}, __size{__other.size()} {}

//    ~span() noexcept = default;

    template <ptrdiff_t _Count>
    inline LUL_VIS_INLINE_FUNC
        constexpr span<element_type, _Count> first() const noexcept
    {
        static_assert(_Count >= 0);
		//	C++2a
//      [[assert: _Count <= size()]];
        return {data(), _Count};
    }

    template <ptrdiff_t _Count>
    inline LUL_VIS_INLINE_FUNC
        constexpr span<element_type, _Count> last() const noexcept
    {
        static_assert(_Count >= 0);
		//	C++2a
//      [[assert: _Count <= size()]];
        return {data() + size() - _Count, _Count};
    }

    LUL_VIS_INLINE_FUNC
    constexpr span<element_type, dynamic_extent> first(index_type __count) const noexcept
    {
		//	C++2a
//      [[assert: __count >= 0 && __count <= size()]];
         return {data(), __count};
    }

    LUL_VIS_INLINE_FUNC
    constexpr span<element_type, dynamic_extent> last (index_type __count) const noexcept
    {
		//	C++2a
//      [[assert: __count >= 0 && __count <= size()]];
        return {data() + size() - __count, __count};
    }

    template <ptrdiff_t _Offset, ptrdiff_t _Count = dynamic_extent>
    inline LUL_VIS_INLINE_FUNC
        constexpr span<_Tp, dynamic_extent> subspan() const noexcept
    {
		//	C++2a
//      [[assert: _Offset >= 0 && _Offset <= size()]];
//      [[assert: _Count == dynamic_extent || _Offset + _Count <= size()]];
        return {data() + _Offset, _Count == dynamic_extent ? size() - _Offset : _Count};
    }

    constexpr span<element_type, dynamic_extent>
    inline LUL_VIS_INLINE_FUNC
       subspan(index_type __offset, index_type __count = dynamic_extent) const noexcept
    {
		//	C++2a
//      [[assert: _Offset >= 0 && _Offset <= size()]];
//      [[assert: (__count  >= 0 && __count  <= size()) || __count == dynamic_extent]];
        if (__count == dynamic_extent)
            return {data() + __offset, size() - __offset};
		//	C++2a
//      [[assert: __offset + __count <= size()]];
        return {data() + __offset, __count};
    }

    LUL_VIS_INLINE_FUNC constexpr index_type size()       const noexcept { return __size; }
    LUL_VIS_INLINE_FUNC constexpr index_type size_bytes() const noexcept { return __size * sizeof(element_type); }
    LUL_VIS_INLINE_FUNC constexpr bool empty()            const noexcept { return __size == 0; }

    LUL_VIS_INLINE_FUNC constexpr reference operator[](index_type __idx) const noexcept
    {
		//	C++2a
//      [[assert: __idx >= 0 && __idx < size()]];
		return __data[__idx];
	}

    LUL_VIS_INLINE_FUNC constexpr reference operator()(index_type __idx) const noexcept
    {
		//	C++2a
//      [[assert: __idx >= 0 && __idx < size()]];
		return __data[__idx];
	}

    LUL_VIS_INLINE_FUNC constexpr pointer data()                         const noexcept { return __data; }

// [span.iter], span iterator support
    LUL_VIS_INLINE_FUNC constexpr iterator                 begin() const noexcept { return iterator(data()); }
    LUL_VIS_INLINE_FUNC constexpr iterator                   end() const noexcept { return iterator(data() + size()); }
    LUL_VIS_INLINE_FUNC constexpr const_iterator          cbegin() const noexcept { return const_iterator(data()); }
    LUL_VIS_INLINE_FUNC constexpr const_iterator            cend() const noexcept { return const_iterator(data() + size()); }
    LUL_VIS_INLINE_FUNC constexpr reverse_iterator        rbegin() const noexcept { return reverse_iterator(end()); }
    LUL_VIS_INLINE_FUNC constexpr reverse_iterator          rend() const noexcept { return reverse_iterator(begin()); }
    LUL_VIS_INLINE_FUNC constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    LUL_VIS_INLINE_FUNC constexpr const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(cbegin()); }

    LUL_VIS_INLINE_FUNC constexpr void swap(span &__other) noexcept
    {
        pointer __p = __data;
        __data = __other.__data;
        __other.__data = __p;

        index_type __sz = __size;
        __size = __other.__size;
        __other.__size = __sz;
    }

    LUL_VIS_INLINE_FUNC span<const std::byte, dynamic_extent> __as_bytes() const noexcept
    { return {reinterpret_cast<const std::byte *>(data()), size_bytes()}; }

    LUL_VIS_INLINE_FUNC span<std::byte, dynamic_extent> __as_writeable_bytes() const noexcept
    { return {reinterpret_cast<std::byte *>(data()), size_bytes()}; }

private:
    pointer    __data;
    index_type __size;
};

template <class _Tp1, ptrdiff_t _Extent1, class _Tp2, ptrdiff_t _Extent2>
    constexpr bool
    operator==(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return std::equal(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end()); }

template <class _Tp1, ptrdiff_t _Extent1, class _Tp2, ptrdiff_t _Extent2>
    constexpr bool
    operator!=(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return !(__rhs == __lhs); }

template <class _Tp1, ptrdiff_t _Extent1, class _Tp2, ptrdiff_t _Extent2>
    constexpr bool
    operator< (const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return std::lexicographical_compare (__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end()); }

template <class _Tp1, ptrdiff_t _Extent1, class _Tp2, ptrdiff_t _Extent2>
    constexpr bool
    operator<=(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return !(__rhs < __lhs); }

template <class _Tp1, ptrdiff_t _Extent1, class _Tp2, ptrdiff_t _Extent2>
    constexpr bool
    operator> (const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return __rhs < __lhs; }

template <class _Tp1, ptrdiff_t _Extent1, class _Tp2, ptrdiff_t _Extent2>
    constexpr bool
    operator>=(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return !(__lhs < __rhs); }

//  as_bytes & as_writeable_bytes
template <class _Tp, ptrdiff_t _Extent>
    auto as_bytes(span<_Tp, _Extent> __s) noexcept
    -> decltype(__s.__as_bytes())
    { return __s.__as_bytes(); }

template <class _Tp, ptrdiff_t _Extent>
    auto as_writeable_bytes(span<_Tp, _Extent> __s) noexcept
    -> typename std::enable_if<!std::is_const_v<_Tp>, decltype(__s.__as_writeable_bytes())>::type
    { return __s.__as_writeable_bytes(); }

template <class _Tp, ptrdiff_t _Extent>
    constexpr void swap(span<_Tp, _Extent> &__lhs, span<_Tp, _Extent> &__rhs) noexcept
    { __lhs.swap(__rhs); }


//  Deduction guides
template<class _Tp, size_t _Sz>
    span(_Tp (&)[_Sz]) -> span<_Tp, _Sz>;

template<class _Tp, size_t _Sz>
    span(std::array<_Tp, _Sz>&) -> span<_Tp, _Sz>;

template<class _Tp, size_t _Sz>
    span(const std::array<_Tp, _Sz>&) -> span<const _Tp, _Sz>;

template<class _Container>
    span(_Container&) -> span<typename _Container::value_type>;

template<class _Container>
    span(const _Container&) -> span<const typename _Container::value_type>;

} // namespace stdproxy

LUL_end_v_namespace


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
