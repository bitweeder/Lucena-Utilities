/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“MemoryTypes.hpp”
	Copyright © 2000-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <array>
#include <memory>
#include <tuple>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulTypes.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	null_deleter

	A do-nothing class that can be passed in as the deleter for a smart pointer
	when you don’t want its destructor to actually delete anything.
*/

template <class T>
struct null_deleter
{
	public:
		void					operator() (T const *) const noexcept { }
};


/*------------------------------------------------------------------------------
	raw_deleter

	A special class for handling overload resolution to ::operator delete when
	attempting to pass ::operator delete (void *) as a function argument. This
	is needed in order for smart pointers whose objects are allocated with
	::operator new (std::size_t).
*/

template <class T>
struct raw_deleter
{
	public:
		void					operator () (T * in_data) const noexcept
								{ ::operator delete (in_data); }
};


/*------------------------------------------------------------------------------
	concrete_deleter

	A stateful deleter that can be passed in as the deleter for a unique_ptr
	when you want to allow polymorphic deletion without relying on virtual
	destructors. Type erasure is used to create an operation that doesn’t care
	what it operates on as long as the signature is right, but static typing is
	in place in the background so that the compiler can prevent misuse.
	
	See make_unique_poly below.
	
	SEEME Admittedly, I have almost no idea why this would ever be useful - if
	you want polymorphic deletion, just defined the damned virtual destructor,
	already. This seems like a brittle solution to the problem of an unwanted
	vtable.
*/

struct concrete_deleter
{
	using del_t = void(*)(void*);
	del_t _del;

	template <typename T>
	static void Delete_It (void * p)
	{
		delete static_cast <T *> (p);
	}

	template <typename T>
	concrete_deleter (T *)
		:	_del (&Delete_It <T>)
	{ }

	void operator() (void * ptr) const
	{
		(*_del) (ptr);
	}
};
 

/*------------------------------------------------------------------------------
	Type Definitions
*/

using SizedRawBuffer = std::tuple <const char *, std::size_t>;

using Buffer = std::unique_ptr <char, raw_deleter <char>>;
using SizedBuffer = std::tuple <Buffer, std::size_t>;

using SharedBuffer = std::shared_ptr <char>;
using SizedSharedBuffer = std::tuple <SharedBuffer, std::size_t>;


/*------------------------------------------------------------------------------
	pimpl

	Handle-body implementation as per Herb Sutter, with modifications.

	The templated constructor fills the role of a move constructor as well as
	supporting general constructors for T. The templated assignment operator
	is to specifically support move-assignment. These are provided to ensure
	that special member functions behave as expected.

	SEEME C++11 Note that in practice, this template serves almost no purpose
	at all. The key issue is that any class hidden behind a pimpl will be
	incomplete at the point of deletion unless the parent class destructor is
	explicitly defined in an implementation file somewhere (defaulting it at
	the point of user declaration is insufficient unless the full declaration
	of the hidden class is given, which would probably defeat the purpose).
	Once the destructor has been user-declared, the move operations must also
	be user-declared, and hence defined, in the same translation unit as the
	class destructor (assuming we want move operations). In turn, the default
	constructor would also have to be explicitly defined since we’re now
	declaring constructors. The only special member functions that we don’t
	have to worry about are the copy functions, since they won’t be implicitly
	generated and we probably don’t need them anyway. The cascade continues
	since move constructors in any inheriting classes aren’t implicitly
	defined, so we still end up having to define those, which means we have to
	define the default constructor as well. This part, at least, can be avoided
	if we simply write off move functions. Of course, none of this is avoided
	if we don’t use this class, but all we’ve really accomplished by using it
	is that we don’t need to explicitly initialize our unique_ptr, assuming the
	default constructor is sufficient.
*/

template <typename T>
class pimpl
{
	public:
								pimpl()
									:	_m {std::make_unique <T>()}			{ }

		template <typename ...Args>
								pimpl (
									Args &&					...args)
									:	_m {std::make_unique <T> (args)...}	{ }

								~pimpl()									{ }

		template <typename U>
		pimpl &					operator = (
									U &&					other)
								{ return _m->operator = (std::forward <U> (other)); }

		T *						operator ->()				{ return _m.get(); }
		const T *				operator ->() const			{ return _m.get(); }
		T &						operator *()				{ return *_m.get();}
		const T &				operator *() const			{ return *_m.get();}

	private:
		std::unique_ptr <T>		_m;
};


/*------------------------------------------------------------------------------
	make_unique_poly
*/

template <typename T, typename... Args>
auto make_unique_poly (Args &&... args)
{
	return std::unique_ptr <T, concrete_deleter> {
		new T {std::forward <Args> (args)...}, static_cast <T *> (nullptr)};
}
 
//	usage, where A is a (static) base class of B
//std::unique_ptr <A, concrete_deleter> ptr = make_unique_poly <B>();


/*------------------------------------------------------------------------------
	make_buffer

	Create a scoped buffer of the specified size that auto-deletes once it goes
	out of scope. This version creates the buffer on the heap.

	APIME A more sophisticated implementation would accept a template parameter
	specifying an allocator to use, relying on type aliases to maintain
	simplicity while still offering the control of a custom memory allocator.

	SEEME C++14 Note that the raw_deleter function obj cannot currently be
	replaced by a pure lambda very easily since its type must be known to be
	provided as a template parameter for unique_ptr. A workaround would be to
	declare an auto variable holding the lambda and refer to that, but this
	approach may interfere with inlining.
*/

inline
Buffer
make_buffer (
	std::size_t in_size)
{
	return Buffer {
		reinterpret_cast <char *> (::operator new (in_size)),
			raw_deleter <char>{}};
}

inline
SizedBuffer
make_sized_buffer (
	std::size_t in_size)
{
	return std::make_tuple (make_buffer (in_size), in_size);
}


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
