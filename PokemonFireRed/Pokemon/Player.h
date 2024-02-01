#pragma once
#include <EngineCore/Actor.h>

class AGround;

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

	FVector GetWorldPos() const
	{
		return WorldPos;
	}

	void SetGround(AGround* _Ground)
	{
		Ground = _Ground;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;

	// ���� ��ǥ
	FVector WorldPos = FVector::Zero;

	// ����
	AGround* Ground = nullptr;
	
	// �ִϸ��̼� ����
	float WalkInterval = 1.0f / 6;
	float WalkTime = 1.0f / 3;
	float CurWalkTime = WalkTime;
	FVector Direction = FVector::Zero;
	bool IsIdle = false;
};

