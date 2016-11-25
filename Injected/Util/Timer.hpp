#pragma once
#include<ctime>

class timer
{
private:
	unsigned long begTime;
public:
	timer()
	{
		start();
	}

	void start()
	{
		begTime = clock();
	}

	unsigned long elapsedTime()
	{
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds)
	{
		return seconds >= elapsedTime();
	}
};
