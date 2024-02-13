#include "PokemonText.h"
#include "Global.h"

std::map<wchar_t, APokemonText::GlyphAlignRule> APokemonText::AlignRuleMap;

APokemonText::APokemonText()
{
}

APokemonText::~APokemonText()
{
}

void APokemonText::SetLines(const std::vector<std::wstring>& _Lines, int LineSpace)
{
	if (GlyphRenderers.size() > 0)
	{
		for (UImageRenderer* Renderer : GlyphRenderers)
		{
			Renderer->Destroy();
		}
		GlyphRenderers.clear();
	}

	FVector Pos = GetActorLocation();

	int Bot = 0;

	Lines = _Lines;

	for (std::wstring& Line : Lines)
	{
		PrepareLine(Line, Bot);
		Bot += LineSpace * Global::PIXEL_SIZE;
	}
}

void APokemonText::PrepareLine(const std::wstring& _Line, int _Bot)
{
	int Left = 0;
	for (const wchar_t& Ch : _Line)
	{
		const GlyphAlignRule& Rule = AlignRuleMap[Ch];

		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		Renderer->SetImage(GlyphImageNamePrefix + Rule.ImageName + ".png");
		Renderer->CameraEffectOff();

		FVector RenderPos = FVector(Left + Global::F_PIXEL_SIZE * Rule.Width / 2, _Bot - Global::F_PIXEL_SIZE * Rule.Height / 2 + Global::F_PIXEL_SIZE * Rule.Base);
		FVector GlyphScale = { Rule.Width, Rule.Height };
		FVector RenderScale = GlyphScale * Global::F_PIXEL_SIZE;
		Renderer->SetTransform({ RenderPos, RenderScale });
		Left += Rule.Width * Global::PIXEL_SIZE;

		GlyphRenderers.push_back(Renderer);
	}
}

void APokemonText::SetVisible()
{
	ActiveOn();

	for (UImageRenderer* Renderer : GlyphRenderers)
	{
		Renderer->ActiveOn();
	}
}

void APokemonText::SetInvisible()
{
	ActiveOff();

	for (UImageRenderer* Renderer : GlyphRenderers)
	{
		Renderer->ActiveOff();
	}
}


void APokemonText::InitAlignRuleMap()
{
	int Left = 72;
	int Top = 31;
	char NameCh = '0';
	for (wchar_t Ch = L'0'; Ch <= L'9'; Ch++)
	{
		AlignRuleMap[Ch] = { std::string("Number") + NameCh, 6, 10, 0 };
		Left += 6;
		NameCh++;
	}

	// 영 대문자
	Left = 64;
	Top = 46;
	NameCh = 'A';
	for (wchar_t Ch = L'A'; Ch <= L'Z'; Ch++)
	{
		AlignRuleMap[Ch] = { std::string("Upper") + NameCh, 6, 9, 0 };
		Left += 6;
		NameCh++;
	}

	// 영 소문자
	AlignRuleMap[L'a'] = { "LowerA", 6, 6, 0 };
	AlignRuleMap[L'b'] = { "LowerB", 6, 9, 0 };
	AlignRuleMap[L'c'] = { "LowerC", 6, 6, 0 };
	AlignRuleMap[L'd'] = { "LowerD", 6, 9, 0 };
	AlignRuleMap[L'e'] = { "LowerE", 6, 6, 0 };
	AlignRuleMap[L'f'] = { "LowerF", 5, 9, 0 };
	AlignRuleMap[L'g'] = { "LowerG", 6, 8, 2 };
	AlignRuleMap[L'h'] = { "LowerH", 6, 9, 0 };
	AlignRuleMap[L'i'] = { "LowerI", 4, 9, 0 };
	AlignRuleMap[L'j'] = { "LowerJ", 6, 11, 2 };
	AlignRuleMap[L'k'] = { "LowerK", 5, 9, 0 };
	AlignRuleMap[L'l'] = { "LowerL", 5, 9, 0 };
	AlignRuleMap[L'm'] = { "LowerM", 6, 6, 0 };
	AlignRuleMap[L'n'] = { "LowerN", 5, 6, 0 };
	AlignRuleMap[L'o'] = { "LowerO", 6, 6, 0 };
	AlignRuleMap[L'p'] = { "LowerP", 6, 8, 2 };
	AlignRuleMap[L'q'] = { "LowerQ", 6, 8, 2 };
	AlignRuleMap[L'r'] = { "LowerR", 5, 6, 0 };
	AlignRuleMap[L's'] = { "LowerS", 5, 6, 0 };
	AlignRuleMap[L't'] = { "LowerT", 5, 8, 0 };
	AlignRuleMap[L'u'] = { "LowerU", 6, 6, 0 };
	AlignRuleMap[L'v'] = { "LowerV", 6, 6, 0 };
	AlignRuleMap[L'w'] = { "LowerW", 6, 6, 0 };
	AlignRuleMap[L'x'] = { "LowerX", 6, 6, 0 };
	AlignRuleMap[L'y'] = { "LowerY", 6, 8, 2 };
	AlignRuleMap[L'z'] = { "LowerZ", 6, 6, 0 };

	// 특수 문자
	AlignRuleMap[L' '] = { "SpecialSpace", 6, 6, 0 };
	AlignRuleMap[L'.'] = { "SpecialPeriod", 4, 3, 0 };
	AlignRuleMap[L','] = { "SpecialComma", 4, 5, 2 };
	AlignRuleMap[L'!'] = { "SpecialExclamation", 6, 11, 0 };
	AlignRuleMap[L'?'] = { "SpecialQuestion", 6, 11, 0 };
	AlignRuleMap[L'\''] = { "SpecialApostrophe", 3, 5, -7 };
	AlignRuleMap[L':'] = { "SpecialColon", 5, 9, 0 };

	// 라틴 소문자
	AlignRuleMap[L'é'] = { "LatinLowerE", 6, 9, 0 };
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