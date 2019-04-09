#include <iostream>
#include "cons.h"

using namespace std;
using namespace Lisp;

template<void *p>
void f<p>(void *p)
{
	static_assert(false);
}

template<>
void f<nullptr>(void *p)
{
	cout << "Inside nullptr specialization" << endl;
}

int main()
{
	//auto x = cons(42, Cons<int>::nil);
	auto x = cons(42, Cons::nil);
	x = cons(43, x);
	cout << "car(x)=" << car(x) << endl;
	cout << "cadr(x)=" << car(cdr(x)) << endl;

	return 0;
}

// vim:ts=4:sw=4:noet
