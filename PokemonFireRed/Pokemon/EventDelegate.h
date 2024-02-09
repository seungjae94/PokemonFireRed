#pragma once
#include <vector>
#include "PokemonMath.h"

class AMovable;

class UEventDelegate
{
public:
	// constructor destructor
	UEventDelegate();
	~UEventDelegate();

	// delete Function
	UEventDelegate(const UEventDelegate& _Other) = delete;
	UEventDelegate(UEventDelegate&& _Other) noexcept = delete;
	UEventDelegate& operator=(const UEventDelegate& _Other) = delete;
	UEventDelegate& operator=(UEventDelegate&& _Other) noexcept = delete;

	/// <summary>
	/// ���͸� ������ ��θ� ���� �̵���Ų��.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	/// <param name="_Actor">AMovable Ÿ���� ����</param>
	/// <param name="_Path">�̵� ���</param>
	/// <returns>EventEnd �̺�Ʈ ���� ����</returns>
	bool MoveActor(float _DeltaTime, AMovable* _Actor, std::vector<FTileVector> _Path, float _MoveSpeed = 3.6f);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_DeltaTime"></param>
	/// <param name="_WaitTime"></param>
	/// <returns></returns>
	//bool Wait(float _DeltaTime, float _WaitTime);


protected:

private:
	// Ÿ�̸�
	float Timer = 0.0f;

	// �̵� ����
	int MoveActorIndex = -1;
	float MoveTime = 0.0f;
	
};

