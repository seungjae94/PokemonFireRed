#include "AnimatedSea.h"

AnimatedSea::AnimatedSea() 
{
}

AnimatedSea::~AnimatedSea() 
{
}

void AnimatedSea::BeginPlay()
{
	Renderer = CreateImageRenderer(ERenderingOrder::BelowBackground);
	Renderer->SetImage("AnimatedSea.png");

	UWindowImage* Image = Renderer->GetImage();
	Renderer->SetTransform({ FVector::Zero, {Global::FloatTileSize, Global::FloatTileSize} });
	Renderer->CreateAnimation("AnimatedSea", Image->GetName(), 0, 7, 0.3f, true);
	Renderer->ChangeAnimation("AnimatedSea");
}

