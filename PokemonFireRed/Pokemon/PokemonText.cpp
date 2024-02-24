#include "PokemonText.h"
#include "Global.h"
#include "PokemonUtil.h"

std::map<EFontSize, std::map<wchar_t, APokemonText::GlyphAlignRule>> APokemonText::AlignRuleMap;

APokemonText::APokemonText()
{
}

APokemonText::~APokemonText()
{
}

void APokemonText::FollowContainer()
{
	AUIElement::FollowContainer();

	FVector AbsolutePos = Pivot + RelativePos;
	switch (AlignType)
	{
	case EAlignType::Left:
		break;
	case EAlignType::Center:
	{
		int PixelLineWidth = LineWidth / Global::PixelSize;
		AbsolutePos -= UPokemonUtil::PixelVector(PixelLineWidth / 2, 0);
	}
	break;
	case EAlignType::Right:
		AbsolutePos -= FVector(LineWidth - Global::PixelSize, 0);
		break;
	default:
		break;
	}

	SetActorLocation(AbsolutePos);
}

void APokemonText::SetText(std::wstring_view _Text)
{
	std::vector<std::wstring> Lines = UPokemonUtil::StringSplit(_Text, L'\n');

	if (GlyphRenderers.size() > 0)
	{
		for (UImageRenderer* Renderer : GlyphRenderers)
		{
			Renderer->Destroy();
		}
		GlyphRenderers.clear();
	}

	int Bot = 0;
	LineWidth = 0;

	for (std::wstring& Line : Lines)
	{
		PrepareLine(Line, Bot);
		Bot += LineSpace * Global::PixelSize;
	}

	if (true == IsSequential)
	{
		CharShowIndex = 0;
		CurCharShowInterval = 0.0f;
		RenderEnd = false;
		AllRenderersActiveOff();
	}
	else
	{
		RenderEnd = true;
	}
}

void APokemonText::PrepareLine(const std::wstring& _Line, int _Bot)
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

	int Left = 0;
	int Bot = _Bot;
	for (const wchar_t& Ch : _Line)
	{
		if (Ch == L' ')
		{
			int Width = 6;
			Left += Width * Global::PixelSize;
			continue;
		}

		if (Ch == L'\t' || Ch == L'\n')
		{
			continue;
		}

		const GlyphAlignRule& Rule = AlignRuleMap[Size][Ch];

		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::Text);
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
	LineWidth = max(Left, LineWidth);
}

void APokemonText::BeginPlay()
{
	AUIElement::BeginPlay();
	SetActive(false);
}

void APokemonText::Tick(float _DeltaTime)
{
	if (nullptr == Container)
	{
		MsgBoxAssert(GetWorld()->GetName() + ":" + GetName() + " 텍스트에 컨테이너를 설정하지 않았습니다.");
		return;
	}

	if (false == IsSequential || true == RenderEnd)
	{
		return;
	}

	if (CharShowIndex >= GlyphRenderers.size())
	{
		RenderEnd = true;
		return;
	}

	if (CurCharShowInterval > 0.0f)
	{
		CurCharShowInterval -= _DeltaTime;
		return;
	}

	CurCharShowInterval = CharShowInterval;
	GlyphRenderers[CharShowIndex]->ActiveOn();
	++CharShowIndex;
}

void APokemonText::InitAlignRuleMap()
{
	// string ImageName, int MarginLeft, int MarginRight, int Base

	char NameCh = '0';
	for (wchar_t Ch = L'0'; Ch <= L'9'; Ch++)
	{
		AlignRuleMap[EFontSize::Normal][Ch] = { std::string("Number") + NameCh, 0, 0, 0 };
		AlignRuleMap[EFontSize::Mini][Ch] = { std::string("Number") + NameCh, 0, 0, 0 };
		NameCh++;
	}

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

	AlignRuleMap[EFontSize::Mini][L'.'] = { "SpecialPeriod", 1, 1, 0 };
	AlignRuleMap[EFontSize::Mini][L','] = { "SpecialComma", 1, 1, 2 };
	AlignRuleMap[EFontSize::Mini][L'!'] = { "SpecialExclamation", 1, 1, 0 };
	AlignRuleMap[EFontSize::Mini][L'?'] = { "SpecialQuestion", 0, 0, 0 };

	// 라틴 소문자
	AlignRuleMap[EFontSize::Normal][L'é'] = { "LatinLowerE", 0, 0, 0 };
}

class AlignRuleMapInitiator
{
public:
	AlignRuleMapInitiator()
	{
		APokemonText::InitAlignRuleMap();
	}
};
AlignRuleMapInitiator AlignRuleMapInitiatorObject = AlignRuleMapInitiator();