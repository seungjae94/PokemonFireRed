#pragma once
#include <Windows.h>

// 설명 :
class EngineTime
{
public:
	// constructor destructor
	EngineTime();
	~EngineTime();

	// delete Function
	EngineTime(const EngineTime& _Other) = delete;
	EngineTime(EngineTime&& _Other) noexcept = delete;
	EngineTime& operator=(const EngineTime& _Other) = delete;
	EngineTime& operator=(EngineTime&& _Other) noexcept = delete;

	// 시간 측정을 시작하는 함수
	// - 객체 생성시 자동으로 호출된다.
	void TimeCheckStart();

	// 시간을 새로 측정하는 함수
	// - PrevTime과 DeltaTime을 갱신한다.
	float TimeCheck();

	double GetDeltaTime()
	{
		return DeltaTime;
	}

protected:

private:
	__int64 Count64;
	LARGE_INTEGER Count;     // 1초에 몇 카운트나 세는지
	LARGE_INTEGER PrevTime;  // 직전 측정 시간
	LARGE_INTEGER CurTime;   // 현재 측정 시간
	double dCount;			 // Count는 LARGE_INTEGER 타입으로  
	double DeltaTime;
};

