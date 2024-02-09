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
	/// 액터를 지정한 경로를 따라 이동시킨다.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	/// <param name="_Actor">AMovable 타입의 액터</param>
	/// <param name="_Path">이동 경로</param>
	/// <returns>EventEnd 이벤트 종료 여부</returns>
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
	// 타이머
	float Timer = 0.0f;

	// 이동 관련
	int MoveActorIndex = -1;
	float MoveTime = 0.0f;
	
};

