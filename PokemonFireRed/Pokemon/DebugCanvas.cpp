#include "DebugCanvas.h"
#include <sstream>

ADebugCanvas::ADebugCanvas()
{
}

ADebugCanvas::~ADebugCanvas()
{
}

void ADebugCanvas::Refresh()
{
	std::wostringstream DmgStream;
	DmgStream << std::setprecision(2) << Global::DamageBonusCoeff;
	std::wstring DmgText = DmgStream.str();
	DmgInfo->SetText(L"DMG: x" + DmgText);

	std::wostringstream ExpStream;
	ExpStream << std::setprecision(2) << Global::ExpBonusCoeff;
	std::wstring ExpText = ExpStream.str();
	ExpInfo->SetText(L"EXP: x" + ExpText);

	std::wostringstream CRStream;
	CRStream << std::setprecision(2) << Global::CatchRateBonusCoeff;
	std::wstring CRText = CRStream.str();
	CatchRateInfo->SetText(L"Catch Rate: x" + CRText);

	std::wostringstream ERStream;
	ERStream << std::setprecision(2) << Global::EncounterRateBonusCoeff;
	std::wstring ERText = ERStream.str();
	EncounterRateInfo->SetText(L"Encounter Rate: x" + ERText);
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
