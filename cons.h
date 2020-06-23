#ifndef _CONS_H_
#define _CONS_H_

// Author Brett Pershing Stahlman
// Synopsis: Mostly pedagogic classes implementing a Lisp-style cons cell that
// can serve as the basis for singly linked lists. The cons cells are completely
// generic and use smart pointers to implement a form of "structural sharing",
// which permits the same elements to exist in multiple lists.
// Correspondences:
//     Lisp:                            C++:
//     ---------------------------------------------------------------------
//     (cons 1 (cons 2 (cons 3 nil)))   Cons<int>Nil.cons(3).cons(2).cons(1)
//     (car xs)                         xs.car()
//     (cadar xs)                       cadar(xs)
//
// Compilation: g++ -o a.out main.cpp -std=c++17

#include <memory>

using namespace std;

namespace Lisp {
	
template<typename T> class Cons;
template<typename T> class Cons_impl;

// class Cons is designed to be used in a Lisp-style list, which supports
// structural sharing through use of smart pointers to manage individual
// elements. In keeping with the Lisp paradigm, car() and cdr() methods are
// provided for accessing head and tail of list, respectively. Initial versions
// of car() and cdr() were non-member functions, but the "friend declaration"
// gymnastics entailed by this approach engendered significant obfuscation,
// which was justified more by Lisp nostalgia than by any real use case
// simplification. The only advantage I can see to the functional approach is
// that a user is probably more likely to assume (mistakenly) that...
//    foo.cons(42)
// ...modifies foo than that the following does...
//    cons(42, foo)
// Note: I discovered another advantage of the non-member function approach when
// I implemented cd*r and ca*r accessor variants (e.g., cadr, caar, cdar,
// etc...): namely, the accessors' return type declaration requires the use of
// SFINAE, which won't work for member functions.
// The Cons cell itself is immutable, in the sense that a client can't rebind it
// to a different element; however, the car() method returns a non-const ref to
// make it easy to modify the elements themselves. An alternate strategy would
// return const refs, but this would force use of const_cast in the event that a
// client wished to invoke a non-const method on a list element.
template<typename T> class Cons {
public:
	// Give client a way to cons initial element.
	// Note: We could provide a version of cons that doesn't require an existing
	// cdr; Nil is provided mostly for lisp look and feel.
	const static Cons Nil;
	// Provide access to head/tail of a cons cell.
	T& car() const;
	const Cons<T> cdr() const;
	// Provide predicate to check for empty list
	bool nil() const { return !sp; }
	// Create new cons cell using...
	// 1) existing element, 2) element to be constructed with supplied args
	Cons<T> cons(T&& val) const;
	template<typename... Args> Cons cons(Args&&... args) const;

private:
	// Ctors are private since cons cells are created only by this class.
	// Note: All lists of type T share a single static Nil element, created with
	// the default ctor.
	Cons() {}
	Cons(shared_ptr<typename Cons_impl<T>::Cons_impl_shared> sp_init)
		: sp{sp_init} {}
	// Maintain shared pointer to the actual cell containing CAR and CDR.
	// Rationale: Permits structural-sharing of list elements.
	shared_ptr<typename Cons_impl<T>::Cons_impl_shared> sp;

public:

};

// Create a single, static Nil element to be shared amongst all lists of a
// specific type.
template<typename T> const Cons<T> Cons<T>::Nil;

// Cons an element constructed from the variadic args onto returned list.
template<typename T>
template<typename... Args>
Cons<T> Cons<T>::cons(Args&&... args) const {
	return make_shared<typename Cons_impl<T>::Cons_impl_shared>(
		sp, std::forward<Args>(args)...);
}

// Cons the input element onto returned list.
template<typename T>
Cons<T> Cons<T>::cons(T&& val) const
{
	return make_shared<typename Cons_impl<T>::Cons_impl_shared>(
		std::forward<T>(val), sp);
}

// Return non-const ref to head element.
template<typename T> T& Cons<T>::car() const
{
	if (!sp)
		throw std::runtime_error {"Illegal call to car on empty list!"};
	return sp->car;
}

// Return tail of list.
// Design Decision: cdr on Nil returns Nil.
template<typename T> const Cons<T> Cons<T>::cdr() const
{
	return !sp ? sp : sp->cdr;
}

// class Cons_impl is the implementation of a Cons cell. Its ctors and all its
// member functions are private, so it can't be constructed directly by a
// client; it is meant to be constructed as the base class of member class
// Cons_impl_shared, which provides a single, public, variadic ctor designed for
// use with make_shared<>().
template<typename T> class Cons_impl {
public:
	// Give Cons<T> access to car and cdr.
	friend class Cons<T>;
	virtual ~Cons_impl() {
#ifdef DEBUG
		cout << "Cons_impl dtor(" << this << ")\n";
#endif
	}

private:
	// Declaring Cons_impl_shared private effectively hides it from clients,
	// which is important because its ctor must be public (to allow creation
	// with make_shared).
	struct Cons_impl_shared;
	// ctor used to cons an element to be constructed from variadic args
	template<typename... Args>
	Cons_impl(shared_ptr<typename Cons_impl<T>::Cons_impl_shared> cdr_init,
			Args&&... car_init)
		: car{std::forward<Args>(car_init)...}, cdr{cdr_init} {}
	// ctor used to cons already-constructed element
	Cons_impl(T&& car_init,
			shared_ptr<typename Cons_impl<T>::Cons_impl_shared> cdr_init)
		: car{std::forward<T>(car_init)}, cdr{cdr_init} {}

	T car;
	shared_ptr<typename Cons_impl<T>::Cons_impl_shared> cdr;
};

// class Cons_impl<T>::Cons_impl_shared is both a member and a subclass of
// Cons_impl<T>. As a member class, it has access to its parent class' private
// ctors, which ensures that make_shared<>() calls in Cons<T> can (indirectly)
// construct Cons_impl<> instances. Although this approach requires us to make
// Cons_impl_shared's ctor public, the fact that it's declared private within
// Cons_impl prevents clients from seeing, much less constructing it.
// Caveat: Definition must appear outside containing class to prevent incomplete
// class warning/error.
template <typename T>
struct Cons_impl<T>::Cons_impl_shared : public Cons_impl<T> {
	// Caveat: ctor must be public to permit creation by Cons<T>.
	template <typename... Args> Cons_impl_shared(Args&&... args)
	: Cons_impl(forward<Args>(args)...) {}
};

// Overload operator<< for lists.
// TODO: Consider adding iterators for lists, keeping in mind that they're not
// particularly lispy.
template<typename T>
ostream& operator<<(ostream& os, Cons<T> xs)
{
	bool comma = false; // Skip leading comma
	cout << '[';
	for (; !xs.nil(); xs = xs.cdr()) {
		comma && (os << ", ") || (comma = true);
		os << xs.car();
	}
	cout << ']';
	return os;
}

// Provide inline versions of all the ca*r and cd*r accessors provided by
// Common Lisp: e.g., caar, cadr, caaar, etc..
// TODO: Consider implementing car() and cdr() as non-member template functions
// as well, for sake of consistency.
// Note: The reason ca*r and cd*r functions *cannot* be implemented as member
// functions is that SFINAE is needed to deduce return type resulting from a
// string of car/cdr applications, and SFINAE won't work for unconditionally
// instantiated member functions.
// Rationale: They're easy to create by applying this Vim substitution...
// Vim Substitution (applied to the list of accessors below):
// Note: Write it to a tmp file and source to avoid problems when multiple lines
// are pasted into Vim cmdline.
/*
'<,'>s@\v%(\s*%(//)?\s*)c([ad]+)r,?\s*@\=
	\ "template<typename T>\nauto"
	\ . ' c' . submatch(1) . 'r(Cons<T> xs)'
	\ . "\n\t-> decltype(std::declval<Cons<T>>()."
	\     . join(map(reverse(split(submatch(1), '\zs')),
	\            'v:val == "a" ? "car()" : "cdr()"'), '.')
	\ . ")\n{ return xs."
	\ . join(map(reverse(split(submatch(1), '\zs')),
		\ 'v:val == "a" ? "car()" : "cdr()"'), '.')
	\ . "; }\n"@g
*/
// Apply the Vim substitute above to the following lines (copied from CL
// hyperspec) to create accessor template functions:
// caar, cadr, caaar, caadr, cadar, caddr, caaaar,
// caaadr, caadar, caaddr, cadaar, cadadr, caddar, cadddr
// cdar, cddr, cdaar, cdadr, cddar, cdddr, cdaaar,
// cdaadr, cdadar, cdaddr, cddaar, cddadr, cdddar, cddddr
// -- BEGIN AUTOGENERATED FUNCTIONS --
template<typename T>
auto caar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car())
{ return xs.car().car(); }
template<typename T>
auto cadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car())
{ return xs.cdr().car(); }
template<typename T>
auto caaar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car().car())
{ return xs.car().car().car(); }
template<typename T>
auto caadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car().car())
{ return xs.cdr().car().car(); }
template<typename T>
auto cadar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr().car())
{ return xs.car().cdr().car(); }
template<typename T>
auto caddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr().car())
{ return xs.cdr().cdr().car(); }
template<typename T>
auto caaaar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car().car().car())
{ return xs.car().car().car().car(); }
template<typename T>
auto caaadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car().car().car())
{ return xs.cdr().car().car().car(); }
template<typename T>
auto caadar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr().car().car())
{ return xs.car().cdr().car().car(); }
template<typename T>
auto caaddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr().car().car())
{ return xs.cdr().cdr().car().car(); }
template<typename T>
auto cadaar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car().cdr().car())
{ return xs.car().car().cdr().car(); }
template<typename T>
auto cadadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car().cdr().car())
{ return xs.cdr().car().cdr().car(); }
template<typename T>
auto caddar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr().cdr().car())
{ return xs.car().cdr().cdr().car(); }
template<typename T>
auto cadddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr().cdr().car())
{ return xs.cdr().cdr().cdr().car(); }
template<typename T>
auto cdar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr())
{ return xs.car().cdr(); }
template<typename T>
auto cddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr())
{ return xs.cdr().cdr(); }
template<typename T>
auto cdaar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car().cdr())
{ return xs.car().car().cdr(); }
template<typename T>
auto cdadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car().cdr())
{ return xs.cdr().car().cdr(); }
template<typename T>
auto cddar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr().cdr())
{ return xs.car().cdr().cdr(); }
template<typename T>
auto cdddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr().cdr())
{ return xs.cdr().cdr().cdr(); }
template<typename T>
auto cdaaar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car().car().cdr())
{ return xs.car().car().car().cdr(); }
template<typename T>
auto cdaadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car().car().cdr())
{ return xs.cdr().car().car().cdr(); }
template<typename T>
auto cdadar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr().car().cdr())
{ return xs.car().cdr().car().cdr(); }
template<typename T>
auto cdaddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr().car().cdr())
{ return xs.cdr().cdr().car().cdr(); }
template<typename T>
auto cddaar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().car().cdr().cdr())
{ return xs.car().car().cdr().cdr(); }
template<typename T>
auto cddadr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().car().cdr().cdr())
{ return xs.cdr().car().cdr().cdr(); }
template<typename T>
auto cdddar(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().car().cdr().cdr().cdr())
{ return xs.car().cdr().cdr().cdr(); }
template<typename T>
auto cddddr(Cons<T> xs)
	-> decltype(std::declval<Cons<T>>().cdr().cdr().cdr().cdr())
{ return xs.cdr().cdr().cdr().cdr(); }

// -- END AUTOGENERATED FUNCTIONS --

}
#endif

// vim:ts=4:sw=4:noet:cino=N-s

