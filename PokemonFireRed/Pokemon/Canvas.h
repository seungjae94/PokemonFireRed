#pragma once
#include <EngineCore/Actor.h>
#include "AllUIElements.h"
#include "UIParent.h"

class UEventProcessor;

class ACanvas : public AUIParent
{
	friend UEventProcessor;
public:
	// constructor destructor
	ACanvas();
	~ACanvas();

	// delete Function
	ACanvas(const ACanvas& _Other) = delete;
	ACanvas(ACanvas&& _Other) noexcept = delete;
	ACanvas& operator=(const ACanvas& _Other) = delete;
	ACanvas& operator=(ACanvas&& _Other) noexcept = delete;

	AText* CreateText(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		EAlignType _AlignType = EAlignType::Left,
		EFontColor _Color = EFontColor::White,
		EFontSize _Size = EFontSize::Normal
	);

	AScrollBar* CreateScrollBar(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		EScrollType _ScrollType
	);

	ACursor* CreateCursor(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		std::string _ImageName = RN::BlackCursor,
		int _PixelGap = 16
	);

	APokemonElement* CreatePokemonElement(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		EPokemonElementType _ElementType 
	);

	AImageElement* CreateImageElement(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY
	);

	FTransform GetUITransform() override;

	// 에러 해결용 임시 함수
	virtual void Sync(ACanvas* _Canvas) {};

protected:
private:
};

