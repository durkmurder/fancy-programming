#include "Factory.hpp"

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <class T>
struct OpNewCreation
{
	static T* Create() { return new T; }
};

template <class T>
struct UniquePtrCreation
{
	static std::unique_ptr<T> Create() { return std::unique_ptr<T>(new T); }
};

namespace FactoryTest
{
	enum ClassID { FooID = 0, BarID };
	class Base
	{
	public:
		virtual void sayHello() = 0;
	};
	class Foo : public Base
	{
	public:
		virtual void sayHello() { cout << "Hello from Foo" << endl; }
	};
	class Bar : public Base
	{
	public:
		virtual void sayHello() { cout << "Hello from Bar" << endl; }
	};

	void testFactory()
	{
		typedef Factory< Base, ClassID > BaseFactory;
		BaseFactory factory;

		factory.Register(ClassID::FooID, UniquePtrCreation<Foo>::Create);
		factory.Register(ClassID::BarID, UniquePtrCreation<Bar>::Create);

		auto fooObj = factory.CreateObject(FooID);
		auto barObj = factory.CreateObject(BarID);

		fooObj->sayHello();
		barObj->sayHello();
	}
}
int main()
{
	FactoryTest::testFactory();

	return 0;
}