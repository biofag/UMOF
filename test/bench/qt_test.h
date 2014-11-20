#ifndef QT_TEST
#define QT_TEST

#include <QObject>
#include <iostream>

//Free functions not supported
/*void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}*/

class Test : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int val READ getVal WRITE setVal)

public:
	Q_INVOKABLE int func(int a, int b)
	{
		return a + b;
	}

	Q_INVOKABLE float func(float a, float b)
	{
		return a + b;
	}

	Q_INVOKABLE int null() const
	{
		return 0;
	}

	Q_INVOKABLE void print() const
	{
		std::cout << "Hello from native func" << std::endl;
	}

	Q_INVOKABLE static void static_func()
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

#endif