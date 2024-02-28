#pragma once
#include "Canvas.h"
#include "Pokemon.h"

class AWildBattleCanvas : public ACanvas
{
private:
	enum class EBattleState
	{
		BattleStart,
		PlayerAction,
		PlayerMove,
		EnemyMove,
		PlayerSecondaryEffect,
		EnemySecondaryEffect,
		BattleEnd
	};

	enum class EBattleStartState
	{
		FadeWait,
		GroundMove,
		EnemyPokemonBoxMove,
		ZClickWait,
		SendOutFirstPokemon,
		PlayerPokemonBoxMove,
	};
public:
	// constructor destructor
	AWildBattleCanvas();
	~AWildBattleCanvas();

	// delete Function
	AWildBattleCanvas(const AWildBattleCanvas& _Other) = delete;
	AWildBattleCanvas(AWildBattleCanvas&& _Other) noexcept = delete;
	AWildBattleCanvas& operator=(const AWildBattleCanvas& _Other) = delete;
	AWildBattleCanvas& operator=(AWildBattleCanvas&& _Other) noexcept = delete;

	void Init(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon);
protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void PrepareElements(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon);
	
	// FSM
	EBattleState State = EBattleState::BattleStart;
	float Timer = 0.0f;

	void ProcessBattleStart(float _DeltaTime);
	void ProcessBattleStartFadeWait(float _DeltaTime);
	void ProcessBattleStartGroundMove(float _DeltaTime);
	void ProcessBattleStartEnemyPokemonBoxMove(float _DeltaTime);
	void ProcessBattleStartZClickWait(float _DeltaTime);
	void ProcessBattleStartSendOutFirstPokemon(float _DeltaTime);
	void ProcessBattleStartPlayerPokemonBoxMove(float _DeltaTime);
	EBattleStartState BattleStartState = EBattleStartState::GroundMove;
	float FadeWaitTime = 2.0f;
	float GroundMoveTime = 2.0f;
	float EnemyPokemonBoxMoveTime = 0.3f;
	float PlayePokemonBoxMoveTime = 0.3f;

	// 최초 위치 기억
	FVector EnemyPokemonBoxInitPos;
	FVector EnemyPokemonBoxHidePos;
	FVector PlayerPokemonBoxInitPos;
	FVector PlayerPokemonBoxHidePos;
	FVector EnemyGroundInitPos;
	FVector EnemyGroundHidePos;
	FVector PlayerGroundInitPos;
	FVector PlayerGroundHidePos;

	// 배경
	UImageRenderer* Background = nullptr;

	// 최상위 요소
	UImageRenderer* MsgBox = nullptr;
	UImageRenderer* ActionBox = nullptr;
	UImageRenderer* EnemyPokemonBox = nullptr;
	UImageRenderer* PlayerPokemonBox = nullptr;
	UImageRenderer* EnemyGround = nullptr;
	UImageRenderer* PlayerGround = nullptr;

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

