#include <camp/camptype.hpp>
#include <camp/class.hpp>

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
	camp::Class::declare<Test>("Test")
		.property("val", &Test::getVal, &Test::setVal)
		.function("func", &Test::func)
		.function("null", &Test::null)
		.function("print", &Test::print);
		//Static methods and free function not supported
		//.function("static_func", &Test::static_func);
		//.function("my_fun", my_fun);
}

CAMP_TYPE(Test)

int main()
{
	Test t;

	declare();
	const camp::Class *metaclass = &camp::classByName("Test");
	const camp::Function *nullFunc = &metaclass->function("func");

	std::clock_t c_start = std::clock();
	int result = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		camp::Value v = nullFunc->call(t, camp::Args(i, i));
		result += v.to<int>();
	}
	std::clock_t c_end = std::clock();
	std::cout << "Camp CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << result << std::endl;

	return 0;
}