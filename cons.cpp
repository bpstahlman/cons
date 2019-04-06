#ifndef _CONS_H_
#define _CONS_H_

#include "cons.h"

namespace Lisp {

	template<typename T> Lisp::Cons<T>(const Cons &rhs)
		: refcnt{++rhs.refcnt}, cdr{rhs.next},
		car{rhs.car}
	{
		// Update car's refcnt
		++car->refcnt;
	}
	template<typename T> Lisp::Cons<T>(T &val, Cons &rhs)
		: refcnt{-1}, cdr{rhs.next},
		car{rhs.car}
	{
		// TODO: Check for rhs.refcnt == -1 as sign we need to create a new one on heap.
		// Also, is refcnt redundant with Car::refcnt???
		++rhs.refcnt;
	}
	Cons(const Cons &&rhs);
	Cons &operator=(const Cons &rhs);
	Cons &operator=(const Cons &&rhs);

	template<typename T> Cons<T> free_cdr()
	{
		// Walk through cdr, deleting till we get to a car or cdr with nonzero refcnt.
		// Note: We may hit used cdr before used car, but no point in continuing past a used car.
	}


	// cons primitive
	template<typename T> Cons<T> cons(T val, Cons cdr)
	{
	}
}
#endif

// vim:ts=4:sw=4:noet

