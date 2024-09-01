#pragma once
#include "UIElement.h"
#include "Global.h"

enum class EBarType
{
	Hp,
	Exp,
};

class ABar : public AUIElement
{
public:
	// constructor destructor
	ABar();
	~ABar();

	// delete Function
	ABar(const ABar& _Other) = delete;
	ABar(ABar&& _Other) noexcept = delete;
	ABar& operator=(const ABar& _Other) = delete;
	ABar& operator=(ABar&& _Other) noexcept = delete;

	void SetBarType(EBarType _BarType)
	{
		BarType = _BarType;
	}

	void SetMaxValue(int _MaxValue)
	{
		MaxValue = _MaxValue;
	}

	void SetValue(int _Value)
	{
		switch (BarType)
		{
		case EBarType::Hp:
			DrawHp(_Value);
			break;
		case EBarType::Exp:
			DrawExp(_Value);
			break;
		default:
			break;
		}
	}

protected:

private:
	std::vector<UImageRenderer*> Renderers;

	int MaxValue = 0;
	EBarType BarType = EBarType::Hp;
	void DrawHp(int _Value);
	void DrawExp(int _Value);
};

