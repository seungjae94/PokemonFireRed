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

	FVector GetWorldPos() const
	{
		return WorldPos;
	}

	void SetWorldPos(const FVector& _WorldPos)
	{
		WorldPos = _WorldPos;
	}

	void SetPlayer(APlayer* _Player)
	{
		Player = _Player;
	}

	void SetBackgroundImage(std::string_view _Name);

	void SetForegroundImage(std::string_view _Name);

	void SetCollisionImage(std::string_view _Name);

	UWindowImage* GetCollisionImage()
	{
		return CollisionRenderer->GetImage();
	}

	bool IsCollisionRendererActive()
	{
		return CollisionRenderer->IsActive();
	}

	void SetCollisionRendererActive(bool _Value)
	{
		CollisionRenderer->SetActive(_Value);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UImageRenderer* BackgroundRenderer = nullptr;
	UImageRenderer* ForegroundRenderer = nullptr;
	UImageRenderer* CollisionRenderer = nullptr;

	FVector TileCount = FVector::Zero;

	/// <summary>
	/// 맵의 월드 좌표
	/// 맵 이미지의 좌상단을 기준으로 한다.
	/// </summary>
	FVector WorldPos = FVector::Zero;

	// 배경의 크기
	FVector RenderScale = FVector::Zero;

	// 플레이어
	APlayer* Player = nullptr;
	
	/// <summary>
	/// 플레이어의 월드 좌표를 기준으로 스크린 좌표를 갱신한다.
	/// </summary>
	void SyncGroundScreenPosToPlayerWorldPos();
};

