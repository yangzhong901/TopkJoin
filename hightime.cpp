#include "hightime.h"

hightime::hightime(void)
{
	QueryPerformanceFrequency(&CPUfrequency);
}

hightime::~hightime(void)
{
}

void hightime::hightimeStart()
{
	QueryPerformanceCounter(&startTime);
}

void hightime::hightimeEnd()
{
	QueryPerformanceCounter(&endTime);

	interval = ((double)endTime.QuadPart - (double)startTime.QuadPart) / (double)CPUfrequency.QuadPart;

}