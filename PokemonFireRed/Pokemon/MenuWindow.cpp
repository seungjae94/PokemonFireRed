#include "MenuWindow.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "PokemonUtil.h"
#include "EventManager.h"
#include "PokemonText.h"
#include "MapLevel.h"
#include "EventTrigger.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "PokemonText.h"
#include "PlayerData.h"

int AMenuWindow::MenuCount = 3;
int AMenuWindow::Cursor = 0;

AMenuWindow::AMenuWindow()
{
}

AMenuWindow::~AMenuWindow()
{
}

void AMenuWindow::Open()
{
	Refresh();
	SetActive(true);
}

void AMenuWindow::SetActive(bool _Active, float _ActiveTime)
{
	AUIElement::SetActive(_Active, _ActiveTime);

	MenuExplainText->SetActive(_Active, _ActiveTime);

	for (APokemonText* MenuText : MenuTexts)
	{
		MenuText->SetActive(_Active, _ActiveTime);
	}
}

void AMenuWindow::BeginPlay()
{
	AUIElement::BeginPlay();

	UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());

	// ���� �޴�â
	MenuWindowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowRenderer->CameraEffectOff();
	DrawMenuWindow();

	for (int i = 0; i < 5; i++)
	{
		APokemonText* MenuText = CurLevel->SpawnText(MenuWindowRenderer);
		MenuText->SetRelativePos(UPokemonUtil::PixelVector(15, 18 + 15 * i));
		MenuText->SetColor(EFontColor::Gray);
		MenuTexts.push_back(MenuText);
	}
	DrawMenuTexts();
	
	// �ϴ� �޴� ����â
	MenuWindowExplainRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowExplainRenderer->CameraEffectOff();
	MenuWindowExplainRenderer->SetImage("MenuWindowExplain.png");

	FVector ExplainRenderScale = UPokemonUtil::GetRenderScale(MenuWindowExplainRenderer);
	FVector ExplainPos = ExplainRenderScale.Half2D() + FVector(0.0f, Global::FloatScreenY - ExplainRenderScale.Y);
	MenuWindowExplainRenderer->SetTransform({ ExplainPos, ExplainRenderScale });

	MenuExplainText = CurLevel->SpawnText(MenuWindowExplainRenderer);
	MenuExplainText->SetRelativePos(UPokemonUtil::PixelVector(2, 16));
	MenuExplainText->SetColor(EFontColor::White);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
	MenuExplainText->FollowContainer();

	// �޴�â Ŀ��
	ArrowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	ArrowRenderer->CameraEffectOff();
	ArrowRenderer->SetImage("MenuWindowArrow.png");

	FVector ArrowRenderScale = UPokemonUtil::GetRenderScale(ArrowRenderer);
	ArrowRenderer->SetScale(ArrowRenderScale);

	FVector ArrowPos = GetArrowPos();
	ArrowRenderer->SetPosition(ArrowPos);
}

void AMenuWindow::Tick(float _DeltaTime)
{
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
		DrawMenuWindow();
		DrawMenuTexts();
		DrawExplainText();
	}
	else if (true == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon) && MenuCount == 3)
	{
		MenuCount = 4;
		DrawMenuWindow();
		DrawMenuTexts();
		DrawExplainText();
	}

	// �޴�â ����
	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown('X'))
	{
		ExitMenu();
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		IncCursor();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		DecCursor();
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
	DrawMenuWindow();
	DrawMenuTexts();
	DrawExplainText();
	DrawArrow();
}

void AMenuWindow::IncCursor()
{
	int NextCursor = (Cursor + 1) % MenuCount;
	MoveCursor(NextCursor);
}

void AMenuWindow::DecCursor()
{
	int NextCursor = (Cursor - 1 + MenuCount) % MenuCount;
	MoveCursor(NextCursor);
}

void AMenuWindow::MoveCursor(int _Cursor)
{
	if (_Cursor < 0 || _Cursor >= MenuCount)
	{
		MsgBoxAssert("�޴�â Ŀ�� ��ġ�� 0 �̸��̰ų� OptionCount �̻��Դϴ�.");
		return;
	}

	Cursor = _Cursor;
	DrawArrow();
	DrawExplainText();
}

void AMenuWindow::MenuAction()
{
	int MenuIndex = GetMenuIndex();

	switch (MenuIndex)
	{
	case MenuIndex::Pokedex:
		break;
	case MenuIndex::Pokemon:
	{
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(Global::PokemonUILevel);
		break;
	}
	case MenuIndex::Bag:
		break;
	case MenuIndex::Player:
		break;
	case MenuIndex::Exit:
		ExitMenu();
		break;
	default:
		break;
	}
}

void AMenuWindow::ExitMenu()
{
	SetActive(false);
	IsFirstTick = true;
}

void AMenuWindow::DrawMenuWindow()
{
	// �޴�â
	std::string ImageName = "MenuWindow" + std::to_string(MenuCount) + ".png";
	MenuWindowRenderer->SetImage(ImageName);
	FVector MenuWindowRenderScale = UPokemonUtil::GetRenderScale(MenuWindowRenderer);
	FVector MenuWindowPos = MenuWindowRenderScale.Half2D() + FVector(Global::FloatScreenX - MenuWindowRenderScale.X, 0.0f);
	MenuWindowPos += FVector(-Global::FloatPixelSize, Global::FloatPixelSize);
	MenuWindowRenderer->SetTransform({ MenuWindowPos, MenuWindowRenderScale });
}

void AMenuWindow::DrawMenuTexts()
{
	// �޴� �ؽ�Ʈ
	for (int i = 0; i < MenuCount; i++)
	{
		MenuTexts[i]->SetActive(true);
		MenuTexts[i]->SetText(MenuNames[(MenuNames.size() - MenuCount) + i]);
		MenuTexts[i]->FollowContainer();
	}
	for (int i = MenuCount; i < 5; i++)
	{
		MenuTexts[i]->SetActive(false);
	}
}

void AMenuWindow::DrawArrow()
{
	ArrowRenderer->SetPosition(GetArrowPos());
}

void AMenuWindow::DrawExplainText()
{
	MenuExplainText->SetActive(true);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
}

FVector AMenuWindow::GetArrowPos()
{
	FVector ArrowRenderScale = ArrowRenderer->GetTransform().GetScale();
	FVector ArrowPos = UPokemonUtil::GetMatchLeftTop(ArrowRenderScale, MenuWindowRenderer->GetTransform());
	ArrowPos += UPokemonUtil::PixelVector(8, 9 + Cursor * 15);
	return ArrowPos;
}