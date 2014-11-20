The uMOF library is meta system for C++.

[![Build Status](https://drone.io/bitbucket.org/occash/umof/status.png)](https://drone.io/bitbucket.org/occash/umof/latest)

## Usage
To use meta system the class should enherit from class Object. Use OBJECT macro to define metaclass specific api. 
```
class Test : public Object
{
	OBJECT(Test, Object)
	EXPOSE(Test, 
		OVERLOAD(lol, Test, int, int, int),
		OVERLOAD(lol, Test, float, float, float),
		METHOD(null),
		METHOD(test)
	)

public:
	Test() = default;

	int func(int a, int b)
	{
		return a + b;
	}

	float func(float a, float b)
	{
		return a + b;
	}

	int null()
	{
		return 0;
	}

	void test()
	{
		std::cout << "test" << std::endl;
	}
};
```
Now you can use exposed api of the class.
```
Test t;

//Api for exact signature
//Will throw if types are not consistent
Method m = t.api()->method("func(int,int)");
int i = any_cast<int>(m.invoke(&t, args));

//This function will try to cast type if possible
//else will throw
Any res = Api::invoke(&t, "func", {5.0f, "6.0"});

```

## Benchmark results
Configuration: Windows, Visual Studio 2013, Release x86

Framework | Compile/Link time, ms | Executable size, KB | Call time spent*, ms
-----|------|-----|-----
Native | 371/63 | 12 | 2 (45**)
uMOF | 406/78 | 18 | 349
Camp | 4492/116 | 66 | 6889
Qt | 1040/80 (+129***) | 15 | 498
cpgf | 2514/166 | 71 | 1184

\* 10000000 calls

\*\* Force noinline

\*\*\* Meta object compiler