#include <iostream>
#include "cons.h"

using namespace std;
using namespace Lisp;

int main()
{
	auto x = cons(42, Cons<int>::nil);
	//auto x = cons(42, Cons::nil);
	x = cons(43, x);
	car(x) = 45;
	x = cons(new int{53}, x);
	cout << "car(x)=" << car(x) << endl;
	cout << "cadr(x)=" << car(cdr(x)) << endl;

	return 0;
}

// vim:ts=4:sw=4:noet
