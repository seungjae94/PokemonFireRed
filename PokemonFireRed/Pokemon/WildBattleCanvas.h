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

	// ���� ��ġ ���
	FVector EnemyPokemonBoxInitPos;
	FVector EnemyPokemonBoxHidePos;
	FVector PlayerPokemonBoxInitPos;
	FVector PlayerPokemonBoxHidePos;
	FVector EnemyGroundInitPos;
	FVector EnemyGroundHidePos;
	FVector PlayerGroundInitPos;
	FVector PlayerGroundHidePos;

	// ���
	UImageRenderer* Background = nullptr;

	// �ֻ��� ���
	UImageRenderer* MsgBox = nullptr;
	UImageRenderer* ActionBox = nullptr;
	UImageRenderer* EnemyPokemonBox = nullptr;
	UImageRenderer* PlayerPokemonBox = nullptr;
	UImageRenderer* EnemyGround = nullptr;
	UImageRenderer* PlayerGround = nullptr;

	// MsgBox ���
	AText* MsgText = nullptr;

	// EnemyPokemonBox ���
	AText* EnemyPokemonNameText = nullptr;
	AText* EnemyPokemonLevelText = nullptr;
	AScrollBar* EnemyPokemonHpBar = nullptr;
	AImageElement* EnemyPokemonGenderMark = nullptr;

	// PlayerPokemonBox ���
	AText* PlayerPokemonNameText = nullptr;
	AText* PlayerPokemonLevelText = nullptr;
	AText* PlayerPokemonCurHpText = nullptr;
	AText* PlayerPokemonHpText = nullptr;
	AScrollBar* PlayerPokemonHpBar = nullptr;
	AScrollBar* PlayerPokemonExpBar = nullptr;
	AImageElement* PlayerPokemonGenderMark = nullptr;

	// EnemyPokemonGround ���
	APokemonElement* EnemyPokemonImage = nullptr;

	// PlayerPokemonGround ���
	AAnimationElement* PlayerBattler = nullptr;
	APokemonElement* PlayerPokemonImage = nullptr;
};

