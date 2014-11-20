#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gscopedinterface.h"

#include <iostream>
#include <ctime>

void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}

class Object {};

class Test : public Object
{
public:
	int func(int a, int b)
	{
		return a + b;
	}

	//Overloaded methods not supported
	/*float func(float a, float b)
	{
		return a + b;
	}*/

	int null() const
	{
		return 0;
	}

	void print() const
	{
		std::cout << "Hello from native func" << std::endl;
	}

	static void static_func()
	{
		std::cout << "Hello from static func" << std::endl;
	}

	int getVal() const
	{
		return _val;
	}

	void setVal(int v)
	{
		_val = v;
	}

private:
	int _val;

};

void declare()
{
	using namespace cpgf;
	GDefineMetaClass<Test>
		::define("Test")
		._method("func", &Test::func)
		._method("null", &Test::null)
		._method("print", &Test::print)
		._method("static_func", &Test::static_func)
		._property("val", &Test::getVal, &Test::setVal)
		;
}

int main()
{
	Test t;

	declare();
	const cpgf::GMetaClass *metaClass;
	const cpgf::GMetaMethod *method;

	metaClass = cpgf::findMetaClass("Test");
	method = metaClass->getMethod("func");

	std::clock_t c_start = std::clock();
	int result = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		result += cpgf::fromVariant<int>(method->invoke(&t, i, i));
	}
	std::clock_t c_end = std::clock();
	std::cout << "Cpgf CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << result << std::endl;

	return 0;
}