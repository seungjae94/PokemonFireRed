#pragma once
#include <EngineCore/Actor.h>

class APlayer;

// 맵의 배경과 전경
class AMap : public AActor
{
public:
	// constructor destructor
	AMap();
	~AMap();

	// delete Function
	AMap(const AMap& _Other) = delete;
	AMap(AMap&& _Other) noexcept = delete;
	AMap& operator=(const AMap& _Other) = delete;
	AMap& operator=(AMap&& _Other) noexcept = delete;

	void SetPlayer(APlayer* _Player)
	{
		Player = _Player;
	}

	void SetBackgroundImage(std::string_view _Name);

	void SetForegroundImage(std::string_view _Name)
	{
		ForegroundRenderer->SetImage(_Name, 1000);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* BackgroundRenderer = nullptr;
	UImageRenderer* ForegroundRenderer = nullptr;

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
	void SyncGroundScreenPosToPlayerWorldPos();
};

