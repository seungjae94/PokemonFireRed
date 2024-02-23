#pragma once
#include <EngineCore/Actor.h>
#include "PokemonText.h"
#include "ScrollBar.h"

class UEventProcessor;

class APage : public AActor
{
	friend UEventProcessor;
public:
	// constructor destructor
	APage();
	~APage();

	// delete Function
	APage(const APage& _Other) = delete;
	APage(APage&& _Other) noexcept = delete;
	APage& operator=(const APage& _Other) = delete;
	APage& operator=(APage&& _Other) noexcept = delete;

	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;

	APokemonText* CreateText(
		UImageRenderer* _Container,
		std::wstring _Text,
		EPivotType _PivotType = EPivotType::LeftTop,
		EAlignType _AlignType = EAlignType::Left,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0,
		EFontColor _Color = EFontColor::White,
		EFontSize _Size = EFontSize::Normal
	);

	AScrollBar* CreateScrollBar(
		UImageRenderer* _Container,
		EScrollType _ScrollType,
		int _CurValue,
		int _MaxValue,
		EPivotType _PivotType = EPivotType::LeftTop,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0
	);

	UImageRenderer* CreateLeftTopAlignedRenderer()
	{
		return nullptr;
	}

protected:
	virtual void Sync(APage* _Other) {}
private:
	std::list<APokemonText*> Texts;
	std::list<AScrollBar*> Bars;
};

