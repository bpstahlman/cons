// --- main.cpp ---
#include <iostream>
#include "cons.h"

using namespace std;
using namespace Lisp;

struct Foo {
	friend ostream& operator<<(ostream& os, const Foo& foo);
	// Note: Move ctor provided only to verify that it's called for temporaries.
	Foo(Foo&& rhs) : x{rhs.x}, y{rhs.y} {
#ifdef DEBUG
		cout << "Move-constructing Foo@" << this << " from " <<
			rhs << "@" << &rhs << endl;
#endif
	}
	Foo(int xin, int yin) : x{xin}, y{yin} {}
	virtual ~Foo() {
#ifdef DEBUG
		cout << "Destructing " << *this << "\n";
#endif
	}
	int x, y;
};

ostream& operator<<(ostream& os, const Foo& foo)
{
	cout << "Foo{" << foo.x << ", " << foo.y << "}";
	return os;
}

int main()
{
	cout << "Construct list of ints\n";
	auto x = Cons<int>::Nil.cons(42);
	x = x.cons(43).cons(44).cons(45);
	cout << "Construct list of Foo\n";
	auto foo = Cons<Foo>::Nil.cons(3, 4);
	foo = foo.cons(5, 6).cons(7, 8);

	// Create a list of lists.
	auto xx = Cons<Cons<int>>::Nil.cons(x);
	xx = xx.cons(Cons<int>::Nil.cons(100).cons(101));
	cout << "xx:" << endl;
	cout << xx << endl;
	// Try out cdar, cddar, etc...
	cout << "(cdar xx): " << cdar(xx) << endl;
	cout << "(cdadr xx): " << cdadr(xx) << endl;

	{
		cout << "Entering block...\n";
		// Create a list called bar by consing a few Foo temporaries onto head
		// of foo list.
		// Note: Structural sharing ensures the last few elements are shared
		// between the foo and bar lists.
		auto bar = foo.cons(Foo{42, 43}).cons(Foo{44, 45});
		caddr(bar).x *= 1000;
		caddr(bar).y *= 1000;
		// Verify that both lists have changed (due to structural sharing).
		cout << "foo:\n" << foo << endl;
		cout << "bar:\n" << bar << endl;
		cout << "Leaving block...\n";

		cout << bar << endl;

	}

	return 0;
}

// vim:ts=4:sw=4:noet
