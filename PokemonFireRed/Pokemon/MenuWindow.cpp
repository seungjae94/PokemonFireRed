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

	// ���� �޴�â
	MenuWindowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowRenderer->CameraEffectOff();
	MenuWindowRenderer->SetImage("MenuWindowFirstClean.png");

	UWindowImage* MenuWindowImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowFirst.png");
	FVector MenuWindowScale = MenuWindowImage->GetScale();
	FVector MenuWindowRenderScale = MenuWindowScale * Global::F_PIXEL_SIZE;
	FVector MenuWindowPos = MenuWindowRenderScale.Half2D() + FVector(Global::F_SCREEN_X - MenuWindowRenderScale.X, 0.0f);
	MenuWindowPos += FVector(-Global::F_PIXEL_SIZE, Global::F_PIXEL_SIZE);
	MenuWindowRenderer->SetTransform({ MenuWindowPos, MenuWindowRenderScale });

	// �ϴ� �޴� ����â
	MenuWindowExplainRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	MenuWindowExplainRenderer->CameraEffectOff();
	MenuWindowExplainRenderer->SetImage("MenuWindowExplain.png");

	UWindowImage* MenuWindowExplainImage = UEngineResourcesManager::GetInst().FindImg("MenuWindowExplain.png");
	FVector MenuWindowExplainScale = MenuWindowExplainImage->GetScale();
	FVector MenuWindowExplainRenderScale = MenuWindowExplainScale * Global::F_PIXEL_SIZE;
	FVector MenuWindowExplainPos = MenuWindowExplainRenderScale.Half2D() + FVector(0.0f, Global::F_SCREEN_Y - MenuWindowExplainRenderScale.Y);
	MenuWindowExplainRenderer->SetTransform({ MenuWindowExplainPos, MenuWindowExplainRenderScale });

	// �޴� Ŀ�� ȭ��ǥ
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

	// �޴� ����
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

	// �̺�Ʈ �Ŵ����� MenuWindow�� ActiveOn�� ���� ƽ���� ������ �����Ѵ�.
	// - ActiveOn�� ƽ���� UEngineInput::IsDown(VK_RETURN) ���� true�� �� �ֱ� �����̴�.
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
		MsgBoxAssert("�޴�â Ŀ�� ��ġ�� 0 �̸��̰ų� OptionCount �̻��Դϴ�.");
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