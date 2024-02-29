#include "MenuWindow.h"
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

int AMenuWindow::MenuCount = 3;

AMenuWindow::AMenuWindow()
{
}

AMenuWindow::~AMenuWindow()
{
}

void AMenuWindow::Open()
{
	UEventManager::SetCurLevelPlayerState(EPlayerState::OutOfControl);
	Refresh();
	SetActive(true);
	IsFirstTick = true;
}


void AMenuWindow::Close()
{
	UEventManager::SetCurLevelPlayerState(EPlayerState::Idle);
	SetActive(false);
}

void AMenuWindow::BeginPlay()
{
	ACanvas::BeginPlay();

	// 메뉴창
	MenuBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::RightTop, 0, 0);
	RefreshMenuBoxImage();

	for (int i = 0; i < 5; i++)
	{
		AText* MenuText = CreateText(MenuBox, ERenderingOrder::UI1, EPivotType::LeftTop, 15, 18 + 15 * i, EAlignType::Left, EFontColor::Gray);
		MenuTexts.push_back(MenuText);
	}
	RefreshMenuTexts();

	// 메뉴창 커서
	Cursor = CreateCursor(MenuBox, ERenderingOrder::UI1, EPivotType::LeftTop, 8, 9, RN::BlackCursor, 15);

	// 하단 메뉴 설명창
	MenuExplainBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftBot, 0, 0);
	MenuExplainBox->SetImage(RN::MenuWindowExplain);
	MenuExplainText = CreateText(MenuExplainBox, ERenderingOrder::UI1, EPivotType::LeftTop, 2, 16, EAlignType::Left, EFontColor::White);
}

void AMenuWindow::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	// 이벤트 매니저가 MenuWindow를 ActiveOn한 다음 틱부터 로직을 실행한다.
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
		RefreshMenuBoxImage();
		RefreshMenuTexts();
		RefreshExplainText();
	}
	else if (true == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon) && MenuCount == 3)
	{
		MenuCount = 4;
		Cursor->SetOptionCount(MenuCount);
		RefreshMenuBoxImage();
		RefreshMenuTexts();
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

void AMenuWindow::Refresh()
{
	RefreshMenuBoxImage();
	RefreshMenuTexts();
	RefreshExplainText();

	Cursor->SetOptionCount(MenuCount);
}

void AMenuWindow::MenuAction()
{
	int MenuIndex = GetMenuIndex();

	switch (MenuIndex)
	{
	case EMenuIndex::Pokedex:
		break;
	case EMenuIndex::Pokemon:
		UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		break;
	case EMenuIndex::Bag:
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

void AMenuWindow::RefreshMenuBoxImage()
{
	// 메뉴창
	std::string ImageName = "MenuWindow" + std::to_string(MenuCount) + ".png";
	MenuBox->SetImage(ImageName);
}

void AMenuWindow::RefreshMenuTexts()
{
	// 메뉴 텍스트
	for (int i = 0; i < MenuCount; i++)
	{
		MenuTexts[i]->SetText(MenuNames[(MenuNames.size() - MenuCount) + i]);
	}
	for (int i = MenuCount; i < 5; i++)
	{
		MenuTexts[i]->SetText(L"");
	}
}

void AMenuWindow::RefreshExplainText()
{
	MenuExplainText->SetActive(true);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
}