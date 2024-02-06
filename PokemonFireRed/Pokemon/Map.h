#pragma once
#include <EngineCore/Actor.h>

class APlayer;

enum class EMapLedgeType
{
	Down,
	Up,
	Left,
	Right
};

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

	// 플레이어
	APlayer* Player = nullptr;
};

