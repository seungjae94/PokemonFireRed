#include "DebugCanvas.h"

ADebugCanvas::ADebugCanvas()
{
}

ADebugCanvas::~ADebugCanvas()
{
}

void ADebugCanvas::Refresh()
{
	DmgInfo->SetText(L"DMG: x" + std::to_wstring(Global::DamageBonusCoeff));
	ExpInfo->SetText(L"EXP: x" + std::to_wstring(Global::ExpBonusCoeff));
	CatchRateInfo->SetText(L"Catch Rate: x" + std::to_wstring(Global::CatchRateBonusCoeff));
	EncounterRateInfo->SetText(L"Encounter Rate: x" + std::to_wstring(Global::WildBattleFrequency));
}

void ADebugCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	DmgInfo = CreateText(this, ERenderingOrder::Super, EPivotType::LeftTop, 2, 10, EAlignType::Left, EFontColor::Black2);
	ExpInfo = CreateText(this, ERenderingOrder::Super, EPivotType::LeftTop, 2, 24, EAlignType::Left, EFontColor::Black2);
	EncounterRateInfo = CreateText(this, ERenderingOrder::Super, EPivotType::LeftTop, 2, 38, EAlignType::Left, EFontColor::Black2);
	CatchRateInfo = CreateText(this, ERenderingOrder::Super, EPivotType::LeftTop, 2, 52, EAlignType::Left, EFontColor::Black2);

	Refresh();
	SetActive(false);
}
