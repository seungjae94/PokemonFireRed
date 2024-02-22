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

AMenuWindow::AMenuWindow()
{
}

AMenuWindow::~AMenuWindow()
{
}

void AMenuWindow::Open()
{
	ActiveOn();
	AllRenderersActiveOn();
	UEventManager::TriggerEvent(MenuWindowOpenTrigger, EEventTriggerAction::Direct);
}

void AMenuWindow::AllRenderersActiveOn()
{
	MenuWindowRenderer->ActiveOn();
	MenuWindowExplainRenderer->ActiveOn();
	ArrowRenderer->ActiveOn();
	MenuExplainText->SetVisible();
	for (APokemonText* MenuText : MenuTexts)
	{
		MenuText->SetVisible();
	}
}

void AMenuWindow::AllRenderersActiveOff()
{
	MenuWindowRenderer->ActiveOff();
	MenuWindowExplainRenderer->ActiveOff();
	ArrowRenderer->ActiveOff();
	MenuExplainText->SetInvisible();
	for (APokemonText* MenuText : MenuTexts)
	{
		MenuText->SetInvisible();
	}
}

void AMenuWindow::BeginPlay()
{
	AUIElement::BeginPlay();

	UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());

	// 우상단 메뉴창
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
	
	// 하단 메뉴 설명창
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

	// 메뉴창 커서
	ArrowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	ArrowRenderer->CameraEffectOff();
	ArrowRenderer->SetImage("MenuWindowArrow.png");

	UWindowImage* ArrowImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowArrow.png");
	FVector ArrowScale = ArrowImage->GetScale();
	FVector ArrowRenderScale = ArrowScale * Global::FloatPixelSize;
	ArrowRenderer->SetScale(ArrowRenderScale);

	FVector ArrowPos = GetArrowPos();
	ArrowRenderer->SetPosition(ArrowPos);

	ArrowRenderer->SetImageCuttingTransform({ {0, 0}, ArrowScale });

	// 트리거 설정
	UEventTargetInit OpenMenuSetting;
	OpenMenuSetting.SetName("MainWindowOpenTrigger");
	OpenMenuSetting.SetPoint({ 1000, 1000 });

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Direct);
	MenuWindowOpenTrigger = CurLevel->SpawnEventTrigger<AEventTrigger>(OpenMenuSetting);
	UEventManager::RegisterEvent(MenuWindowOpenTrigger, Cond,
		ES::Start(true) >> ES::End(false)
	);

	UEventTargetInit CloseMenuSetting;
	CloseMenuSetting.SetName("MainWindowCloseTrigger");
	CloseMenuSetting.SetPoint({ 2000, 2000 });

	MenuWindowCloseTrigger = CurLevel->SpawnEventTrigger<AEventTrigger>(CloseMenuSetting);
	UEventManager::RegisterEvent(MenuWindowCloseTrigger, Cond,
		ES::Start(false) >> ES::End(true)
	);

	AllRenderersActiveOff();
}

void AMenuWindow::Tick(float _DeltaTime)
{
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

	// 메뉴창 끄기
	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown('X'))
	{
		ExitMenu();
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		IncCursor();
		MenuExplainText->SetVisible();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		DecCursor();
		MenuExplainText->SetVisible();
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		MenuAction();
		return;
	}
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
		MsgBoxAssert("메뉴창 커서 위치가 0 미만이거나 OptionCount 이상입니다.");
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
	ActiveOff();
	AllRenderersActiveOff();
	IsFirstTick = true;
	UEventManager::TriggerEvent(MenuWindowCloseTrigger, EEventTriggerAction::Direct);
}

void AMenuWindow::DrawMenuWindow()
{
	// 메뉴창
	std::string ImageName = "MenuWindow" + std::to_string(MenuCount) + ".png";
	MenuWindowRenderer->SetImage(ImageName);
	FVector MenuWindowRenderScale = UPokemonUtil::GetRenderScale(MenuWindowRenderer);
	FVector MenuWindowPos = MenuWindowRenderScale.Half2D() + FVector(Global::FloatScreenX - MenuWindowRenderScale.X, 0.0f);
	MenuWindowPos += FVector(-Global::FloatPixelSize, Global::FloatPixelSize);
	MenuWindowRenderer->SetTransform({ MenuWindowPos, MenuWindowRenderScale });
}

void AMenuWindow::DrawMenuTexts()
{
	// 메뉴 텍스트
	for (int i = 0; i < MenuCount; i++)
	{
		MenuTexts[i]->SetText(MenuNames[(MenuNames.size() - MenuCount) + i]);
		MenuTexts[i]->SetVisible();
		MenuTexts[i]->FollowContainer();
	}
	for (int i = MenuCount; i < 5; i++)
	{
		MenuTexts[i]->SetText(L"");
		MenuTexts[i]->SetInvisible();
		MenuTexts[i]->FollowContainer();
	}
}

void AMenuWindow::DrawArrow()
{
	ArrowRenderer->SetPosition(GetArrowPos());
}

void AMenuWindow::DrawExplainText()
{
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
	MenuExplainText->SetVisible();
}

FVector AMenuWindow::GetArrowPos()
{
	FVector ArrowRenderScale = ArrowRenderer->GetTransform().GetScale();
	FVector ArrowPos = UPokemonUtil::GetMatchLeftTop(ArrowRenderScale, MenuWindowRenderer->GetTransform());
	ArrowPos += UPokemonUtil::PixelVector(8, 9 + Cursor * 15);
	return ArrowPos;
}