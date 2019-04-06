#ifndef _CONS_H_
#define _CONS_H_

namespace Lisp {
	
	template<typename T> class Car {
	public:
		// Allow access to refcnt.
		friend class Cons<T>;
		Car(T *p) : ptr{p}, refcnt{p ? 1 : -1} {}
		~Car() {
			delete ptr;
		}
	private:
		T *ptr;
		mutable int refcnt{-1};
	};

	template<typename T> class Cons {
	public:
		Cons(T &val, Cons &rhs);
		Cons(T &&val, Cons &rhs);
		Cons(const Cons &rhs);
		Cons &operator=(const Cons &rhs);
		Cons();
		~Cons() {
			// Do we need to delete car?
			if (!car->refcnt--) {
				delete car;
			}
			// If here, begin deleting cdr.
		}

		// TODO: Probably move this to cpp
		static const Cons nil{};
	private:
		// Default constructor available only for constructing nil
		Cons() : refcnt{-1}, car{nullptr}, clr{nullptr};
		mutable int refcnt{-1};
		T *car;
		Cons *cdr;

	};


	template<typename T> Cons cons(T val, Cons cdr);
}
#endif

// vim:ts=4:sw=4:noet

