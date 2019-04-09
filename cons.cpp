
#include <memory>
#include "cons.h"

template<typename T> Lisp::Cons<T> Lisp::Cons<T>::nil{};

template<typename T> Lisp::Cons<T>::Cons(T &val, shared_ptr<Cons<T> > sp_cdr)
	: cdr{sp_cdr}, car{new T{val}}, self{nullptr}
{

}

template<typename T> Lisp::Cons<T>::Cons(const Cons<T> &rhs)
	: car{rhs.car}, cdr{rhs.cdr}, self{nullptr}
{
}

template<typename T>
Lisp::Cons<T> &Lisp::Cons<T>::operator=(const Cons<T> &rhs)
{
	car = rhs.car;
	cdr = rhs.cdr;
	// Assumption: lhs of assignment is never heap instance (though its car
	// and cdr are).
	self = nullptr;
}

// cons primitive
// TODO: Add support for rhs refs later...
template<typename T> Lisp::Cons<T> Lisp::cons(T &val, Cons<T> cdr)
{
	Cons<T> cell{new T{val}, cdr.self
		// Re-use existing shared_ptr to a Cons cell already on heap.
		? *cdr.self
		// Allocate the cons cell on the heap and wrap in shared_ptr.
		: shared_ptr<Cons<T> >(new Cons<T>(cdr))};
	// Give the newly-allocated cons cell a *pointer to* the only
	// shared_ptr now referencing it.
	// Rationale: Allows us to initialize other shared_ptr's later.
	cdr.self = &cell.cdr;
	return cell;
}

// vim:ts=4:sw=4:noet

