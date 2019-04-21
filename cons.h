#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using std::shared_ptr;

namespace Lisp {
	
// Empty struct used for sentinel nil, kept private to ensure only one instance.
struct Nil {};

template<typename T> class Cons; // forward decl
// Friend functions
template<typename T> Cons<T> cons(const T& val, Cons<T> cdr);
template<typename T> Cons<T> cons(T* p, Cons<T> cdr);
template<typename T> T &car(Cons<T> cell);
template<typename T> Cons<T> cdr(Cons<T> cell);


template<typename T> class Cons {
public:
	friend Cons<T> cons<>(const T&, Cons);
	friend Cons<T> cons<>(T*, Cons);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	Cons(T *val, Cons cdr);
	Cons(const Cons &rhs);
	Cons &operator=(const Cons &rhs);
	~Cons() {}
	bool is_empty() const {
		// FIXME: This is wrong now...
		return this == &nil;
	}

	// FIXME: This should really be const.
	const static Cons nil;
private:
	// Default constructor available only for constructing nil
	Cons() {}
	Cons(shared_ptr<T> car, shared_ptr<Cons> cdr);
	shared_ptr<T> car {nullptr};
	shared_ptr<Cons> cdr {nullptr};
};

// Instantiate static member.
template<typename T> const Cons<T> Cons<T>::nil{};

template<typename T> Cons<T>::Cons(T *val, Cons cell)
	: car{shared_ptr<T>{val}}, cdr{shared_ptr<Cons>(new Cons{cell})}
{
}

template<typename T> Cons<T>::Cons(const Cons<T> &rhs)
	: car{rhs.car}, cdr{rhs.cdr}
{
}

template<typename T> Cons<T>::Cons(shared_ptr<T> rcar, shared_ptr<Cons> rcdr)
	: car{rcar}, cdr{rcdr}
{
}

template<typename T>
Cons<T> &Cons<T>::operator=(const Cons<T> &rhs)
{
	car = rhs.car;
	cdr = rhs.cdr;
}

// cons primitive
template<typename T> Cons<T> cons(const T &val, Cons<T> cdr)
{
	// FIXME: Handle nil specially...
	return Cons<T> {shared_ptr<T>(new T{val}), shared_ptr<Cons<T>>(new Cons<T>(cdr))};
}

template<typename T> Cons<T> cons(T* p, Cons<T> cdr)
{
	// FIXME: Handle nil specially...
	return Cons<T> {shared_ptr<T>(p), shared_ptr<Cons<T>>(new Cons<T>(cdr))};
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
	return cell.cdr ? *cell.cdr : Cons<T>::nil;
}

// TODO
// initializer_list ctor
// cadr, cdar, etc...
// Handle nil specially in cons

}
#endif

// vim:ts=4:sw=4:noet

