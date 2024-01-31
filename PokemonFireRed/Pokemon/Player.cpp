#include "Player.h"

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	AActor::BeginPlay();

	SetActorLocation({ 360, 240 });

	Renderer = CreateImageRenderer(1);
	Renderer->SetImage("Player.bmp");
	Renderer->SetTransform({ {0, 0}, {48, 96} });
	Renderer->SetImageCuttingTransform({{0, 0}, {16, 32}});
}
