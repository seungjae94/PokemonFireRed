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
	__int64 Count64;
	LARGE_INTEGER Count;     // 1�ʿ� �� ī��Ʈ�� ������
	LARGE_INTEGER PrevTime;  // ���� ���� �ð�
	LARGE_INTEGER CurTime;   // ���� ���� �ð�
	double dCount;			 // Count�� LARGE_INTEGER Ÿ������  
	double DeltaTime;
};

