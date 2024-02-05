#pragma once
#include <EngineCore/Actor.h>

class APlayer;

// ���� ���� ����
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
	/// ���� ���� ��ǥ
	/// �� �̹����� �»���� �������� �Ѵ�.
	/// </summary>
	FVector WorldPos = FVector::Zero;

	// ����� ũ��
	FVector RenderScale = FVector::Zero;

	// �÷��̾�
	APlayer* Player = nullptr;
	
	/// <summary>
	/// �÷��̾��� ���� ��ǥ�� �������� ��ũ�� ��ǥ�� �����Ѵ�.
	/// </summary>
	void SyncGroundScreenPosToPlayerWorldPos();
};

