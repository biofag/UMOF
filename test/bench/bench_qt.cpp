#include "qt_test.h"

#include <QMetaObject>
#include <QMetaMethod>

#include <ctime>

int main()
{
	Test t;
	const QMetaObject *meta = t.metaObject();
	QMetaMethod m = meta->method(meta->indexOfMethod("func(int,int)"));
	if (!m.isValid())
		return 1;

	std::clock_t c_start = std::clock();
	int result = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		int res;
		m.invoke(&t, Q_RETURN_ARG(int, res), Q_ARG(int, i), Q_ARG(int, i));
		result += res;
	}
	std::clock_t c_end = std::clock();
	std::cout << "Qt CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << result << std::endl;

	return 0;
}