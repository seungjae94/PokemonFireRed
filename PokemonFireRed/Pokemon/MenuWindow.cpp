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
	APage::BeginPlay();

	// ���� �޴�â
	MenuWindowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowRenderer->CameraEffectOff();
	RefreshMenuWindow();

	for (int i = 0; i < 5; i++)
	{
		AText* MenuText = CreateText(
			MenuWindowRenderer,
			L"",
			EPivotType::LeftTop,
			EAlignType::Left,
			15, 18 + 15 * i,
			EFontColor::Gray
		);
		MenuTexts.push_back(MenuText);
	}
	RefreshMenuTexts();
	
	// �ϴ� �޴� ����â
	MenuWindowExplainRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowExplainRenderer->CameraEffectOff();
	MenuWindowExplainRenderer->SetImage("MenuWindowExplain.png");

	FVector ExplainRenderScale = UPokemonUtil::GetRenderScale(MenuWindowExplainRenderer);
	FVector ExplainPos = ExplainRenderScale.Half2D() + FVector(0.0f, Global::FloatScreenY - ExplainRenderScale.Y);
	MenuWindowExplainRenderer->SetTransform({ ExplainPos, ExplainRenderScale });

	MenuExplainText = CreateText(
		MenuWindowExplainRenderer,
		L"",
		EPivotType::LeftTop,
		EAlignType::Left,
		2, 16,
		EFontColor::White
	);

	// �޴�â Ŀ��
	Cursor = CreateCursor(
		MenuWindowRenderer,
		0, MenuCount,
		EPivotType::LeftTop,
		8, 9, 15
	);
}

void AMenuWindow::Tick(float _DeltaTime)
{
	APage::Tick(_DeltaTime);

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
		RefreshMenuWindow();
		RefreshMenuTexts();
		RefreshExplainText();
	}
	else if (true == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon) && MenuCount == 3)
	{
		MenuCount = 4;
		Cursor->SetOptionCount(MenuCount);
		RefreshMenuWindow();
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
	RefreshMenuWindow();
	RefreshMenuTexts();
	RefreshExplainText();
}

void AMenuWindow::MenuAction()
{
	int MenuIndex = GetMenuIndex();

	switch (MenuIndex)
	{
	case EMenuIndex::Pokedex:
		break;
	case EMenuIndex::Pokemon:
		UEventManager::ChangeLevelFade(GetWorld(), Global::PokemonUILevel);
		break;
	case EMenuIndex::Bag:
		break;
	case EMenuIndex::Player:
		UEventManager::ChangeLevelFade(GetWorld(), Global::TrainerCardUILevel);
		break;
	case EMenuIndex::Exit:
		Close();
		break;
	default:
		break;
	}
}

void AMenuWindow::RefreshMenuWindow()
{
	// �޴�â
	std::string ImageName = "MenuWindow" + std::to_string(MenuCount) + ".png";
	MenuWindowRenderer->SetImage(ImageName);
	FVector MenuWindowRenderScale = UPokemonUtil::GetRenderScale(MenuWindowRenderer);
	FVector MenuWindowPos = MenuWindowRenderScale.Half2D() + FVector(Global::FloatScreenX - MenuWindowRenderScale.X, 0.0f);
	MenuWindowPos += FVector(-Global::FloatPixelSize, Global::FloatPixelSize);
	MenuWindowRenderer->SetTransform({ MenuWindowPos, MenuWindowRenderScale });
}

void AMenuWindow::RefreshMenuTexts()
{
	// �޴� �ؽ�Ʈ
	for (int i = 0; i < MenuCount; i++)
	{
		MenuTexts[i]->SetActive(true);
		MenuTexts[i]->SetText(MenuNames[(MenuNames.size() - MenuCount) + i]);
	}
	for (int i = MenuCount; i < 5; i++)
	{
		MenuTexts[i]->SetActive(false);
	}
}

void AMenuWindow::RefreshExplainText()
{
	MenuExplainText->SetActive(true);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
}