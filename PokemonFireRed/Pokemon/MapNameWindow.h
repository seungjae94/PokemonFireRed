#pragma once
#include "UIElement.h"
#include "PokemonText.h"
#include "Global.h"

enum class EMapNameWindowState
{
	Hide,
	Open,
	Close,
	Show,
};

class AMapNameWindow : public AUIElement
{
public:
	// constructor destructor
	AMapNameWindow();
	~AMapNameWindow();

	// delete Function
	AMapNameWindow(const AMapNameWindow& _Other) = delete;
	AMapNameWindow(AMapNameWindow&& _Other) noexcept = delete;
	AMapNameWindow& operator=(const AMapNameWindow& _Other) = delete;
	AMapNameWindow& operator=(AMapNameWindow&& _Other) noexcept = delete;

	void Open(std::wstring_view _MapName);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;
	APokemonText* MapNameText = nullptr;

	FVector HidePos;
	FVector ShowPos;

	EMapNameWindowState State = EMapNameWindowState::Hide;

	float ChangeSpeed = 5.0f;
	float ChangeTime = 1 / ChangeSpeed;
	float ShowTime = 3.0f;
	float CurChangeTime = 0.0f;

	void OpenTick(float _DeltaTime);
	void ShowTick(float _DeltaTime);
	void CloseTick(float _DeltaTime);

	bool WaitingReopen = false;
	std::wstring BackupString = L"";

	// Hide = 0.0f, Show = 1.0f로 계산할 때 현재 위치
	float GetCurProp()
	{
		return (GetActorLocation().Y - HidePos.Y) / (ShowPos.Y - HidePos.Y);
	}

	void UpdateTextPos()
	{
		FVector RendererScale = Renderer->GetTransform().GetScale();
		FVector ActorTopLeft = GetActorLocation() - RendererScale.Half2D();
		int TextX = MapNameText->GetCenterAlignedX(ActorTopLeft.iX(), RendererScale.iX());
		int TextY = ActorTopLeft.iY() + 13 * Global::FloatPixelSize;
		MapNameText->SetActorLocation({TextX, TextY});
	}
};

