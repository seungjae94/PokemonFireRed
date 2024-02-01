#pragma once
#include <EngineCore/Actor.h>

class APlayer;

// 플레이어가 돌아다닐 바닥
class AGround : public AActor
{
public:
	// constructor destructor
	AGround();
	~AGround();

	// delete Function
	AGround(const AGround& _Other) = delete;
	AGround(AGround&& _Other) noexcept = delete;
	AGround& operator=(const AGround& _Other) = delete;
	AGround& operator=(AGround&& _Other) noexcept = delete;

	void SetPlayer(APlayer* _Player)
	{
		Player = _Player;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;

	/// <summary>
	/// 배경 이미지 좌상단(Left Top)의 월드 좌표
	/// </summary>
	const FVector LTWorldPos = { -6.5f, -8.5f };
	FVector TileCount = FVector::Zero;

	// 배경의 크기
	FVector RenderScale = FVector::Zero;

	// 플레이어
	APlayer* Player = nullptr;
	
	/// <summary>
	/// 플레이어의 월드 좌표를 기준으로 스크린 좌표를 갱신한다.
	/// </summary>
	void UpdateScreenPos();
};

