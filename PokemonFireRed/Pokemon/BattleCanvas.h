#pragma once
#include "Canvas.h"
#include "Pokemon.h"

class ABattleCanvas : public ACanvas
{
	friend class ABattleStartStateMachine;
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

	// BattleStart
	void LerpShowGrounds(float _t);
	void LerpShowEnemyPokemonBox(float _t);
	void PlayBattlerThrowingAnimation();
	void LerpHidePlayerBattler(float _t);
	void PlayThrowedBallAnimation();
	void TakeOutPokemonFromBall(float _t);
	void LerpShowPlayerPokemonBox(float _t);
	void SetActionBoxActive(bool _Value);
	void SetBattleMessage(std::wstring_view _Msg);

	// PlayerAction
	int GetActionCursor() const
	{
		return ActionCursor->GetCursor();
	}

	void SetActionCursor(int _Cursor)
	{
		ActionCursor->SetCursor(_Cursor);
	}



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
	FVector PlayerPokemonImageInitPos;
	FVector PlayerPokemonImageHidePos;
	FVector PlayerBattlerInitPos;
	FVector PlayerBattlerHidePos;

	// 배경
	AImageElement* Background = nullptr;

	// 최상위 요소
	AImageElement* MsgBox = nullptr;
	AImageElement* ActionBox = nullptr;
	AImageElement* EnemyPokemonBox = nullptr;
	AImageElement* PlayerPokemonBox = nullptr;
	AImageElement* EnemyGround = nullptr;
	AImageElement* PlayerGround = nullptr;

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
	AImageElement* EnemyPokemonImage = nullptr;

	// PlayerPokemonGround 요소
	AImageElement* PlayerPokemonImage = nullptr;
	AImageElement* PlayerBattler = nullptr;
	AImageElement* ThrowedBall = nullptr;

	// ActionBox 요소
	ACursor* ActionCursor = nullptr;
};

