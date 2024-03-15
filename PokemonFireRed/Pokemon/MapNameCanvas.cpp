#include "MapNameCanvas.h"
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"
#include "PokemonMath.h"
#include "PokemonUtil.h"

AMapNameCanvas::AMapNameCanvas()
{
}

AMapNameCanvas::~AMapNameCanvas()
{
}

void AMapNameCanvas::Open(std::wstring_view _MapName)
{
	if (EMapNameCanvasState::Hide != State)
	{
		// 이미 열려 있는 경우 빠르게 닫고 다시 연다.
		MemoryMapName = _MapName;
		State = EMapNameCanvasState::FastClose;

		CurChangeTime = FastChangeTime;
		return;
	}

	MapNameText->SetText(_MapName);
	CurChangeTime = ChangeTime;
	State = EMapNameCanvasState::Open;
}

void AMapNameCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// 배경 이미지 생성
	MapNameBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 2);
	MapNameBox->SetImage(RN::MapNameWindow);

	// 텍스트 설정
	MapNameText = CreateText(MapNameBox, ERenderingOrder::UI1, EPivotType::CenterBot, 0, -8, EAlignType::Center, EFontColor::Gray);

	MapNameBox->SetActive(false);
}

void AMapNameCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	switch (State)
	{
	case EMapNameCanvasState::Hide:
		break;
	case EMapNameCanvasState::Open:
		OpenTick(_DeltaTime);
		break;
	case EMapNameCanvasState::Close:
		CloseTick(_DeltaTime);
		break;
	case EMapNameCanvasState::FastClose:
		FastCloseTick(_DeltaTime);
		break;
	case EMapNameCanvasState::Show:
		ShowTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void AMapNameCanvas::OpenTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;
		
	FVector Pos = UPokemonMath::Lerp(ShowPos, HidePos, CurChangeTime / ChangeTime);
	MapNameBox->SetRelativePosition(Pos);

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameCanvasState::Show;
		CurChangeTime = ShowTime;
	}
}

void AMapNameCanvas::ShowTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameCanvasState::Close;
		CurChangeTime = ChangeTime;
	}
}

void AMapNameCanvas::CloseTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;
	
	FVector Pos = UPokemonMath::Lerp(HidePos, ShowPos, CurChangeTime / ChangeTime);
	MapNameBox->SetRelativePosition(Pos);

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameCanvasState::Hide;
	}
}

void AMapNameCanvas::FastCloseTick(float _DeltaTime)
{
	CurChangeTime -= _DeltaTime;

	FVector Pos = UPokemonMath::Lerp(HidePos, ShowPos, CurChangeTime / FastChangeTime);
	MapNameBox->SetRelativePosition(Pos);

	if (CurChangeTime < 0.0f)
	{
		State = EMapNameCanvasState::Open;
		MapNameText->SetText(MemoryMapName);
		CurChangeTime = ChangeTime;
		State = EMapNameCanvasState::Open;
	}
}

