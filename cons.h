#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using namespace std;

namespace Lisp {
	
template<typename T> class Cons;
template <typename T> class Cons_impl;

template<typename T> class Cons {
public:
	const static Cons Nil;
	friend class Cons_impl<T>; // private ctor
	const T& car() const;
	Cons<T> cdr() const; // TODO: constness?
	template<typename... Args> Cons cons(Args&&... args) const;
	Cons<T> cons(T&& val) const;

private:
	Cons() {}
	Cons(shared_ptr<typename Cons_impl<T>::Cons_impl_shared> sp_init)
		: sp{sp_init} {}
	shared_ptr<typename Cons_impl<T>::Cons_impl_shared> sp;
};

template<typename T> const Cons<T> Cons<T>::Nil;

template<typename T>
template<typename... Args>
Cons<T> Cons<T>::cons(Args&&... args) const {
	return make_shared<typename Cons_impl<T>::Cons_impl_shared>(
		sp, std::forward<Args>(args)...);
}

template<typename T>
Cons<T> Cons<T>::cons(T&& val) const
{
	return make_shared<typename Cons_impl<T>::Cons_impl_shared>(
		std::forward<T>(val), sp);
}

// TODO: Decide on mutability.
template<typename T> const T& Cons<T>::car() const
{
	// FIXME: Throw exception if this == nil
	if (!sp)
		throw std::runtime_error {"car on end of list"};
	return sp->car;
}

template<typename T> Cons<T> Cons<T>::cdr() const
{
	// Question: Should I return nil for cdr on nil element, or throw exception?
	return sp->cdr;
}

template<typename T> class Cons_impl {
public:
	friend class Cons<T>;
	struct Cons_impl_shared;
	virtual ~Cons_impl() {
		cout << "Cons_impl dtor(" << this << ")\n";
	}

private:
	template<typename... Args>
	Cons_impl(shared_ptr<typename Cons_impl<T>::Cons_impl_shared> cdr_init,
			Args&&... car_init)
		: car{std::forward<Args>(car_init)...}, cdr{cdr_init} {}
	Cons_impl(T&& car_init,
			shared_ptr<typename Cons_impl<T>::Cons_impl_shared> cdr_init)
		: car{std::forward<T>(car_init)}, cdr{cdr_init} {}

	T car;
	shared_ptr<typename Cons_impl<T>::Cons_impl_shared> cdr;
};

// Note: Definition must appear outside class to prevent incomplete class
// warning/error.
template <typename T>
struct Cons_impl<T>::Cons_impl_shared : public Cons_impl<T> {
	template <typename... Args> Cons_impl_shared(Args&&... args)
	: Cons_impl(forward<Args>(args)...) {}
};


// TODO
// initializer_list ctor
// cadr, cdar, etc...
// Handle nil specially in cons

}
#endif

// vim:ts=4:sw=4:noet:cino=N-s

