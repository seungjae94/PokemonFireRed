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
	AActor::BeginPlay();

	// 우상단 메뉴창
	MenuWindowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowRenderer->CameraEffectOff();
	MenuWindowRenderer->SetImage("MenuWindowFirstClean.png");

	UWindowImage* MenuWindowImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowFirst.png");
	FVector MenuWindowScale = MenuWindowImage->GetScale();
	FVector MenuWindowRenderScale = MenuWindowScale * Global::FloatPixelSize;
	FVector MenuWindowPos = MenuWindowRenderScale.Half2D() + FVector(Global::FloatScreenX - MenuWindowRenderScale.X, 0.0f);
	MenuWindowPos += FVector(-Global::FloatPixelSize, Global::FloatPixelSize);
	MenuWindowRenderer->SetTransform({ MenuWindowPos, MenuWindowRenderScale });

	// 하단 메뉴 설명창
	MenuWindowExplainRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowExplainRenderer->CameraEffectOff();
	MenuWindowExplainRenderer->SetImage("MenuWindowExplain.png");

	UWindowImage* MenuWindowExplainImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowExplain.png");
	FVector MenuWindowExplainScale = MenuWindowExplainImage->GetScale();
	FVector MenuWindowExplainRenderScale = MenuWindowExplainScale * Global::FloatPixelSize;
	FVector MenuWindowExplainPos = MenuWindowExplainRenderScale.Half2D() + FVector(0.0f, Global::FloatScreenY - MenuWindowExplainRenderScale.Y);
	MenuWindowExplainRenderer->SetTransform({ MenuWindowExplainPos, MenuWindowExplainRenderScale });

	// 메뉴 커서 화살표
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

	// 텍스트 설정
	UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());

	MenuExplainText = CurLevel->SpawnUIElement<APokemonText>("MenuExplainText");
	MenuExplainText->SetActorLocation(FVector(2, 137) * Global::FloatPixelSize);
	MenuExplainText->SetColor(EFontColor::White);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);

	for (int i = 0; i < MenuCount; i++)
	{
		APokemonText* MenuText = CurLevel->SpawnUIElement<APokemonText>("MenuText" + std::to_string(i));
		FVector MenuTextPos = MenuWindowRenderer->GetTransform().LeftTop();
		MenuTextPos += FVector(15, 18 + 15 * i) * Global::FloatPixelSize;
		MenuText->SetActorLocation(MenuTextPos);
		MenuText->SetColor(EFontColor::Gray);
		MenuText->SetText(MenuNames[(MenuNames.size() - MenuCount) + i]);
		MenuTexts.push_back(MenuText);
	}

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

	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown('X'))
	{
		ActiveOff();
		AllRenderersActiveOff();
		IsFirstTick = true;

		UEventManager::TriggerEvent(MenuWindowCloseTrigger, EEventTriggerAction::Direct);
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
}

void AMenuWindow::IncCursor()
{
	int NextCursor = (Cursor + 1) % OptionCount;
	MoveCursor(NextCursor);
}

void AMenuWindow::DecCursor()
{
	int NextCursor = (Cursor - 1 + OptionCount) % OptionCount;
	MoveCursor(NextCursor);
}

void AMenuWindow::MoveCursor(int _Cursor)
{
	if (_Cursor < 0 || _Cursor >= OptionCount)
	{
		MsgBoxAssert("메뉴창 커서 위치가 0 미만이거나 OptionCount 이상입니다.");
		return;
	}

	Cursor = _Cursor;
	FVector ArrowPos = GetArrowPos();
	ArrowRenderer->SetPosition(ArrowPos);
	MenuExplainText->SetText(MenuExplains[GetMenuIndex()]);
}

FVector AMenuWindow::GetArrowPos()
{
	FVector ArrowRenderScale = ArrowRenderer->GetTransform().GetScale();
	FVector ArrowPos = UPokemonUtil::GetMatchLeftTop(ArrowRenderScale, MenuWindowRenderer->GetTransform());
	ArrowPos += FVector(8, 9 + Cursor * 15) * Global::FloatPixelSize;
	return ArrowPos;
}