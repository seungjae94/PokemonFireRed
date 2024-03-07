#include "Text.h"
#include "Global.h"
#include "PokemonUtil.h"
#include "PokemonString.h"
#include <EnginePlatform/EngineInput.h>

std::map<EFontSize, std::map<wchar_t, AText::GlyphAlignRule>> AText::AlignRuleMap;

AText::AText()
{
}

AText::~AText()
{
}

void AText::FollowParentPosition()
{
	AUIElement::FollowParentPosition();

	if (true == Lines.empty())
	{
		return;
	}

	FVector AbsolutePos = Pivot + RelativePos;
	switch (AlignType)
	{
	case EAlignType::Left:
		break;
	case EAlignType::Center:
	{
		int PixelLineWidth = LineWidths[0] / Global::PixelSize;
		AbsolutePos -= UPokemonUtil::PixelVector(PixelLineWidth / 2, 0);
	}
	break;
	case EAlignType::Right:
		AbsolutePos -= FVector(LineWidths[0] - Global::PixelSize, 0);
		break;
	default:
		break;
	}

	SetActorLocation(AbsolutePos);
}

void AText::SetText(std::wstring_view _Text)
{
	Lines = UPokemonString::Split(_Text, L'\n');
	LineWidths.clear();

	if (GlyphRenderers.size() > 0)
	{
		for (UImageRenderer* Renderer : GlyphRenderers)
		{
			Renderer->Destroy();
		}
		GlyphRenderers.clear();
	}

	int Bot = 0;
	for (std::wstring& Line : Lines)
	{
		PrepareLine(Line, Bot);
		Bot += LineSpace * Global::PixelSize;
	}

	FollowParentPosition();
}

void AText::SetAllGlyphsActive(bool _Value)
{
	for (UImageRenderer* Glyph : GlyphRenderers)
	{
		Glyph->SetActive(_Value);
	}
}

void AText::SetGlyphActive(int _Index, bool _Value)
{
	if (_Index >= GlyphRenderers.size())
	{
		MsgBoxAssert("AText에서 글리프 인덱스를 벗어난 글리프를 켜거나 끄려고 했습니다.");
		return;
	}

	GlyphRenderers[_Index]->SetActive(_Value);
}

void AText::SetLineActive(int _Index, bool _Value)
{
	if (_Index >= Lines.size())
	{
		MsgBoxAssert("AText에서 라인 인덱스를 벗어난 라인을 켜거나 끄려고 했습니다.");
		return;
	}

	int Acc = 0;
	for (int i = 0; i < _Index; ++i)
	{
		Acc += GetLineGlyphCount(i);
	}
	
	for (int i = Acc; i < Acc + GetLineGlyphCount(_Index); ++i)
	{
		SetGlyphActive(i, _Value);
	}
}

std::string AText::ResolveImageNamePrefix() const
{
	std::string ImageNamePrefix = "";
	if (Size == EFontSize::Mini)
	{
		ImageNamePrefix = "Mini";
	}

	switch (Color)
	{
	case EFontColor::Black:
		ImageNamePrefix += "Black";
		break;
	case EFontColor::Black2:
		ImageNamePrefix += "Black2";
		break;
	case EFontColor::Black3:
		ImageNamePrefix += "Black3";
		break;
	case EFontColor::Gray:
		ImageNamePrefix += "Gray";
		break;
	case EFontColor::White:
		ImageNamePrefix += "White";
		break;
	case EFontColor::Blue:
		ImageNamePrefix += "Blue";
		break;
	case EFontColor::Red:
		ImageNamePrefix += "Red";
		break;
	case EFontColor::WhiteGray:
		ImageNamePrefix += "WhiteGray";
		break;
	default:
		break;
	}

	ImageNamePrefix += "Glyph";

	return ImageNamePrefix;
}

