#pragma once
#include <EngineCore/Actor.h>

class APokemonCenterBalls : public AActor
{
public:
	// constructor destructor
	APokemonCenterBalls();
	~APokemonCenterBalls();

	// delete Function
	APokemonCenterBalls(const APokemonCenterBalls& _Other) = delete;
	APokemonCenterBalls(APokemonCenterBalls&& _Other) noexcept = delete;
	APokemonCenterBalls& operator=(const APokemonCenterBalls& _Other) = delete;
	APokemonCenterBalls& operator=(APokemonCenterBalls&& _Other) noexcept = delete;

	void SetActiveBall(int _Index, bool _Value);
	void SetActiveAllBalls(bool _Value);
	void PlayHealAnimation();
	void StopHealAnimation();

protected:

private:
	void BeginPlay();
	std::vector<UImageRenderer*> Balls;
};

