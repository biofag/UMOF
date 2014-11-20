#include <iostream>
#include <ctime>

//Force noinline
#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define NOINLINE __attribute__ ((noinline))
#else
#define NOINLINE
#endif

void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}

class Object {};

class Test : public Object
{
public:
	NOINLINE int func(int a, int b)
	{
		return a + b;
	}

	NOINLINE float func(float a, float b)
	{
		return a + b;
	}

	NOINLINE int null() const
	{
		return 0;
	}

	NOINLINE void print() const
	{
		std::cout << "Hello from native func" << std::endl;
	}

	NOINLINE static void static_func()
	{
		std::cout << "Hello from static func" << std::endl;
	}
	
	NOINLINE int getVal() const
	{
		return _val;
	}

	NOINLINE void setVal(int v)
	{
		_val = v;
	}

private:
	int _val;

};

int main()
{
	Test t;

	std::clock_t c_start = std::clock();
	int result = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		result += t.func(i, i);
	}
	std::clock_t c_end = std::clock();
	std::cout << "Native CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << result << std::endl;

	return 0;
}