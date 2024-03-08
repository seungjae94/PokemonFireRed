#pragma once
#include "Canvas.h"
#include "Pokemon.h"
#include "Battler.h"

class ABattleCanvas : public ACanvas
{
	friend class AWildBattleStartStateMachine;
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

private: // 초기화 함수
	void InitPlayerImages();	// 플레이어 배틀러, 플레이어 포켓몬, 볼 위치 초기화
	void InitEnemyImages();		// 적 배틀러, 적 포켓몬, 적 볼 위치 초기화
	void InitPlayerUI();
	void InitEnemyUI();

public:	// Refresh 함수
	void RefreshEnemyPokemonBox();
	void RefreshPlayerPokemonBox();
	void RefreshMoveSelectBox();
	void RefreshPlayerEntry();
	void RefreshEnemyEntry();
public:
	// BattleStart
	void LerpShowGrounds(float _t);
	void LerpShowEnemyPokemonBox(float _t);
	void LerpShowEnemyEntryArrow(float _t);
	void LerpShowPlayerEntryArrow(float _t);
	void LerpEnemyEntryBall(int _Index, float _t);
	void LerpPlayerEntryBall(int _Index, float _t);
	void LerpHideEnemyEntry(float _t);
	void LerpHidePlayerEntry(float _t);
	void PlayBattlerThrowingAnimation();
	void LerpHidePlayerBattler(float _t);
	void LerpHideEnemyBattler(float _t);
	void PlayThrowedBallAnimation();
	bool IsThrowedBallAnimationEnd();
	void SetThrowedBallActive(bool _Value);
	void TakeInPlayerPokemonToBall(float _t);
	void TakeOutPlayerPokemonFromBall(float _t);
	void TakeInEnemyPokemonFromBall(float _t);
	void TakeOutEnemyPokemonFromBall(float _t);
	void LerpShowPlayerPokemonBox(float _t);
	void SetActionBoxActive(bool _Value);
	void SetMoveSelectBoxActive(bool _Value);

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
	void LerpFaintPlayerPokemon(float _t);
	void LerpFaintEnemyPokemon(float _t);
	void SetPlayerPokemonBoxActive(bool _Value);
	void SetEnemyPokemonBoxActive(bool _Value);
	void SetPlayerPokemonImageActive(bool _Value);
	void SetEnemyPokemonImageActive(bool _Value);
	void LerpExpBar(int _BeforeExp, int _AfterExp, int _MaxExp, float _t);
	void ShowStatUpBox(const FLevelUpData& _LevelUpData);
	void ShowStatAfterBox(const UPokemon* _Pokemon);
	void HideStatUpWindow();
protected:

private:
	const UBattler* Player = nullptr;
	const UBattler* Enemy = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;


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
	FVector PlayerPokemonImageFaintPos;
	FVector PlayerBattlerInitPos;
	FVector PlayerBattlerHidePos;
	FVector EnemyBattlerInitPos;
	FVector EnemyBattlerHidePos;
	FVector EnemyPokemonImageInitPos;
	FVector EnemyPokemonImageHidePos;
	FVector EnemyPokemonImageFaintPos;
	FVector PlayerEntryArrowInitPos;
	FVector PlayerEntryArrowHidePos;
	FVector EnemyEntryArrowInitPos;
	FVector EnemyEntryArrowHidePos;
	std::vector<FVector> EnemyEntryBallsInitPos;
	std::vector<FVector> EnemyEntryBallsHidePos;
	std::vector<FVector> PlayerEntryBallsInitPos;
	std::vector<FVector> PlayerEntryBallsHidePos;
	int HpBarMaxValue = 1000;
	int ExpBarMaxValue = 1000;

	// 최상위 요소
	AImageElement* Background = nullptr;
	AImageElement* ActionBox = nullptr;
	AImageElement* MoveSelectBox = nullptr;
	AImageElement* ThrowedBall = nullptr;		// 볼 날아가는 애니메이션
	AImageElement* PlayerGround = nullptr;
	AImageElement* EnemyGround = nullptr;
	AImageElement* PlayerPokemonBox = nullptr;
	AImageElement* EnemyPokemonBox = nullptr;
	AImageElement* StatBox = nullptr;
	AImageElement* PlayerEntryArrow = nullptr;
	AImageElement* EnemyEntryArrow = nullptr;

	// EnemyPokemonBox 요소
	AText* EnemyPokemonNameText = nullptr;
	AText* EnemyPokemonLevelText = nullptr;
	AScrollBar* EnemyPokemonHpBar = nullptr;
	AImageElement* EnemyPokemonGenderMark = nullptr;
	AImageElement* EnemyPokemonStatusMark = nullptr;

	// PlayerPokemonBox 요소
	AText* PlayerPokemonNameText = nullptr;
	AText* PlayerPokemonLevelText = nullptr;
	AText* PlayerPokemonCurHpText = nullptr;
	AText* PlayerPokemonHpText = nullptr;
	AScrollBar* PlayerPokemonHpBar = nullptr;
	AScrollBar* PlayerPokemonExpBar = nullptr;
	AImageElement* PlayerPokemonGenderMark = nullptr;
	AImageElement* PlayerPokemonStatusMark = nullptr;

	// EnemyPokemonGround 요소
	AImageElement* EnemyPokemonImage = nullptr;
	AImageElement* EnemyBattler = nullptr;

	// PlayerPokemonGround 요소
	AImageElement* PlayerPokemonImage = nullptr;
	AImageElement* PlayerBattler = nullptr;

	// ActionBox 요소
	ACursor* ActionCursor = nullptr;

	// MoveSelectBox 요소
	ACursor* MoveSelectCursor = nullptr;
	std::vector<AText*> MoveTexts;
	AText* CurPPText = nullptr;
	AText* MaxPPText = nullptr;
	AText* MoveTypeText = nullptr;

	// StatBox 요소
	AText* StatHpText = nullptr;
	AText* StatAtkText = nullptr;
	AText* StatDefText = nullptr;
	AText* StatSpAtkText = nullptr;
	AText* StatSpDefText = nullptr;
	AText* StatSpeedText = nullptr;

	// EntryArrow 요소
	std::vector<AImageElement*> PlayerEntryBalls;
	std::vector<AImageElement*> EnemyEntryBalls;
};

