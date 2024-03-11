#include "EndingCanvas.h"

AEndingCanvas::AEndingCanvas()
{
}

AEndingCanvas::~AEndingCanvas()
{
}

void AEndingCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 40);
	Background->SetImage(RN::EndingBackground);
	Background->SetAlpha(0.7f);

	Message = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 0);
	Message->SetImage(RN::EndingMessage);

	Runner = CreateImageElement(Background, ERenderingOrder::UI2, EPivotType::RightBot, -40, -13);
	Runner->SetImage(RN::EndingRunner);
	Runner->CreateAnimation("Run", 0, 5, 0.1f, true);
	Runner->ChangeAnimation("Run");

	RunnerGround = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::RightBot, -20, -5);
	RunnerGround->SetImage(RN::EndingRunnerGround);
}
