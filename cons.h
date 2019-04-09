#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using std::shared_ptr;

namespace Lisp {
	
	// Empty struct used for sentinel nil, kept private to ensure only one instance.
	struct Nil {};

	template<typename T> class Cons; // forward decl
	// Friend functions
	template<typename T> Cons<T> cons(const T &val, Cons<T> cdr);
	template<typename T> Cons<T> cons(const T &val, Cons::Nil);
	template<typename T, void *p> Cons<T> cons(const T &val, void *p);
	template<typename T> T &car(Cons<T> cell);
	template<typename T> Cons<T> cdr(Cons<T> cell);


	template<typename T> class Cons {
	public:
		friend Cons<T> cons<>(const T &, Cons);
		friend Cons<T> cons<>(const T &val, Nil);
		friend T &car<>(Cons<T> cell);
		friend Cons<T> cdr<>(Cons<T> cell);
		Cons(T *val, shared_ptr<Cons> cdr);
		Cons(const Cons &rhs);
		Cons &operator=(const Cons &rhs);
		~Cons() {}
		bool is_empty() const {
			// FIXME: This is wrong now...
			return this == Cons::nil;
		}

		// FIXME: This should really be const.
		static Cons nilcons;
		static Nil nil {};
	private:
		// Default constructor available only for constructing nil
		Cons() {}
		shared_ptr<T> car {nullptr};
		shared_ptr<Cons> cdr {nullptr};
		shared_ptr<Cons> *self {nullptr};
	};


	template<typename T> Cons<T> Cons<T>::nil{};

	template<typename T> Cons<T>::Cons(T *val, shared_ptr<Cons<T> > sp_cdr)
		: cdr{sp_cdr}, car{shared_ptr<T>{val}}, self{nullptr}
	{
	}

	template<typename T> Cons<T>::Cons(const Cons<T> &rhs)
		: car{rhs.car}, cdr{rhs.cdr}, self{nullptr}
	{
	}

	template<typename T>
	Cons<T> &Cons<T>::operator=(const Cons<T> &rhs)
	{
		car = rhs.car;
		cdr = rhs.cdr;
		// Assumption: lhs of assignment is never heap instance (though its car
		// and cdr are).
		self = nullptr;
	}

	// cons primitive
	// TODO: Add support for rhs refs later...
	template<typename T> Cons<T> cons(const T &val, Cons<T> cdr)
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
	template<typename T> Cons<T> cons(const T &val, Nil nil)
	{
		return Cons<T> {new T{val}, nullptr};
	}

	template<typename T> T &car(Cons<T> cell)
	{
		return *cell.car;
	}

	template<typename T> Cons<T> cdr(Cons<T> cell)
	{
		// FIXME: Perhaps return nil instead of nullptr, but if I do that, I'll
		// need to give it some operations, at the very least, a conversion to
		// bool.
		return cell.cdr ? *cell.cdr : nullptr;
	}

}
#endif

// vim:ts=4:sw=4:noet

