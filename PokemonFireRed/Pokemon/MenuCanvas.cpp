#include "MenuCanvas.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "PokemonUtil.h"
#include "EventManager.h"
#include "Text.h"
#include "MapLevel.h"
#include "EventTrigger.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "PlayerData.h"

int AMenuCanvas::MenuCount = 3;

AMenuCanvas::AMenuCanvas()
{
}

AMenuCanvas::~AMenuCanvas()
{
}

void AMenuCanvas::Open()
{
	Refresh();
	SetActive(true);
	IsFirstTick = true;
}


void AMenuCanvas::Close()
{
	UEventManager::SetCurLevelPlayerState(EPlayerState::Idle);
	SetActive(false);
}

void AMenuCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// 메뉴창
	MenuBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::RightTop, -1, 2);

	for (int i = 0; i < 5; i++)
	{
		AText* MenuText = CreateText(MenuBox, ERenderingOrder::UI1, EPivotType::LeftTop, 15, 18 + 15 * i, EAlignType::Left, EFontColor::Gray);
		MenuTexts.push_back(MenuText);
	}

	// 메뉴창 커서
	Cursor = CreateCursor(MenuBox, ERenderingOrder::UI1, EPivotType::LeftTop, 8, 9, RN::BlackCursor, 15);

	// 하단 메뉴 설명창
	MenuExplainBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftBot, 0, 0);
	MenuExplainBox->SetImage(RN::MenuWindowExplain);
	MenuExplainText = CreateText(MenuExplainBox, ERenderingOrder::UI1, EPivotType::LeftTop, 2, 16, EAlignType::Left, EFontColor::White);

	// 초기 상태 설정
	MenuBox->SetActive(false);
	MenuExplainBox->SetActive(false);
}

void AMenuCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	// 이벤트 매니저가 MenuCanvas를 ActiveOn한 다음 틱부터 로직을 실행한다.
	// - ActiveOn한 틱에는 UEngineInput::IsDown(VK_RETURN) 값이 true로 들어가 있기 때문이다.
	if (IsFirstTick)
	{
		IsFirstTick = false;
		return;
	}

	// 윈도우 갱신
	if (true == UPlayerData::IsAchieved(EAchievement::GetPokedex) && MenuCount == 4)
	{
		MenuCount = 5;
		Cursor->SetOptionCount(MenuCount);
		RefreshMenuBox();
		RefreshExplainText();
	}
	else if (true == UPlayerData::IsAchieved(EAchievement::SelectFirstPokemon) && MenuCount == 3)
	{
		MenuCount = 4;
		Cursor->SetOptionCount(MenuCount);
		RefreshMenuBox();
		RefreshExplainText();
	}

	// 메뉴창 끄기
	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown('X'))
	{
		Close();
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		Cursor->IncCursor();
		RefreshExplainText();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		Cursor->DecCursor();
		RefreshExplainText();
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		MenuAction();
		return;
	}
}

void AMenuCanvas::Refresh()
{
	RefreshMenuBox();
	RefreshExplainText();
}

void AMenuCanvas::MenuAction()
{
	int MenuIndex = GetMenuIndex();

	switch (MenuIndex)
	{
	case EMenuIndex::Pokedex:
		break;
	case EMenuIndex::Pokemon:
		UEventManager::FadeChangeLevel(Global::MenuPokemonUILevel);
		break;
	case EMenuIndex::Bag:
		UEventManager::FadeChangeLevel(Global::MenuBagUILevel);
		break;
	case EMenuIndex::Player:
		UEventManager::FadeChangeLevel(Global::TrainerCardUILevel);
		break;
	case EMenuIndex::Exit:
		Close();
		break;
	default:
		break;
	}
}

void AMenuCanvas::RefreshMenuBox()
{
	std::string ImageName = "MenuWindow" + std::to_string(MenuCount) + ".png";
	MenuBox->SetImage(ImageName);

	Cursor->SetOptionCount(MenuCount);
	Cursor->SetCursor(Cursor->GetCursor());

	for (int i = 0; i < MenuCount; i++)
	{
		MenuTexts[i]->SetText(MenuNames[(MenuNames.size() - MenuCount) + i]);
	}
	for (int i = MenuCount; i < 5; i++)
	{
		MenuTexts[i]->SetText(L"");
	}
}

void AMenuCanvas::RefreshExplainText()
{
	MenuExplainText->SetActive(true);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
}