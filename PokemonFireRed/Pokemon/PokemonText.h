#pragma once
#include <EngineCore/Actor.h>

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
class APokemonText : public AActor
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
			//GlyphImageNamePrefix = "BlackGlyph";
			break;
		case EFontColor::Gray:
			//GlyphImageNamePrefix = "GrayGlyph";
			break;
		case EFontColor::White:
			GlyphImageNamePrefix = "WhiteGlyph";
			break;
		case EFontColor::Blue:
			//GlyphImageNamePrefix = "BlueGlyph";
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

	void SetLines(const std::vector<std::wstring>& _Lines, int LineSpace = 14);

	void SetVisible();
	void SetInvisible();

protected:
	
private:
	class GlyphAlignRule
	{
	public:
		std::string ImageName;
		int Width = 0;
		int Height = 0;
		int Base = 0;
	};

	EFontColor Color = EFontColor::White;
	std::string GlyphImageNamePrefix = "WhiteGlyph";
	bool IsSequential = false;

	static std::map<wchar_t, GlyphAlignRule> AlignRuleMap;

	std::vector<std::wstring> Lines;
	std::vector<UImageRenderer*> GlyphRenderers;

	static void InitAlignRuleMap();

	void PrepareLine(const std::wstring& _Line, int _Bot);
};

