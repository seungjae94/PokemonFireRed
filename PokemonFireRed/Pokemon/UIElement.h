#pragma once
#include <EngineCore/Actor.h>
#include "Global.h"
#include "PokemonMath.h"

enum class EPivotType
{
	LeftTop,
	LeftBot,
	RightTop,
	RightBot,
	CenterTop,
	CenterBot,
};

class AUIElement : public AActor
{
public:
	// constructor destructor
	AUIElement();
	~AUIElement();

	// delete Function
	AUIElement(const AUIElement& _Other) = delete;
	AUIElement(AUIElement&& _Other) noexcept = delete;
	AUIElement& operator=(const AUIElement& _Other) = delete;
	AUIElement& operator=(AUIElement&& _Other) noexcept = delete;

	virtual void SetRelativePos(FVector _PivotRelativePos);
	virtual void FollowContainer();
	
	void SetRenderingOrder(ERenderingOrder _Order)
	{
		RenderingOrder = _Order;
	}

protected:
	UImageRenderer* Container = nullptr;
	FVector Pivot = FVector::Zero;
	FVector RelativePos = FVector::Zero;
	EPivotType PivotType = EPivotType::LeftTop;
	ERenderingOrder RenderingOrder = ERenderingOrder::UI2;
private:

};

