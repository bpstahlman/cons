#include <iostream>
#include "cons.h"

using namespace std;
using namespace Lisp;

struct Foo;
struct Foo {
	friend ostream& operator<<(ostream& os, const Foo& foo);
	Foo(int xin, int yin) : x{xin}, y{yin} {}
	virtual ~Foo() { cout << "Destructing " << *this << "\n"; }
	int x, y;
};

ostream& operator<<(ostream& os, const Foo& foo)
{
	cout << "Foo(" << foo.x << ", " << foo.y << ")\n";
	return os;
}

int main()
{
	auto x = Cons<int>::Nil.cons(42);
	auto foo = Cons<Foo>::Nil.cons(3, 4);
	foo.cons(5, 6);
	foo.cons(7, 8);

	{
		auto bar = foo.cdr();
		bar = bar.cons(Foo{42, 43});
		bar = bar.cons(Foo{52, 53});
		cout << "bar.cdr.car: " << bar.cdr().car() << endl;
	}

	return 0;
}

#if 0
struct Foo {
		static int cnt;
		Foo() : x{cnt++} { cout << "default constructor\n"; }
		Foo(const Foo& rhs) : x{rhs.x} { cout << "Foo copy constructor\n"; }
		Foo(const Foo&& rhs) : x{rhs.x} { cout << "Foo move constructor\n"; }
		Foo& operator=(const Foo& rhs) {
			x = rhs.x; cout << "Foo copy assignment\n"; return *this;
		}
		Foo& operator=(const Foo&& rhs) {
			x = rhs.x; cout << "Foo move assignment\n"; return *this;
		}
		~Foo() { cout << "Foo destructor\n"; }
		int x;
};

int Foo::cnt {0};

ostream& operator<<(ostream& os, const Foo& foo)
{
	os << foo.x;
}

int main()
{
	auto x = cons(42, Cons<int>::nil);
	x = cons(43, x);
	car(x) = 45;
	x = cons(53, x);
	x = cdr(x);
	cout << "car(x)=" << car(x) << endl;
	x = cdr(x);
	cout << "car(x)=" << car(x) << endl;
	//cout << "cadr(x)=" << car(cdr(x)) << endl;
	
#if 0
	auto f = cons(Foo{}, Cons<Foo>::nil);
	f = cons(Foo{}, f);
	cout << car(f) << " " << car(cdr(f)) << " " << endl;
#endif

	return 0;
}
#endif

// vim:ts=4:sw=4:noet
