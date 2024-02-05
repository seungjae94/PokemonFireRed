#pragma once
#include <EngineCore/Actor.h>

class AMap;

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

	void SetMap(AMap* _Map)
	{
		Map = _Map;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;

	// ���� ��ǥ
	FVector WorldPos = FVector::Zero;

	// ����
	AMap* Map = nullptr;
	
	// �̵� ����
	float speed = 3.6f;
	float WalkInterval = 1.0f / 6;		// �ȱ� �ִϸ��̼� ������ ��� �ð�

	float WalkTime = 1.0f / speed;
	float DelayInputTime = 1.0f / 10;
	FVector Direction = FVector::Zero;
	bool IsIdle = false;
};

