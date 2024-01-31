#include "Player.h"
#include <EnginePlatform/EngineInput.h>
#include "Ground.h"

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

void APlayer::Tick(float _DeltaTime)
{
	static float speed = 3 * 48.0f;

	FVector MoveVec = FVector::Zero;
	if (EngineInput::IsPress(VK_LEFT))
	{
		MoveVec = FVector::Left * speed * _DeltaTime;
	}
	else if (EngineInput::IsPress(VK_RIGHT))
	{
		MoveVec = FVector::Right * speed * _DeltaTime;
	}
	else if (EngineInput::IsPress(VK_UP))
	{
		MoveVec = FVector::Up * speed * _DeltaTime;
	}
	else if (EngineInput::IsPress(VK_DOWN))
	{
		MoveVec = FVector::Down * speed * _DeltaTime;
	}

	//AddActorLocation(MoveVec);
	Ground->AddActorLocation(MoveVec * (-1));
}


