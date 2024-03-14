#include "EngineTime.h"

EngineTime::EngineTime()
{
	TimeCheckStart();
}

EngineTime::~EngineTime()
{
}

void EngineTime::TimeCheckStart()
{
	QueryPerformanceFrequency(&Count);
	QueryPerformanceCounter(&PrevTime);

	dCount = static_cast<double>(Count.QuadPart);
}

float EngineTime::TimeCheck()
{
	QueryPerformanceCounter(&CurTime);

	// DeltaTime 갱신
	__int64 Tick = (CurTime.QuadPart - PrevTime.QuadPart);
	
	double dTick = static_cast<double>(Tick);
	DeltaTime = dTick / dCount;

	// PrevTime 갱신
	PrevTime.QuadPart = CurTime.QuadPart;
	return static_cast<float>(DeltaTime);
}