void AText::PrepareLine(const std::wstring& _Line, int _Bot)
{
	std::string ImageNamePrefix = ResolveImageNamePrefix();

	int Left = 0;
	int Bot = _Bot;
	for (const wchar_t& Ch : _Line)
	{
		if (Ch == L'\t' || Ch == L'\n')
		{
			continue;
		}

		const GlyphAlignRule& Rule = AlignRuleMap[Size][Ch];

		UImageRenderer* Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->SetImage(ImageNamePrefix + Rule.ImageName + ".png");
		Renderer->CameraEffectOff();

		FVector ImageScale = UPokemonUtil::GetImageScale(Renderer);
		FVector RenderScale = UPokemonUtil::GetRenderScale(Renderer);
		int ImageWidth = ImageScale.iX();
		int ImageHeight = ImageScale.iY();
		int Width = Rule.MarginLeft + ImageWidth + Rule.MarginRight;

		FVector RenderPos = FVector(Left, Bot);									// 글리프 중심이 (Left, Bot)에 위치
		RenderPos += RenderScale.Half2D();
		RenderPos -= FVector(0.0f, RenderScale.Y - 3.0f);						// 글리프 좌하단이 (Left, Bot)에 위치
		RenderPos += UPokemonUtil::PixelVector(Rule.MarginLeft, Rule.Base);		// 마진 및 Baseline 적용

		Renderer->SetTransform({ RenderPos, RenderScale });
		Left += Width * Global::PixelSize;

		GlyphRenderers.push_back(Renderer);
	}
	
	LineWidths.push_back(Left);
}

void AText::BeginPlay()
{
	AUIElement::BeginPlay();
	SetActive(false);
}

