#include <iostream>
#include "cons.h"

using namespace std;
using namespace Lisp;

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
	//auto x = cons(42, Cons::nil);
	x = cons(43, x);
	car(x) = 45;
	x = cons(new int{53}, x);
	x = cdr(x);
	cout << "car(x)=" << car(x) << endl;
	x = cdr(x);
	cout << "car(x)=" << car(x) << endl;
	//cout << "cadr(x)=" << car(cdr(x)) << endl;
	
	auto f = cons(Foo{}, Cons<Foo>::nil);
	f = cons(Foo{}, f);
	cout << car(f) << " " << car(cdr(f)) << " " << endl;

	return 0;
}

// vim:ts=4:sw=4:noet
