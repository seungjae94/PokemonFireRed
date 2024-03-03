#pragma once
#include "Canvas.h"
#include "Pokemon.h"
#include "Battler.h"

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

	void Init(const UBattler* _Player, const UBattler* _Enemy);

	// BattleStart
	void LerpShowGrounds(float _t);
	void LerpShowEnemyPokemonBox(float _t);
	void PlayBattlerThrowingAnimation();
	void LerpHidePlayerBattler(float _t);
	void PlayThrowedBallAnimation();
	bool IsThrowedBallAnimationEnd();
	void SetThrowedBallActive(bool _Value);
	void TakeOutPokemonFromBall(float _t);
	void LerpShowPlayerPokemonBox(float _t);
	void SetActionBoxActive(bool _Value);
	void SetMoveSelectBoxActive(bool _Value);
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

	int GetMoveSelectCursor() const
	{
		return MoveSelectCursor->GetCursor();
	}

	void SetMoveSelectCursor(int _Cursor)
	{
		MoveSelectCursor->SetCursor(_Cursor);
	}

	/*
	* 포켓몬 순서 바꿀때 반드시 MoveSelectCursor 초기화 작업 필요!
	*/

	// Turn
	void LerpPlayerHpInfo(int _BeforeHp, int _AfterHp, int _MaxHp, float _t);
	void LerpEnemyHpInfo(int _BeforeHp, int _AfterHp, int _MaxHp, float _t);
	void RefreshPlayerPokemonCurHpText();

protected:

private:
	const UBattler* Player = nullptr;
	const UBattler* Enemy = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void RefreshEnemyPokemonBox();
	void RefreshPlayerPokemonBox();
	void RefreshMoveSelectBox();

	// 렌더링 데이터
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
	int HpBarMaxValue = 1000;

	// 최상위 요소
	AImageElement* Background = nullptr;
	AImageElement* MsgBox = nullptr;
	AImageElement* ActionBox = nullptr;
	AImageElement* MoveSelectBox = nullptr;
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
	AImageElement* ThrowedBall = nullptr;		// 볼 날아가는 애니메이션

	// ActionBox 요소
	ACursor* ActionCursor = nullptr;

	// MoveSelectBox 요소
	ACursor* MoveSelectCursor = nullptr;
	std::vector<AText*> MoveTexts;
	AText* CurPPText = nullptr;
	AText* MaxPPText = nullptr;
	AText* MoveTypeText = nullptr;
};

