#include "Surprise.h"
#include "Global.h"

ASurprise::ASurprise()
{
}

ASurprise::~ASurprise()
{
}

void ASurprise::Play()
{
	SetActive(true);
	Renderer->ChangeAnimation("Surprise");
}

bool ASurprise::IsEnd() const
{
	return Renderer->IsCurAnimationEnd();
}

void ASurprise::BeginPlay()
{
	AActor::BeginPlay();

	const int WaitCount = 15;
	std::vector<int> Indexes;
	for (int i = 0; i <= 7; ++i)
	{
		Indexes.push_back(i);
	}
	for (int i = 0; i < WaitCount; ++i)
	{
		Indexes.push_back(7);
	}

	Renderer = CreateImageRenderer(ERenderingOrder::UI0);
	Renderer->SetImage(RN::Surprise);
	Renderer->CreateAnimation("Surprise", RN::Surprise, Indexes, 1.0f / 30, false);
	Renderer->SetTransform({ {0, 0}, {Global::FloatTileSize, 2 * Global::FloatTileSize } });
}

