#include <queue>
#include <set>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

struct A
{
	A(){ cout << "CA "; };
	~A() { cout << "DA "; }
};
struct B:public A
{
	B() { cout << "CB "; }
	B(const B&) { cout << "CCB ";}
	~B() { cout << "DB "; }

};

int main()
{
	B b;
	A *pa = &b;
	A a(*pa);
	return 0;
}
//CB CA 