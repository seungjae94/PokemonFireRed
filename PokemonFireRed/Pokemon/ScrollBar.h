#pragma once
#include "UIElement.h"
#include "Global.h"

enum class EScrollType
{
	Hp,
	Exp,
};

enum class EPivotType;

class AScrollBar : public AUIElement
{
	friend class APage;
public:
	// constructor destructor
	AScrollBar();
	~AScrollBar();

	// delete Function
	AScrollBar(const AScrollBar& _Other) = delete;
	AScrollBar(AScrollBar&& _Other) noexcept = delete;
	AScrollBar& operator=(const AScrollBar& _Other) = delete;
	AScrollBar& operator=(AScrollBar&& _Other) noexcept = delete;

	void SetScrollType(EScrollType _ScrollType)
	{
		ScrollType = _ScrollType;
	}

	void SetMaxValue(int _MaxValue)
	{
		MaxValue = _MaxValue;
	}

	void SetValue(int _Value)
	{
		switch (ScrollType)
		{
		case EScrollType::Hp:
			DrawHp(_Value);
			break;
		case EScrollType::Exp:
			DrawExp(_Value);
			break;
		default:
			break;
		}
	}

	void SetRelativePos(FVector _PivotRelativePos);
	void FollowContainer();

protected:

private:
	UImageRenderer* Container = nullptr;
	FVector RelativePos = FVector::Zero;
	EPivotType PivotType = EPivotType::LeftTop;
	std::vector<UImageRenderer*> Renderers;

	int MaxValue = 0;
	EScrollType ScrollType = EScrollType::Hp;
	void DrawHp(int _Value);
	void DrawExp(int _Value);
};

