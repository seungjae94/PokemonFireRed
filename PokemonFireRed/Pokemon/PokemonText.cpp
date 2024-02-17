#include "PokemonText.h"
#include "Global.h"
#include "PokemonUtil.h"

std::map<wchar_t, APokemonText::GlyphAlignRule> APokemonText::AlignRuleMap;

APokemonText::APokemonText()
{
}

APokemonText::~APokemonText()
{
}

void APokemonText::SetText(const std::wstring& _Text, bool _IsVisible)
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

	FVector Pos = GetActorLocation();

	int Bot = 0;

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
		if (true == _IsVisible)
		{
			SetVisible();
		}
		RenderEnd = true;
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
		Renderer->SetActive(false);

		FVector RenderPos = FVector(Left + Global::FloatPixelSize * (Rule.Width / 2), _Bot - Global::FloatPixelSize * ((Rule.Height-1) / 2) + Global::FloatPixelSize * Rule.Base);
		FVector GlyphScale = { Rule.Width, Rule.Height };
		FVector RenderScale = GlyphScale * Global::FloatPixelSize;
		Renderer->SetTransform({ RenderPos, RenderScale });
		Left += Rule.Width * Global::PixelSize;

		GlyphRenderers.push_back(Renderer);
	}
}

void APokemonText::SetVisible()
{
	ActiveOn();
	AllRenderersActiveOn();
}

void APokemonText::SetInvisible()
{
	ActiveOff();
	AllRenderersActiveOff();
}

void APokemonText::Tick(float _DeltaTime)
{
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
	char NameCh = '0';
	for (wchar_t Ch = L'0'; Ch <= L'9'; Ch++)
	{
		AlignRuleMap[Ch] = { std::string("Number") + NameCh, 6, 10, 0 };
		NameCh++;
	}

	// 영 대문자
	NameCh = 'A';
	for (wchar_t Ch = L'A'; Ch <= L'Z'; Ch++)
	{
		AlignRuleMap[Ch] = { std::string("Upper") + NameCh, 6, 9, 0 };
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
	AlignRuleMap[L'!'] = { "SpecialExclamation", 4, 11, 0 };
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