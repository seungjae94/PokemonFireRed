#pragma once
#include "UIElement.h"

enum class EFontColor
{
	Black,
	Gray,
	White,
	Blue,
	Red
};

// 텍스트를 그리는 액터.
// - 액터 위치 X값에 첫 글자의 왼쪽 끝을 맞춘다.
// - 액터 위치 Y값을 BaseLine의 높이로 두고 그린다.
class APokemonText : public AUIElement
{
	class GlyphAlignRule;
	friend class AlignRuleMapInitiator;
public:
	// constructor destructor
	APokemonText();
	~APokemonText();

	// delete Function
	APokemonText(const APokemonText& _Other) = delete;
	APokemonText(APokemonText&& _Other) noexcept = delete;
	APokemonText& operator=(const APokemonText& _Other) = delete;
	APokemonText& operator=(APokemonText&& _Other) noexcept = delete;

	void SetColor(EFontColor _Color)
	{
		Color = _Color;

		switch (_Color)
		{
		case EFontColor::Black:
			GlyphImageNamePrefix = "BlackGlyph";
			break;
		case EFontColor::Gray:
			GlyphImageNamePrefix = "GrayGlyph";
			break;
		case EFontColor::White:
			GlyphImageNamePrefix = "WhiteGlyph";
			break;
		case EFontColor::Blue:
			GlyphImageNamePrefix = "BlueGlyph";
			break;
		case EFontColor::Red:
			GlyphImageNamePrefix = "RedGlyph";
			break;
		default:
			break;
		}
	}

	void SetSequential(bool _IsSequential)
	{
		IsSequential = _IsSequential;
	}

	bool IsRenderEnd() const
	{
		return RenderEnd;
	}

	void Skip()
	{
		RenderEnd = true;

		for (UImageRenderer* Renderer : GlyphRenderers)
		{
			Renderer->ActiveOn();
		}
	}

	void SetContainer(AActor* _Container)
	{
		Container = _Container;
	}

	void SetLineSpace(int _LineSpace)
	{
		LineSpace = _LineSpace;
	}

	void SetText(std::wstring_view _Text, bool _IsVisible = false);

	void SetVisible();
	void SetInvisible();

	int GetCenterAlignedX(int _RectLeft, int _RectWidth) const
	{
		return (_RectWidth - LineWidth) / 2;
	}

	void SetPosX(float _X)
	{
		FVector Pos = GetActorLocation();
		SetActorLocation(FVector(_X, Pos.X));
	}

	void SetPosX(int _X)
	{
		SetPosX(static_cast<float>(_X));
	}

protected:
	void Tick(float _DeltaTime) override;
private:
	class GlyphAlignRule
	{
	public:
		std::string ImageName;
		int Width = 0;
		int Height = 0;
		int Base = 0;
	};

	AActor* Container = nullptr;

	int LineSpace = 14;
	EFontColor Color = EFontColor::White;
	std::string GlyphImageNamePrefix = "WhiteGlyph";

	// 텍스트를 천천히 출력할 때 사용
	bool IsSequential = false;
	float CharShowInterval = 0.05f;
	float CurCharShowInterval = CharShowInterval;
	int CharShowIndex = 0;

	// 렌더링 완료 여부 (스킵에 사용)
	bool RenderEnd = false;

	static std::map<wchar_t, GlyphAlignRule> AlignRuleMap;

	std::vector<std::wstring> Lines;
	std::vector<UImageRenderer*> GlyphRenderers;

	static void InitAlignRuleMap();

	void PrepareLine(const std::wstring& _Line, int _Bot);

	// 텍스트 정렬 기능
	int LineWidth = 0;

};

