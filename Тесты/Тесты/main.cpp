#include <iostream>
#include <string>
#include <vector>
#include <type_traits>


using namespace std;

struct B1
{
	B1(){ cout << "A::A" << endl; }
};


class A1
{
private:
	int _p1, _p2, _p3;
public:
	A1();
	A1(int p1) : A1(p1, 12) {};
	A1(int p1, int p2) : A1(p1, p2, 10) {};
	A1(int p1, int p2, int p3) : _p1(p1), _p2(p2), _p3(p3){};
};

struct Object
{
	double first;
	int second;
};

struct IdString
{
	string name;
	int identifier;
};

IdString GetString()
{
	return {"SomeName", 4 };
}


struct A{
	virtual void g() final {}; // Никакой наследник не может её перегузить.
	virtual void f() {};
};

struct B : A
{
//	virtual void f() const override;
//	virtual void g() override;
};

struct F final
{
	int x, y;
};
//struct D : F{//ошибка: наследование от final классов запрещено
//	int z;
//};

template <class T>
void run(T* aData, size_t n)
{
	static_assert(std::is_pod<T>::value, "Тип Т должен быть простым.");//Проверка типа Т на простость на этапе компиляции.
	/*...*/
}

class SequenceClass{
public:
	SequenceClass(std::initializer_list<int> list){};
};


int main()
{
	SequenceClass someVar = { 1, 4, 5, 6 };


	// фишка
	int my_array[5] { 1, 2, 3, 4, 5 };
	for (int &x : my_array){
		x *= 2;
	}




	Object scalar{ 0.54, 45 };
	std::vector <std::string> v1 = { "a", "b", "c" };
	std::vector <std::string> v2{ "a", "b", "c" };
	std::vector <int> v3{ 5 };
	cout << "size = " << v3.size() << endl;


	cout << "Hello World" << endl;
	A a{};
	system("pause");


	return 0;
}

A a() { cout << "Hell" << endl; return A(); }