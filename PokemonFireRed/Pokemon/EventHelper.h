#pragma once
#include <vector>
#include "PokemonMath.h"

class AMovable;

class UEventHelper
{
public:
	// constructor destructor
	UEventHelper();
	~UEventHelper();

	// delete Function
	UEventHelper(const UEventHelper& _Other) = delete;
	UEventHelper(UEventHelper&& _Other) noexcept = delete;
	UEventHelper& operator=(const UEventHelper& _Other) = delete;
	UEventHelper& operator=(UEventHelper&& _Other) noexcept = delete;

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

