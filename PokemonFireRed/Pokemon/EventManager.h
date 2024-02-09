#pragma once
#include <vector>
#include "PokemonMath.h"

class AMovable;

class UEventManager
{
public:
	// constructor destructor
	UEventManager();
	~UEventManager();

	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

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

