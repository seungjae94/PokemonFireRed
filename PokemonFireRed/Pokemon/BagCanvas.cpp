#include "BagCanvas.h"

ABagCanvas::ABagCanvas()
{
}

ABagCanvas::~ABagCanvas()
{
}

void ABagCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::UBItemsBackground);

	//LeftArrow = CreateImageElement(Background, ERenderingOrder::UI0, EPivotType::LeftTop, 3, 65);
	//LeftArrow->SetImage(RN::BagArrowLeft);
	//LeftArrow->CreateAnimation(Global::BagArrowLeft, 0, 1, 0.1f, true);
	//LeftArrow->ChangeAnimation(Global::BagArrowLeft);
	//
	//RightArrow = CreateImageElement(Background, ERenderingOrder::UI0, EPivotType::LeftTop, 70, 65);
	//RightArrow->SetImage(RN::BagArrowLeft);
	//RightArrow->CreateAnimation(Global::BagArrowLeft, 0, 1, 0.1f, true);
	//RightArrow->ChangeAnimation(Global::BagArrowLeft);

	ItemImage = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 4, -8);
	ItemImage->SetImage("ItemPotion.png");

	ItemExplain = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 40, -33, EAlignType::Left, EFontColor::White);
	ItemExplain->SetText(L"Debug Text\nDebug Text2\nDebuging...");

	const int RowGap = 16;
	for (int i = 0; i < 6; ++i)
	{
		AText* ItemName = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 97, 21 + RowGap * i, EAlignType::Left, EFontColor::Black3);
		AText* ItemCross = CreateText(Background, ERenderingOrder::UI1, EPivotType::RightTop, -35, 20 + RowGap * i, EAlignType::Right, EFontColor::Black3);
		AText* ItemCount = CreateText(Background, ERenderingOrder::UI1, EPivotType::RightTop, -19, 21 + RowGap * i, EAlignType::Right, EFontColor::Black3, EFontSize::Mini);
		
		ItemName->SetText(L"DebugItemName");
		ItemCross->SetText(L"x");
		ItemCount->SetText(L"999");

		ItemNames.push_back(ItemName);
		ItemCrosses.push_back(ItemCross);
		ItemCounts.push_back(ItemCount);
	}
}
