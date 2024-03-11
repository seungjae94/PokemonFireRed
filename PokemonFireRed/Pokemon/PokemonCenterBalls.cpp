#include "PokemonCenterBalls.h"
#include "Global.h"
#include "PokemonUtil.h"

APokemonCenterBalls::APokemonCenterBalls() 
{
}

APokemonCenterBalls::~APokemonCenterBalls() 
{
}

void APokemonCenterBalls::SetActiveBall(int _Index, bool _Value)
{
	Balls[_Index]->SetActive(_Value);
}

void APokemonCenterBalls::SetActiveAllBalls(bool _Value)
{
	for (UImageRenderer* Ball : Balls)
	{
		Ball->SetActive(_Value);
	}
}

void APokemonCenterBalls::BeginPlay()
{
	for (int i = 0; i < 6; ++i)
	{
		UImageRenderer* Ball = CreateImageRenderer(ERenderingOrder::Foreground);
		Ball->SetImage(RN::BallHeal);

		int Row = i / 2;
		int Col = i % 2;

		Ball->SetTransform({ UPokemonUtil::PixelVector(6 * Col, 4 * Row), {Global::FloatTileSize, Global::FloatTileSize} });
		Ball->CreateAnimation("BallIdle", RN::BallHeal, 0, 0, 0.0f, true);
		Ball->CreateAnimation("BallBlink", RN::BallHeal, 0, 4, 0.1f, true);
		Ball->SetActive(false);
		Balls.push_back(Ball);
	}
}

