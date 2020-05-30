#include <iostream>
#include "ThreadPool.h"
#include "UTestCase.h"

using namespace std;

bool sleep500ms()
{
	this_thread::sleep_for(500ms);
	return true;
}

int main()
{
	UTestCase p1("pool1", [] {
		ThreadPool pool;
		for (int i = 1; i <= 500; ++i)
		{
			UTestCase t("test" + to_string(i), sleep500ms);
			pool.enqueue([t]() mutable { t(); cout << t; });
		}
		return true;
	});
	p1();
	cout << p1;

	return 0;
}
