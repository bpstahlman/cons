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

// TODO: Consider containment rather than derivation. If it stays this way,
// consider making derivation private...
template<typename T> class Cons : public shared_ptr<Cons_impl<T>> {
	public:
	friend class Cons_impl<T>;
	// Decide which class really needs friendship...
	friend Cons<T> cons<>(const T&, Cons<T>);
	friend Cons<T> cons<>(T*, Cons<T>);
	friend T& car<>(Cons<T> cell);
	friend Cons<T> cdr<>(Cons<T> cell);
	const static Cons nil;
	private:
	// UNDER CONSTRUCTION!!!
	Cons(shared_ptr<T> p, Cons cdr) :
		shared_ptr<Cons_impl<T>>{new Cons_impl<T>{p, cdr}} {}
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
	// Default constructor available only for constructing nil
	//Cons_impl() {}
	Cons_impl(shared_ptr<T> car_init, Cons<T> cdr_init);
	shared_ptr<T> car {nullptr};
	Cons<T> cdr {nullptr};
};

template<typename T> Cons_impl<T>::Cons_impl(shared_ptr<T> car_init, Cons<T> cdr_init)
	: car{car_init}, cdr{cdr_init}
{
}


// cons primitive
// TODO: Use perfect forwarding
template<typename T> Cons<T> cons(const T &val, Cons<T> cdr)
{
	// FIXME: Handle nil specially...
	return Cons<T>{shared_ptr<T>{new T{val}}, cdr};
}

template<typename T> Cons<T> cons(T* p, Cons<T> cdr)
{
	// FIXME: Handle nil specially...
	return Cons<T>{shared_ptr<T>(p), cdr};
}

template<typename T> T& car(Cons<T> cell)
{
	// FIXME: Throw exception if this == nil
	return *cell->car;
}

template<typename T> Cons<T> cdr(Cons<T> cell)
{
	// FIXME: Perhaps return nil instead of nullptr, but if I do that, I'll
	// need to give it some operations, at the very least, a conversion to
	// bool.
	// TODO: Research a nullptr version of sp subclass.
	return cell ? cell->cdr : Cons<T>::nil;
}

// FIXME: Maybe remove...
#if 0
template<typename T> Cons_impl<T>::Cons_impl(const Cons_impl<T> &rhs)
	: car{rhs.car}, cdr{rhs.cdr}
{
}

template<typename T> Cons_impl<T>::Cons_impl(shared_ptr<T> rcar, shared_ptr<Cons_impl> rcdr)
	: car{rcar}, cdr{rcdr}
{
}
#endif

#if 0 // may not need this now...
template<typename T>
Cons_impl<T> &Cons_impl<T>::operator=(const Cons_impl<T> &rhs)
{
	shared_ptr<T>::operator=(static_cast<shared_ptr<Cons<T>>
	car = rhs.car;
	cdr = rhs.cdr;
}
#endif
// TODO
// initializer_list ctor
// cadr, cdar, etc...
// Handle nil specially in cons

}
#endif

// vim:ts=4:sw=4:noet

