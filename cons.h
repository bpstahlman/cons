#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using namespace std;

namespace Lisp {
	
// TODO: Make this a null shared_ptr using a variable template.
template <typename T> class Cons_impl;
template <typename T> using Cons = shared_ptr<Cons_impl<T>>;
template <typename T> Cons<T> Nil{};

template<typename T, typename... Args>
Cons<T> cons(Args&&... args, Cons<T> cdr);
template <typename T> Cons<T> cons(T&& val, Cons<T> cdr);
template <typename T> T& car(Cons<T> cell);
template <typename T> Cons<T> cdr(Cons<T> cell);

// Instantiate static member.
//template<typename T> const Cons<T> Cons<T>::nil{};

template<typename T> class Cons_impl {
public:
	struct Cons_impl_shared;
	friend Cons<T> cons<>(T&&, Cons<T>);
	// TODO: Understand why cons<> causes error.
	template<typename... Args>
	friend Cons<T> cons(Args&&... args, Cons<T> cdr);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	virtual ~Cons_impl() {
		cout << "Cons_impl dtor(" << this << ")\n";
	}

private:
	template<typename... Args>
	Cons_impl(Args&&... car_init, Cons<T> cdr_init)
		: car{std::forward<Args>(car_init)...}, cdr{cdr_init} {}
	Cons_impl(T&& car_init, Cons<T> cdr_init)
		: car{std::forward<T>(car_init)}, cdr{cdr_init} {}

	T car;
	Cons<T> cdr;
};

// Note: Definition must appear outside class to prevent incomplete class
// warning/error.
template <typename T>
struct Cons_impl<T>::Cons_impl_shared : public Cons_impl<T> {
	template <typename... Args> Cons_impl_shared(Args&&... args)
	: Cons_impl(forward<Args>(args)...) {}
};

template<typename T, typename... Args>
Cons<T> cons(Args&&... args, Cons<T> cdr)
{
	// TODO: Make sure this is really friend of Cons_impl.
	// Weird! Why does this compile, even if I use cdr.car or cdr.foobar???
	// Perhaps because it's not being instantiated?
	T x{cdr->car};
	return Cons<T>{
		make_shared<typename Cons_impl<T>::Cons_impl_shared>(
				std::forward<Args>(args)..., cdr)};
}

template<typename T> Cons<T> cons(T&& val, Cons<T> cdr)
{
	// TODO: Is Cons<T> needed?
	return Cons<T>{
		make_shared<typename Cons_impl<T>::Cons_impl_shared>(
				std::forward<T>(val), cdr)};
}

// TODO: Decide on mutability.
template<typename T> T& car(Cons<T> cell)
{
	// FIXME: Throw exception if this == nil
	if (!cell)
		throw std::runtime_error {"car on end of list"};
	return cell->car;
}

template<typename T> Cons<T> cdr(Cons<T> cell)
{
	// Question: Should I return nil for cdr on nil element, or throw exception?
	return cell->cdr ? cell->cdr : Nil<T>;
}


// TODO
// initializer_list ctor
// cadr, cdar, etc...
// Handle nil specially in cons

}
#endif

// vim:ts=4:sw=4:noet:cino=N-s

