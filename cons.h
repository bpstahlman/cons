#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using namespace std;

namespace Lisp {
	
template <typename T> class Cons;
template <typename T> class Cons_impl;
template <typename T> Cons<T> cons(T&& val, Cons<T> cdr);
template <typename T> T& car(Cons<T> cell);
template <typename T> Cons<T> cdr(Cons<T> cell);

template <typename T> class Cons {
	public:
	using cell_type = shared_ptr<Cons_impl<T>>;
	friend Cons<T> cons<>(T&&, Cons<T>);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	const static Cons nil;

	private:
	Cons(cell_type cell_init) :
		cell {cell_init} {}
	Cons() {}
	cell_type cell;
};

// Instantiate static member.
template<typename T> const Cons<T> Cons<T>::nil{};

template<typename T> class Cons_impl {
public:
	struct Cons_impl_shared;
	friend class Cons<T>;
	friend Cons<T> cons<>(T&&, Cons<T>);
	template<typename... Args>
	friend Cons<T> cons(Args... args, Cons<T> cdr);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	virtual ~Cons_impl() {
		cout << "Cons_impl dtor(" << this << ")\n";
	}

private:
	template<typename... Args>
	Cons_impl(Args... car_init, shared_ptr<Cons_impl<T>> cdr_init)
		: car{car_init...}, cdr{cdr_init} {}
	Cons_impl(T&& car_init, shared_ptr<Cons_impl<T>> cdr_init)
		: car{std::forward<T>(car_init)}, cdr{cdr_init} {}

	T car;
	shared_ptr<Cons_impl<T>> cdr;
};

// Note: Definition must appear outside class to prevent incomplete class
// warning/error.
template <typename T>
struct Cons_impl<T>::Cons_impl_shared : public Cons_impl<T> {
	template <typename... Args>
	Cons_impl_shared(Args&&... args)
	: Cons_impl(forward<Args>(args)...) {}
};

template<typename T> Cons<T> cons(T&& val, Cons<T> cdr)
{
	return Cons<T>{
		make_shared<typename Cons_impl<T>::Cons_impl_shared>(
				std::forward<T>(val), cdr.cell)};
}

template<typename T> T& car(Cons<T> cell)
{
	// FIXME: Throw exception if this == nil
	if (!cell.cell)
		throw std::runtime_error {"car on end of list"};
	return cell.cell->car;
}

template<typename T> Cons<T> cdr(Cons<T> cell)
{
	// Question: Should I return nil for cdr on nil element, or throw exception?
	return cell.cell->cdr ? Cons<T>{cell.cell->cdr} : Cons<T>::nil;
}


// TODO
// initializer_list ctor
// cadr, cdar, etc...
// Handle nil specially in cons

}
#endif

// vim:ts=4:sw=4:noet:cino=N-s

