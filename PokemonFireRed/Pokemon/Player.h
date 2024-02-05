#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class AMap;

enum class EPlayerState
{
	Idle,
	Walk,
	Jump,		// Ledge�� ���� ��
};

class APlayer : public AActor
{
public:
	// constructor destructor
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	FIntPoint GetWorldPos() const
	{
		return WorldPos;
	}

	void SetMap(AMap* _Map)
	{
		Map = _Map;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// FSM (Finite State Machine)
	void StateUpdate(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Walk(float _DeltaTime);
	void Jump(float _DeltaTime) = delete;

	// �浹 üũ
	bool CheckCollision();
private:
	UImageRenderer* Renderer = nullptr;

	// ���� ��ǥ
	FIntPoint WorldPos = FIntPoint::Zero;
	FIntPoint Direction = FIntPoint::Down;

	// ����
	AMap* Map = nullptr;
	
	// ���� ����
	EPlayerState State = EPlayerState::Idle;

	// �ȱ� �ִϸ��̼� �����Ӵ� ��� �ð�
	float WalkInterval = 1.0f / 6;

	// �̵� ����
	float speed = 3.6f;
	float WalkTime = 1.0f / speed;
	bool IsMoving = false;
};

