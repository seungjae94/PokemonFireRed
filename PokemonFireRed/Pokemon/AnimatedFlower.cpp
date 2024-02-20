#include "AnimatedFlower.h"


AnimatedFlower::AnimatedFlower() 
{
}

AnimatedFlower::~AnimatedFlower() 
{
}

void AnimatedFlower::BeginPlay()
{
	Renderer = CreateImageRenderer(ERenderingOrder::BelowCharacter);
	Renderer->SetImage("AnimatedFlower.png");

	UWindowImage* Image = Renderer->GetImage();
	Renderer->SetTransform({ FVector::Zero, {Global::FloatTileSize, Global::FloatTileSize} });
	Renderer->CreateAnimation("AnimatedFlower", Image->GetName(), 0, 4, 0.2f, true);
	Renderer->ChangeAnimation("AnimatedFlower");
}

