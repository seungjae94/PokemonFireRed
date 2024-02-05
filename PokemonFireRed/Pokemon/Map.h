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
	/// ��� �̹��� �»��(Left Top)�� ���� ��ǥ
	/// </summary>
	const FVector LTWorldPos = { -6.5f, -8.5f };
	FVector TileCount = FVector::Zero;

	// ����� ũ��
	FVector RenderScale = FVector::Zero;

	// �÷��̾�
	APlayer* Player = nullptr;
	
	/// <summary>
	/// �÷��̾��� ���� ��ǥ�� �������� ��ũ�� ��ǥ�� �����Ѵ�.
	/// </summary>
	void SyncGroundScreenPosToPlayerWorldPos();
};

