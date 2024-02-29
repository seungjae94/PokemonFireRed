#pragma once
#include "UIParent.h"
#include "Global.h"
#include "PokemonMath.h"
#include "PokemonUtil.h"

enum class EPivotType
{
	LeftTop,
	LeftBot,
	RightTop,
	RightBot,
	CenterTop,
	CenterBot,
};

class AUIElement : public AUIParent
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

	void SetParent(AUIParent* _Parent)
	{
		Parent = _Parent;
	}

	void SetPivotType(EPivotType _PivotType)
	{
		PivotType = _PivotType;
	}

	void SetRenderingOrder(ERenderingOrder _Order)
	{
		RenderingOrder = _Order;
	}

	FVector GetRelativePosition() const
	{
		return RelativePos;
	}

	virtual void SetRelativePosition(FVector _RelativePos)
	{
		RelativePos = _RelativePos;
		FollowParentPosition();
	}

	virtual void SetRelativePosition(int _PixelX, int _PixelY)
	{
		SetRelativePosition(UPokemonUtil::PixelVector(_PixelX, _PixelY));
	}

	virtual void AddRelativePosition(FVector _RelativePos)
	{
		SetRelativePosition(RelativePos + _RelativePos);
	}

	virtual void AddRelativePosition(int _PixelX, int _PixelY)
	{
		AddRelativePosition(UPokemonUtil::PixelVector(_PixelX, _PixelY));
	}

	virtual void FollowParentPosition();

	bool GetIsFollowParentPosition() const
	{
		return IsFollowParentPosition;
	}

	void SetIsFollowParentPosition(bool _Value)
	{
		IsFollowParentPosition = _Value;
	}

	bool GetIsFollowParentActive() const
	{
		return IsFollowParentActive;
	}

	void SetIsFollowParentActive(bool _Value)
	{
		IsFollowParentPosition = _Value;
	}

protected:
	AUIParent* Parent = nullptr;
	ERenderingOrder RenderingOrder = ERenderingOrder::UI0;
	FVector Pivot = FVector::Zero;
	FVector RelativePos = FVector::Zero;
	EPivotType PivotType = EPivotType::LeftTop;

	bool IsFollowParentPosition = true;
	bool IsFollowParentActive = true;
private:

};

