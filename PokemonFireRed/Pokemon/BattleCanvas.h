#pragma once
#include "Canvas.h"
#include "Pokemon.h"

class ABattleCanvas : public ACanvas
{
	friend class ABattleStartStateManchine;
public:
	// constructor destructor
	ABattleCanvas();
	~ABattleCanvas();

	// delete Function
	ABattleCanvas(const ABattleCanvas& _Other) = delete;
	ABattleCanvas(ABattleCanvas&& _Other) noexcept = delete;
	ABattleCanvas& operator=(const ABattleCanvas& _Other) = delete;
	ABattleCanvas& operator=(ABattleCanvas&& _Other) noexcept = delete;

	void Init(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon);

	void LerpShowGrounds(float _t);
	void LerpShowEnemyPokemonBox(float _t);
	void PlayBattlerThrowingAnimation();
	void LerpHidePlayerBattler(float _t);
	void PlayThrowedBallAnimation();
	void TakeOutPokemonFromBall(float _t);
	void LerpShowPlayerPokemonBox(float _t);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void PrepareElements(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon);

	// 최초 위치 기억
	FVector EnemyPokemonBoxInitPos;
	FVector EnemyPokemonBoxHidePos;
	FVector PlayerPokemonBoxInitPos;
	FVector PlayerPokemonBoxHidePos;
	FVector EnemyGroundInitPos;
	FVector EnemyGroundHidePos;
	FVector PlayerGroundInitPos;
	FVector PlayerGroundHidePos;
	FVector PlayerPokemonImageInitRelativePos;
	FVector PlayerPokemonImageHideRelativePos;
	FVector PlayerBattlerInitRelativePos;
	FVector PlayerBattlerHideRelativePos;

	// 배경
	UImageRenderer* Background = nullptr;

	// 최상위 요소
	UImageRenderer* MsgBox = nullptr;
	UImageRenderer* ActionBox = nullptr;
	UImageRenderer* EnemyPokemonBox = nullptr;
	UImageRenderer* PlayerPokemonBox = nullptr;
	UImageRenderer* EnemyGround = nullptr;
	UImageRenderer* PlayerGround = nullptr;
	UImageRenderer* ThrowedBall = nullptr;

	// MsgBox 요소
	AText* MsgText = nullptr;

	// EnemyPokemonBox 요소
	AText* EnemyPokemonNameText = nullptr;
	AText* EnemyPokemonLevelText = nullptr;
	AScrollBar* EnemyPokemonHpBar = nullptr;
	AImageElement* EnemyPokemonGenderMark = nullptr;

	// PlayerPokemonBox 요소
	AText* PlayerPokemonNameText = nullptr;
	AText* PlayerPokemonLevelText = nullptr;
	AText* PlayerPokemonCurHpText = nullptr;
	AText* PlayerPokemonHpText = nullptr;
	AScrollBar* PlayerPokemonHpBar = nullptr;
	AScrollBar* PlayerPokemonExpBar = nullptr;
	AImageElement* PlayerPokemonGenderMark = nullptr;

	// EnemyPokemonGround 요소
	APokemonElement* EnemyPokemonImage = nullptr;

	// PlayerPokemonGround 요소
	AAnimationElement* PlayerBattler = nullptr;
	APokemonElement* PlayerPokemonImage = nullptr;
};

