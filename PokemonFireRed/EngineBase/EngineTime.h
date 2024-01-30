#pragma once
#include <Windows.h>

// ���� :
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

	// �ð� ������ �����ϴ� �Լ�
	// - ��ü ������ �ڵ����� ȣ��ȴ�.
	void TimeCheckStart();

	// �ð��� ���� �����ϴ� �Լ�
	// - PrevTime�� DeltaTime�� �����Ѵ�.
	float TimeCheck();

	double GetDeltaTime()
	{
		return DeltaTime;
	}

protected:

private:
	LARGE_INTEGER Count;     // 1�ʿ� �� ī��Ʈ�� ������
	double dCount;			 // Count�� LARGE_INTEGER Ÿ������  
	__int64 Count64;

	LARGE_INTEGER PrevTime;  // ���� ���� �ð�
	LARGE_INTEGER CurTime;   // ���� ���� �ð�
	double DeltaTime;
};

