// CRTP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;


class Base
{
public:
	Base(int b){a = b;cout << __FUNCTION__ << endl;}

	virtual ~Base(){cout << __FUNCTION__ << endl;}
	virtual void do_something(){cout << __FUNCTION__ << endl;}

	int getA(){return a;}

private:
	int a;

};


class Derived : public Base
{
public:
	Derived(int b):Base(b){cout << __FUNCTION__ << endl;}
	~Derived(){cout << __FUNCTION__ << endl;}

	void do_something(){cout << __FUNCTION__ << endl;}

private:

};


template <typename T>
class CRTPClass
{
public:
	CRTPClass(){cout << __FUNCTION__ << endl;}
	virtual ~CRTPClass(){
		cout << __FUNCTION__ << endl;
	}

	void do_something_interface() const
	{
		static_cast<const T*>(this)->do_something();
		/**
		 因为使用方法是class T: public CRTPClass<T>
		 所以T一定是CRTPClass的一个子类，所以这里可以用static_cast将父类指针this转换为子类指针去调用子类函数
		*/
	}

};

class CRTPClass_Derived1 : public CRTPClass<CRTPClass_Derived1>
{
public:
	CRTPClass_Derived1() { cout << __FUNCTION__ << endl; }
	~CRTPClass_Derived1() { cout << __FUNCTION__ << endl; }
	void do_something() const
	{
		cout << __FUNCTION__ << endl;
	}

};

class CRTPClass_Derived2 : public CRTPClass<CRTPClass_Derived2>
{
public:
	CRTPClass_Derived2() { cout << __FUNCTION__ << endl; }
	~CRTPClass_Derived2() { cout << __FUNCTION__ << endl; }
	void do_something() const
	{
		cout << __FUNCTION__ << endl;
	}

};



int main()
{

	/*  1.
		运行时多态，即在运行的时候基类指针调用函数时，发现函数是虚函数，于是去查找所指向对象的虚函数表。
		如果指向对象是子类，子类的虚函数表中就会用子类中重写的子类函数去覆盖掉父类函数，这样最终调用的就是子类的函数
	*/	
	Derived* d1 = new Derived(10);
	d1->do_something();
	cout << d1->getA() << endl;
	delete d1;
	cout << "------" << endl;

	Base* d2 = new Derived(10);
	d2->do_something();
	cout << d2->getA() << endl;
	delete d2;
	cout << "------" << endl;

	/*  2.
		编译期多态（静态多态）, 利用CRTP机制，通过模板和静态绑定的方式实现类似于虚函数的行为，但不需要虚函数表，从而提高性能。
	*/
	CRTPClass<CRTPClass_Derived1>* c1 = new CRTPClass_Derived1();
	c1->do_something_interface();
	delete c1;
	cout << "------" << endl;

	CRTPClass<CRTPClass_Derived2>* c2 = new CRTPClass_Derived2();
	c2->do_something_interface();
	delete c2;
}

