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

	void SetWorldPos(const FVector& _WorldPos)
	{
		LTWorldPos = _WorldPos;
	}

	void SetPlayer(APlayer* _Player)
	{
		Player = _Player;
	}

	void SetBackgroundImage(std::string_view _Name);

	void SetForegroundImage(std::string_view _Name);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UImageRenderer* BackgroundRenderer = nullptr;
	UImageRenderer* ForegroundRenderer = nullptr;

	FVector TileCount = FVector::Zero;

	/// <summary>
	/// ��� �̹��� �»��(Left Top)�� ���� ��ǥ
	/// </summary>
	FVector LTWorldPos = FVector::Zero;

	// ����� ũ��
	FVector RenderScale = FVector::Zero;

	// �÷��̾�
	APlayer* Player = nullptr;
	
	/// <summary>
	/// �÷��̾��� ���� ��ǥ�� �������� ��ũ�� ��ǥ�� �����Ѵ�.
	/// </summary>
	void SyncGroundScreenPosToPlayerWorldPos();
};

