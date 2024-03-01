#pragma once
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
public:
	// constructor destructor
	AText();
	~AText();

	// delete Function
	AText(const AText& _Other) = delete;
	AText(AText&& _Other) noexcept = delete;
	AText& operator=(const AText& _Other) = delete;
	AText& operator=(AText&& _Other) noexcept = delete;

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

	void FollowParentPosition() override;

	void SetLineSpace(int _LineSpace)
	{
		LineSpace = _LineSpace;
	}

	void SetText(std::wstring_view _Text);

	int GetPixelLineWidth() const
	{
		return LineWidth / Global::PixelSize;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	class GlyphAlignRule
	{
	public:
		std::string ImageName;
		int MarginLeft = 0;
		int MarginRight = 0;
		int Base = 0;
	};

	EAlignType AlignType = EAlignType::Left;
	EFontColor Color = EFontColor::White;
	EFontSize Size = EFontSize::Normal;
	int LineSpace = 14;

	// 텍스트를 천천히 출력할 때 사용
	bool IsSequential = false;
	float CharShowInterval = 0.05f;
	float CurCharShowInterval = CharShowInterval;
	int CharShowIndex = 0;

	// 렌더링 완료 여부 (스킵에 사용)
	bool RenderEnd = false;

	// AlignRuleMap[FontSize][WChar]
	static std::map<EFontSize, std::map<wchar_t, GlyphAlignRule>> AlignRuleMap;

	std::vector<std::wstring> Lines;
	std::vector<UImageRenderer*> GlyphRenderers;

	static void InitAlignRuleMap();

	void PrepareLine(const std::wstring& _Line, int _Bot);

	// 텍스트 정렬 기능
	int LineWidth = 0;

	void SetActorLocation(FVector _Pos) 
	{
		AActor::SetActorLocation(_Pos);
	};
};

