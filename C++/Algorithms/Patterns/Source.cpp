#include "Factory.hpp"

#include <iostream>

using std::cout;
using std::endl;

template <class T>
struct OpNewCreation
{
	static T* Create() { return new T; }
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

		factory.Register(ClassID::FooID, OpNewCreation<Foo>::Create);
		factory.Register(ClassID::BarID, OpNewCreation<Bar>::Create);

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