#include "MapNameWindow.h"
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"
#include "PokemonMath.h"
#include "PokemonUtil.h"

AMapNameWindow::AMapNameWindow()
{
}

AMapNameWindow::~AMapNameWindow()
{
}

void AMapNameWindow::Open(std::wstring_view _MapName)
{
	if (EMapNameWindowState::Hide != State)
	{
		// 이미 열려 있는 경우 빠르게 닫고 다시 연다.
		WaitingReopen = true;
		BackupString = _MapName;
		State = EMapNameWindowState::Close;

		float CurProp = GetCurProp();
		CurChangeTime = ChangeTime * CurProp;
		return;
	}

	MapNameText->SetText(_MapName, true);
	CurChangeTime = ChangeTime;
	State = EMapNameWindowState::Open;
}

void AMapNameWindow::BeginPlay()
{
	AUIElement::BeginPlay();

	// 배경 이미지 렌더러 설정
	Renderer = CreateImageRenderer(ERenderingOrder::LowerUI);

	std::string ImageName = "MapNameWindow.png";
	Renderer->SetImage(ImageName);
	Renderer->CameraEffectOff();
	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(ImageName);
	FVector ImageRenderScale = Image->GetScale() * Global::FloatPixelSize;
	ShowPos = ImageRenderScale.Half2D() + UPokemonUtil::PixelVector(2, 0);
	HidePos = ShowPos - FVector(0.0f, ImageRenderScale.Y);
	SetActorLocation(HidePos);
	Renderer->SetTransform({ FVector::Zero, ImageRenderScale });

	// 텍스트 설정
	UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
	MapNameText = CurLevel->SpawnText(Renderer, EPivotType::Center, EAlignType::Center);
	MapNameText->SetColor(EFontColor::Gray);
	MapNameText->FollowContainer();
}

void AMapNameWindow::Tick(float _DeltaTime)
{
	switch (State)
	{
	case EMapNameWindowState::Hide:
		break;
	case EMapNameWindowState::Open:
		OpenTick(_DeltaTime);
		break;
	case EMapNameWindowState::Close:
		CloseTick(_DeltaTime);
		break;
	case EMapNameWindowState::Show:
		ShowTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void AMapNameWindow::OpenTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;
		
	FVector Pos = UPokemonMath::Lerp(ShowPos, HidePos, CurChangeTime / ChangeTime);
	SetActorLocation(Pos);
	MapNameText->FollowContainer();

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameWindowState::Show;
		CurChangeTime = ShowTime;
	}
}

void AMapNameWindow::ShowTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameWindowState::Close;
		CurChangeTime = ChangeTime;
	}
}

void AMapNameWindow::CloseTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;
	
	FVector Pos = UPokemonMath::Lerp(HidePos, ShowPos, CurChangeTime / ChangeTime);
	SetActorLocation(Pos);
	MapNameText->FollowContainer();

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameWindowState::Hide;

		// Close 종료시 reopen 대기 중일 경우 열어준다.
		if (true == WaitingReopen)
		{
			WaitingReopen = false;
			Open(BackupString);
			BackupString = L"";
		}
	}
}

