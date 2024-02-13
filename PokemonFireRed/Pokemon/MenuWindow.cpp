#include "MenuWindow.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "PokemonUtil.h"
#include "EventManager.h"
#include "PokemonText.h"

AMenuWindow::AMenuWindow()
{
}

AMenuWindow::~AMenuWindow()
{
}

void AMenuWindow::AllRenderersActiveOn()
{
	MenuWindowRenderer->ActiveOn();
	MenuWindowExplainRenderer->ActiveOn();
	ArrowRenderer->ActiveOn();
	MenuExplainText->SetVisible();
}

void AMenuWindow::AllRenderersActiveOff()
{
	MenuWindowRenderer->ActiveOff();
	MenuWindowExplainRenderer->ActiveOff();
	ArrowRenderer->ActiveOff();
	MenuExplainText->SetInvisible();
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
	FVector MenuWindowRenderScale = MenuWindowScale * Global::F_PIXEL_SIZE;
	FVector MenuWindowPos = MenuWindowRenderScale.Half2D() + FVector(Global::F_SCREEN_X - MenuWindowRenderScale.X, 0.0f);
	MenuWindowPos += FVector(-Global::F_PIXEL_SIZE, Global::F_PIXEL_SIZE);
	MenuWindowRenderer->SetTransform({ MenuWindowPos, MenuWindowRenderScale });

	// 하단 메뉴 설명창
	MenuWindowExplainRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowExplainRenderer->CameraEffectOff();
	MenuWindowExplainRenderer->SetImage("MenuWindowExplain.png");

	UWindowImage* MenuWindowExplainImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowExplain.png");
	FVector MenuWindowExplainScale = MenuWindowExplainImage->GetScale();
	FVector MenuWindowExplainRenderScale = MenuWindowExplainScale * Global::F_PIXEL_SIZE;
	FVector MenuWindowExplainPos = MenuWindowExplainRenderScale.Half2D() + FVector(0.0f, Global::F_SCREEN_Y - MenuWindowExplainRenderScale.Y);
	MenuWindowExplainRenderer->SetTransform({ MenuWindowExplainPos, MenuWindowExplainRenderScale });

	// 메뉴 커서 화살표
	ArrowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	ArrowRenderer->CameraEffectOff();
	ArrowRenderer->SetImage("MenuWindowArrow.png");

	UWindowImage* ArrowImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowArrow.png");
	FVector ArrowScale = ArrowImage->GetScale();
	FVector ArrowRenderScale = ArrowScale * Global::F_PIXEL_SIZE;
	ArrowRenderer->SetScale(ArrowRenderScale);

	FVector ArrowPos = GetArrowPos(Cursor);
	ArrowRenderer->SetPosition(ArrowPos);

	ArrowRenderer->SetImageCuttingTransform({ {0, 0}, ArrowScale });

	// 메뉴 설명
	MenuExplainText = GetWorld()->SpawnActor<APokemonText>();
	MenuExplainText->SetActorLocation(FVector(2, 137) * Global::F_PIXEL_SIZE);
	MenuExplainText->SetText(
		LR"(Equipped with pockets for storing items
you bought, received, or found.)"
	);
}

void AMenuWindow::Tick(float _DeltaTime)
{
	static bool IsFirstTick = true;

	// 이벤트 매니저가 MenuWindow를 ActiveOn한 다음 틱부터 로직을 실행한다.
	// - ActiveOn한 틱에는 UEngineInput::IsDown(VK_RETURN) 값이 true로 들어가 있기 때문이다.
	if (IsFirstTick)
	{
		MenuExplainText->ActiveOn();
		IsFirstTick = false;
		return;
	}

	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown('X'))
	{
		UEventManager::GiveBackPlayerControl();
		ActiveOff();
		AllRenderersActiveOff();
		IsFirstTick = true;
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
	FVector ArrowPos = GetArrowPos(Cursor);
	ArrowRenderer->SetPosition(ArrowPos);
}

FVector AMenuWindow::GetArrowPos(int _Cursor)
{
	FVector ArrowRenderScale = ArrowRenderer->GetTransform().GetScale();
	FVector ArrowPos = UPokemonUtil::GetMatchLeftTop(ArrowRenderScale, MenuWindowRenderer->GetTransform());
	ArrowPos += FVector(8, 9 + _Cursor * 15) * Global::F_PIXEL_SIZE;
	return ArrowPos;
}