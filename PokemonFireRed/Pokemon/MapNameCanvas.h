#pragma once
#include "Canvas.h"
#include "Text.h"
#include "Global.h"

enum class EMapNameCanvasState
{
	Hide,
	Open,
	Close,
	FastClose,
	Show,
};

class AMapNameCanvas : public ACanvas
{
public:
	// constructor destructor
	AMapNameCanvas();
	~AMapNameCanvas();

	// delete Function
	AMapNameCanvas(const AMapNameCanvas& _Other) = delete;
	AMapNameCanvas(AMapNameCanvas&& _Other) noexcept = delete;
	AMapNameCanvas& operator=(const AMapNameCanvas& _Other) = delete;
	AMapNameCanvas& operator=(AMapNameCanvas&& _Other) noexcept = delete;

	void Open(std::wstring_view _MapName);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	AImageElement* MapNameBox = nullptr;
	AText* MapNameText = nullptr;

	FVector HidePos = UPokemonUtil::PixelVector(0, -22);
	FVector ShowPos = UPokemonUtil::PixelVector(0, 0);

	EMapNameCanvasState State = EMapNameCanvasState::Hide;

	float ChangeTime = 1 / 3.0f;
	float FastChangeTime = 1 / 12.0f;
	float ShowTime = 3.0f;
	float CurChangeTime = 0.0f;

	void OpenTick(float _DeltaTime);
	void ShowTick(float _DeltaTime);
	void CloseTick(float _DeltaTime);
	void FastCloseTick(float _DeltaTime);

	bool WaitingReopen = false;
	std::wstring MemoryMapName = L"";

	// Hide = 0.0f, Show = 1.0f로 계산할 때 현재 위치
	float GetCurProp()
	{
		return (GetActorLocation().Y - HidePos.Y) / (ShowPos.Y - HidePos.Y);
	}
};

