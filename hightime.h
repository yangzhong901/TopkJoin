#pragma once
#ifndef HIGHTIME_H_
#define HIGHTIME_H_
#include <windows.h>
class hightime
{
public:
	hightime(void);
	~hightime(void);//Îö¹¹º¯Êý

private:
	LARGE_INTEGER startTime;

	LARGE_INTEGER endTime;

	LARGE_INTEGER CPUfrequency;

public:
	double interval;

public:
	void hightimeStart();
	void hightimeEnd();
};




#endif