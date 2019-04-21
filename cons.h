#ifndef _CONS_H_
#define _CONS_H_

#include <memory>

using std::shared_ptr;

namespace Lisp {
	
// Empty struct used for sentinel nil, kept private to ensure only one instance.
struct Nil {};

template<typename T> class Cons; // forward decl
// Friend functions
template<typename T> shared_ptr<Cons<T>> cons(const T &val, Cons<T> cdr);
template<typename T> shared_ptr<Cons<T>> cons(const T &val, Cons<T>::Nil);
template<typename T, void *p> shared_ptr<Cons<T>> cons(const T &val, void *p);
template<typename T> T &car(shared_ptr<Cons<T>> cell);
template<typename T> shared_ptr<Cons<T>> cdr(shared_ptr<Cons<T>> cell);


template<typename T> class Cons {
public:
	friend shared_ptr<Cons<T>> cons<>(const T& , shared_ptr<Cons>);
	friend shared_ptr<Cons<T>> cons<>(const T& val, Cons);
	friend T& car<>(Cons<T> cell);
	friend shared_ptr<Cons<T>> cdr<>(shared_ptr<Cons<T>> cell);
	Cons(T *val, shared_ptr<Cons> cdr);
	Cons(const Cons &rhs);
	Cons &operator=(const Cons &rhs);
	~Cons() {}
	bool is_empty() const {
		// FIXME: This is wrong now...
		return this == Cons::nil;
	}

	// FIXME: This should really be const.
	static Cons nil;
private:
	// Default constructor available only for constructing nil
	Cons() {}
	shared_ptr<T> car {nullptr};
	shared_ptr<Cons> cdr {nullptr};
};

// Instantiate static member.
template<typename T> Cons<T> Cons<T>::nil{};

template<typename T> Cons<T>::Cons(T *val, shared_ptr<Cons<T> > sp_cdr)
	: car{shared_ptr<T>{val}}, cdr{sp_cdr}
{
}

template<typename T> Cons<T>::Cons(const Cons<T> &rhs)
	: car{rhs.car}, cdr{rhs.cdr}
{
}

template<typename T>
Cons<T> &Cons<T>::operator=(const Cons<T> &rhs)
{
	car = rhs.car;
	cdr = rhs.cdr;
}

// cons primitive
template<typename T> shared_ptr<Cons<T>> cons(const T &val, shared_ptr<Cons<T>> cdr)
{
	return Cons<T> {new T{val}, cdr}
}
template<typename T> Cons<T> cons(const T &val, Cons<T>::nil)
{
	return Cons<T> {new T{val}, nullptr};
}

template<typename T> T &car(shared_ptr<Cons<T>> cell)
{
	return *cell->car;
}

template<typename T> shared_ptr<Cons<T>> cdr(shared_ptr<Cons<T>> cell)
{
	// FIXME: Perhaps return nil instead of nullptr, but if I do that, I'll
	// need to give it some operations, at the very least, a conversion to
	// bool.
	return cell.cdr ? cell.cdr : nullptr;
}


}
#endif

// vim:ts=4:sw=4:noet

