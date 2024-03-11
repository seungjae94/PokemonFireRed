#include "PokemonCenterCanvas.h"

APokemonCenterCanvas::APokemonCenterCanvas() 
{
}

APokemonCenterCanvas::~APokemonCenterCanvas() 
{
}

void APokemonCenterCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	OptionBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::RightBot, -32, -48);
	OptionBox->SetImage(RN::YesNoOptionBox);

	ActionCursor = CreateCursor(OptionBox, ERenderingOrder::UI1, EPivotType::LeftTop, 8, 11);
	ActionCursor->SetOptionCount(2);
}

void APokemonCenterCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

void APokemonCenterCanvas::SetOptionBoxActive(bool _Value)
{
	OptionBox->SetActive(_Value);
}

void APokemonCenterCanvas::IncCursor()
{
	if (ActionCursor->GetCursor() == 0)
	{
		ActionCursor->IncCursor();
	}
}

void APokemonCenterCanvas::DecCursor()
{
	if (ActionCursor->GetCursor() == 1)
	{
		ActionCursor->DecCursor();
	}
}

int APokemonCenterCanvas::GetCursor()
{
	return ActionCursor->GetCursor();
}
