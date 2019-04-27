#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using std::shared_ptr;

namespace Lisp {
	
// Forward declarations
template<typename T> class Cons;
template<typename T> class Cons_impl;
// Friend functions
template<typename T> Cons<T> cons(const T& val, Cons<T> cdr);
template<typename T> Cons<T> cons(T* p, Cons<T> cdr);
template<typename T> T& car(Cons<T> cell);
template<typename T> Cons<T> cdr(Cons<T> cell);

template<typename T> class Cons : private shared_ptr<Cons_impl<T>> {
	using base = shared_ptr<Cons_impl<T>>;
	public:
	friend class Cons_impl<T>;
	// Decide which class really needs friendship...
	friend Cons<T> cons<>(const T&, Cons<T>);
	friend Cons<T> cons<>(T*, Cons<T>);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	const static Cons nil;
	private:
	Cons(T* p, Cons cdr) :
		shared_ptr<Cons_impl<T>>{new Cons_impl<T>{shared_ptr<T>{p}, cdr}} {}
	Cons() : shared_ptr<Cons_impl<T>>(nullptr) {}
};

// Instantiate static member.
template<typename T> const Cons<T> Cons<T>::nil{};

template<typename T> class Cons_impl {
public:
	friend class Cons<T>;
	friend Cons<T> cons<>(const T&, Cons<T>);
	friend Cons<T> cons<>(T*, Cons<T>);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	//Cons_impl(const Cons_impl &rhs);
	//Cons_impl &operator=(const Cons_impl &rhs);
	~Cons_impl() {}
	bool is_empty() const {
		return !(bool)*this;
	}

private:
	Cons_impl(shared_ptr<T> car_init, Cons<T> cdr_init)
		: car{car_init}, cdr{cdr_init} {}
	shared_ptr<T> car {nullptr};
	Cons<T> cdr {nullptr};
};

// cons primitive
// TODO: Use perfect forwarding
template<typename T> Cons<T> cons(const T &val, Cons<T> cdr)
{
	return Cons<T>{new T{val}, cdr};
}

template<typename T> Cons<T> cons(T* p, Cons<T> cdr)
{
	return Cons<T>{p, cdr};
}

template<typename T> T& car(Cons<T> cell)
{
	// FIXME: Throw exception if this == nil
	if (!cell)
		throw std::runtime_error {"car on end of list"};
	return *cell->car;
}

template<typename T> Cons<T> cdr(Cons<T> cell)
{
	// Question: Should I return nil for cdr on nil element, or throw exception?
	return cell ? cell->cdr : Cons<T>::nil;
}

// FIXME: Maybe remove...

// TODO
// initializer_list ctor
// cadr, cdar, etc...
// Handle nil specially in cons

}
#endif

// vim:ts=4:sw=4:noet:cino=N-s

