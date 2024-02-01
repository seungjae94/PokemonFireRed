#pragma once
#include <EngineCore/Actor.h>

class APlayer;

// �÷��̾ ���ƴٴ� �ٴ�
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
	void UpdateScreenPos();
};

