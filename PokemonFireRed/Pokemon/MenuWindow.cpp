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

	// �޴�â
	MenuBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::RightTop, 0, 0);
	RefreshMenuBoxImage();

	for (int i = 0; i < 5; i++)
	{
		AText* MenuText = CreateText(MenuBox, ERenderingOrder::UI1, EPivotType::LeftTop, 15, 18 + 15 * i, EAlignType::Left, EFontColor::Gray);
		MenuTexts.push_back(MenuText);
	}
	RefreshMenuTexts();

	// �޴�â Ŀ��
	Cursor = CreateCursor(MenuBox, ERenderingOrder::UI1, EPivotType::LeftTop, 8, 9, RN::BlackCursor, 15);

	// �ϴ� �޴� ����â
	MenuExplainBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftBot, 0, 0);
	MenuExplainBox->SetImage(RN::MenuWindowExplain);
	MenuExplainText = CreateText(MenuExplainBox, ERenderingOrder::UI1, EPivotType::LeftTop, 2, 16, EAlignType::Left, EFontColor::White);
}

void AMenuWindow::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	// �̺�Ʈ �Ŵ����� MenuWindow�� ActiveOn�� ���� ƽ���� ������ �����Ѵ�.
	// - ActiveOn�� ƽ���� UEngineInput::IsDown(VK_RETURN) ���� true�� �� �ֱ� �����̴�.
	if (IsFirstTick)
	{
		IsFirstTick = false;
		return;
	}

	// ������ ����
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

	// �޴�â ����
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
	// �޴�â
	std::string ImageName = "MenuWindow" + std::to_string(MenuCount) + ".png";
	MenuBox->SetImage(ImageName);
}

void AMenuWindow::RefreshMenuTexts()
{
	// �޴� �ؽ�Ʈ
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