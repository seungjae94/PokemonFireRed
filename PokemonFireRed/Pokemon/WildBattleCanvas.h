#pragma once
#include "Canvas.h"
#include "Pokemon.h"

class AWildBattleCanvas : public ACanvas
{
public:
	// constructor destructor
	AWildBattleCanvas();
	~AWildBattleCanvas();

	// delete Function
	AWildBattleCanvas(const AWildBattleCanvas& _Other) = delete;
	AWildBattleCanvas(AWildBattleCanvas&& _Other) noexcept = delete;
	AWildBattleCanvas& operator=(const AWildBattleCanvas& _Other) = delete;
	AWildBattleCanvas& operator=(AWildBattleCanvas&& _Other) noexcept = delete;

	void BattleStart(const UPokemon& _Pokemon);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Background = nullptr;

	// 최상위 요소
	UImageRenderer* MsgBox = nullptr;
	UImageRenderer* ActionBox = nullptr;
	UImageRenderer* EnemyPokemonBox = nullptr;
	UImageRenderer* PlayerPokemonBox = nullptr;
};

