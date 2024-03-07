#pragma once
#include <EngineBase/EngineMath.h>
#include "UIElement.h"

enum class EFontColor
{
	Black,
	Black2,
	Black3,
	Gray,
	White,
	Blue,
	Red,
	WhiteGray
};

enum class EFontSize
{
	Normal,
	Mini
};

enum class EAlignType
{
	Left,		// Pivot과 텍스트의 좌하단을 맞춘다.    (RelativePos가 0일 때)
	Center,		// Pivot과 텍스트의 중앙 하단을 맞춘다. (RelativePos가 0일 때)
	Right		// Pivot과 텍스트의 우하단을 맞춘다.    (RelativePos가 0일 때)
};

// 텍스트를 그리는 액터.
// - 액터 위치 X값에 첫 글자의 왼쪽 끝을 맞춘다.
// - 액터 위치 Y값을 BaseLine의 높이로 두고 그린다.
class AText : public AUIElement
{
	class GlyphAlignRule;
	friend class AlignRuleMapInitiator;
private:
	// 글리프
	class GlyphAlignRule
	{
	public:
		std::string ImageName;
		int MarginLeft = 0;
		int MarginRight = 0;
		int Base = 0;
	};
	// AlignRuleMap[FontSize][WChar]
	static std::map<EFontSize, std::map<wchar_t, GlyphAlignRule>> AlignRuleMap;
	static void InitAlignRuleMap();
public:
	// constructor destructor
	AText();
	~AText();

	// delete Function
	AText(const AText& _Other) = delete;
	AText(AText&& _Other) noexcept = delete;
	AText& operator=(const AText& _Other) = delete;
	AText& operator=(AText&& _Other) noexcept = delete;

	void FollowParentPosition() override;

	// 초기화
	void SetAlignType(EAlignType _AlignType)
	{
		AlignType = _AlignType;
	}

	void SetColor(EFontColor _Color)
	{
		Color = _Color;
	}

	void SetSize(EFontSize _Size)
	{
		Size = _Size;
	}

	int GetLineSpace() const
	{
		return LineSpace;
	}

	void SetLineSpace(int _LineSpace)
	{
		LineSpace = _LineSpace;
	}

	int GetLineCount() const
	{
		return static_cast<int>(Lines.size());
	}

	int GetLineGlyphCount(int _LineIndex) const
	{
		int Count = 0;
		for (wchar_t Glyph : Lines[_LineIndex])
		{
			if (Glyph == L'\t' || Glyph == L'\n')
			{
				continue;
			}

			++Count;
		}

		return Count;
	}

	/*
	* ABCDE
	* FGHI
	* 
	* FindLineOfGlyph(4) = 0
	* FindLineOfGlyph(5) = 1
	* FindLineOfGlyph(9) = -1
	*/
	int FindLineOfGlyph(int _GlyphIndex) const
	{
		int CurGlyphIndex = _GlyphIndex;
		for (int i = 0; i < Lines.size(); ++i)
		{
			int LineGlyphCount = GetLineGlyphCount(i);
			CurGlyphIndex -= LineGlyphCount;

			if (CurGlyphIndex < 0)
			{
				return i;
			}
		}

		return -1;
	}

	int GetAllGlyphsCount() const
	{
		return static_cast<int>(GlyphRenderers.size());
	}

	int GetPixelLineWidth(int _LineIndex) const
	{
		return LineWidths[_LineIndex] / Global::PixelSize;
	}

	// 렌더링
	void SetText(std::wstring_view _Text);

	// 모든 문자를 켜고 끈다.
	void SetAllGlyphsActive(bool _Value);

	// 특정 인덱스의 글자 렌더러를 켜고 끈다.
	void SetGlyphActive(int _Index, bool _Value);

	// 특정 라인의 글자 렌더러를 전부 켜고 끈다.
	void SetLineActive(int _Index, bool _Value);

protected:
	void BeginPlay() override;
	//void Tick(float _DeltaTime) override;
private:
	// 데이터
	std::vector<std::wstring> Lines;
	std::vector<int> LineWidths;
	std::vector<UImageRenderer*> GlyphRenderers;

	// 설정값
	EAlignType AlignType = EAlignType::Left;
	EFontColor Color = EFontColor::White;
	EFontSize Size = EFontSize::Normal;
	int LineSpace = 14;
	FVector CutLeftTop;
	FVector CutScale;

	// 유틸 함수
	std::string ResolveImageNamePrefix() const;
	void PrepareLine(const std::wstring& _Line, int _Bot);
	
	// SetActorLocation 대신 SetRelativePosition 함수를 사용해야 한다.
	void SetActorLocation(FVector _Pos) 
	{
		AActor::SetActorLocation(_Pos);
	};
};