void AText::InitAlignRuleMap()
{
	char NameCh = '0';
	for (wchar_t Ch = L'0'; Ch <= L'9'; Ch++)
	{
		AlignRuleMap[EFontSize::Normal][Ch] = { std::string("Number") + NameCh, 0, 0, 0 };
		AlignRuleMap[EFontSize::Mini][Ch] = { std::string("Number") + NameCh, 0, 0, 0 };
		NameCh++;
	}

	AlignRuleMap[EFontSize::Normal][L'1'].MarginLeft = 1;
	AlignRuleMap[EFontSize::Mini][L'1'].MarginLeft = 1;

	// 영 대문자
	NameCh = 'A';
	for (wchar_t Ch = L'A'; Ch <= L'Z'; Ch++)
	{
		AlignRuleMap[EFontSize::Normal][Ch] = { std::string("Upper") + NameCh, 0, 0, 0 };
		AlignRuleMap[EFontSize::Mini][Ch] = { std::string("Upper") + NameCh, 0, 0, 0 };
		NameCh++;
	}

	// 영 소문자
	AlignRuleMap[EFontSize::Normal][L'a'] = { "LowerA", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'b'] = { "LowerB", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'c'] = { "LowerC", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'd'] = { "LowerD", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'e'] = { "LowerE", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'f'] = { "LowerF", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'g'] = { "LowerG", 0, 0, 2 };
	AlignRuleMap[EFontSize::Normal][L'h'] = { "LowerH", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'i'] = { "LowerI", 1, 1, 0 };
	AlignRuleMap[EFontSize::Normal][L'j'] = { "LowerJ", 0, 1, 2 };
	AlignRuleMap[EFontSize::Normal][L'k'] = { "LowerK", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'l'] = { "LowerL", 1, 1, 0 };
	AlignRuleMap[EFontSize::Normal][L'm'] = { "LowerM", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'n'] = { "LowerN", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'o'] = { "LowerO", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'p'] = { "LowerP", 0, 0, 2 };
	AlignRuleMap[EFontSize::Normal][L'q'] = { "LowerQ", 0, 0, 2 };
	AlignRuleMap[EFontSize::Normal][L'r'] = { "LowerR", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L's'] = { "LowerS", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L't'] = { "LowerT", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'u'] = { "LowerU", 0, 1, 0 };
	AlignRuleMap[EFontSize::Normal][L'v'] = { "LowerV", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'w'] = { "LowerW", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'x'] = { "LowerX", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'y'] = { "LowerY", 0, 0, 2 };
	AlignRuleMap[EFontSize::Normal][L'z'] = { "LowerZ", 0, 0, 0 };

	AlignRuleMap[EFontSize::Mini][L'a'] = { "LowerA", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'b'] = { "LowerB", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'c'] = { "LowerC", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'd'] = { "LowerD", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'e'] = { "LowerE", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'f'] = { "LowerF", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'g'] = { "LowerG", 0, 0, 2 };
	AlignRuleMap[EFontSize::Mini][L'h'] = { "LowerH", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'i'] = { "LowerI", 1, 1, 0 };
	AlignRuleMap[EFontSize::Mini][L'j'] = { "LowerJ", 0, 0, 2 };
	AlignRuleMap[EFontSize::Mini][L'k'] = { "LowerK", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'l'] = { "LowerL", 1, 1, 0 };
	AlignRuleMap[EFontSize::Mini][L'm'] = { "LowerM", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'n'] = { "LowerN", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'o'] = { "LowerO", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'p'] = { "LowerP", 0, 0, 2 };
	AlignRuleMap[EFontSize::Mini][L'q'] = { "LowerQ", 0, 0, 2 };
	AlignRuleMap[EFontSize::Mini][L'r'] = { "LowerR", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L's'] = { "LowerS", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L't'] = { "LowerT", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'u'] = { "LowerU", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'v'] = { "LowerV", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'w'] = { "LowerW", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'x'] = { "LowerX", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'y'] = { "LowerY", 0, 0, 2 };
	AlignRuleMap[EFontSize::Mini][L'z'] = { "LowerZ", 0, 0, 0 };

	// 특수 문자
	AlignRuleMap[EFontSize::Normal][L'.'] = { "SpecialPeriod", 1, 1, 0 };
	AlignRuleMap[EFontSize::Normal][L','] = { "SpecialComma", 1, 1, 2 };
	AlignRuleMap[EFontSize::Normal][L'!'] = { "SpecialExclamation", 1, 1, 0 };
	AlignRuleMap[EFontSize::Normal][L'?'] = { "SpecialQuestion", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'\''] = { "SpecialApostrophe", 0, 0, -7 };
	AlignRuleMap[EFontSize::Normal][L':'] = { "SpecialColon", 1, 1, 0 };
	AlignRuleMap[EFontSize::Normal][L'…'] = { "SpecialHorizontalEllipsis", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'“'] = { "SpecialLeftDoubleQuotation", 0, 0, -7 };
	AlignRuleMap[EFontSize::Normal][L'”'] = { "SpecialRightDoubleQuotation", 0, 0, -7 };
	AlignRuleMap[EFontSize::Normal][L'‘'] = { "SpecialLeftSingleQuotation", 0, 0, -7 };
	AlignRuleMap[EFontSize::Normal][L'’'] = { "SpecialRightDoubleQuotation", 0, 0, -7 };
	AlignRuleMap[EFontSize::Normal][L'/'] = { "SpecialSlash", 0, 0, 0 };
	AlignRuleMap[EFontSize::Normal][L'-'] = { "SpecialMinus", 0, 0, -4 };
	AlignRuleMap[EFontSize::Normal][L' '] = { "SpecialSpace", 0, 0, 0 };

	AlignRuleMap[EFontSize::Mini][L'.'] = { "SpecialPeriod", 1, 1, 0 };
	AlignRuleMap[EFontSize::Mini][L','] = { "SpecialComma", 1, 1, 2 };
	AlignRuleMap[EFontSize::Mini][L'!'] = { "SpecialExclamation", 1, 1, 0 };
	AlignRuleMap[EFontSize::Mini][L'?'] = { "SpecialQuestion", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'ℓ'] = { "SpecialLv", 0, 0, 0 };
	AlignRuleMap[EFontSize::Mini][L'-'] = { "SpecialMinus", 0, 0, -4 };
	AlignRuleMap[EFontSize::Mini][L' '] = { "SpecialSpace", 0, 0, 0 };

	// 라틴 소문자
	AlignRuleMap[EFontSize::Normal][L'é'] = { "LatinLowerE", 0, 0, 0 };
}

class AlignRuleMapInitiator
{
public:
	AlignRuleMapInitiator()
	{
		AText::InitAlignRuleMap();
	}
};
AlignRuleMapInitiator AlignRuleMapInitiatorObject = AlignRuleMapInitiator();