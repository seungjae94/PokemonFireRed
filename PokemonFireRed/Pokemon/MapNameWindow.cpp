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

	MapNameText->SetText(_MapName);
	CurChangeTime = ChangeTime;
	State = EMapNameWindowState::Open;
}

void AMapNameWindow::BeginPlay()
{
	ACanvas::BeginPlay();

	// 배경 이미지 생성
	MapNameBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 2);
	MapNameBox->SetImage(RN::MapNameWindow);

	// 텍스트 설정
	MapNameText = CreateText(MapNameBox, ERenderingOrder::UI1, EPivotType::CenterBot, 0, -8, EAlignType::Center, EFontColor::Gray);

	MapNameBox->SetActive(false);
}

void AMapNameWindow::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

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
	MapNameBox->SetRelativePosition(Pos);

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
	MapNameBox->SetRelativePosition(Pos);

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

