#include "AnimatedSea.h"

bool AnimatedSea::AnimationCreated = false;

AnimatedSea::AnimatedSea() 
{
}

AnimatedSea::~AnimatedSea() 
{
}

void AnimatedSea::Init(int _Index)
{
	UWindowImage* Image = Renderer->GetImage();
	if (false == AnimationCreated)
	{
		for (int i = 0; i < 8; i++)
		{
			std::vector<int> Indexs;
			int Index = i;

			for (int k = 0; k < 8; k++)
			{
				Indexs.push_back(Index);
				Index = (Index + 1) % 8;
			}

			Renderer->CreateAnimation("AnimatedSea" + std::to_string(i), Image->GetName(), Indexs, 0.2f, true);
		}

	}
	Renderer->SetTransform({ FVector::Zero, {Global::FloatTileSize, Global::FloatTileSize} });
	Renderer->ChangeAnimation("AnimatedSea" + std::to_string(_Index));
}

void AnimatedSea::BeginPlay()
{
	Renderer = CreateImageRenderer(ERenderingOrder::BelowBackground);
	Renderer->SetImage("AnimatedSea.png");
}

