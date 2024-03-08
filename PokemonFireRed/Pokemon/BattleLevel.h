#pragma once
#include "PokemonLevel.h"
#include "PlayerData.h"
#include "BattleCanvas.h"
#include "WildBattleStartStateMachine.h"
#include "TrainerBattleStartStateMachine.h"
#include "BattlePlayerActionSelectStateMachine.h"
#include "BattleTurnStateMachine.h"
#include "BattleActionStateMachine.h"
#include "BattleEOTStateMachine.h"
#include "BattleFaintStateMachine.h"
#include "BattleExpGainStateMachine.h"
#include "BattlePlayerShiftStateMachine.h"
#include "BattleEnemyShiftStateMachine.h"
#include "BattlePrepareTurnStateMachine.h"
#include "Battler.h"

class UBattleLevel : public UPokemonLevel
{
private:
	enum class EState
	{
		BattleStart,
		PrepareTurn1,
		PrepareTurn2,
		PlayerActionSelect,
		Turn,
		FinishBattle,
		Run,
		End,
	};
public:
	// constructor destructor
	UBattleLevel();
	~UBattleLevel();

	// delete Function
	UBattleLevel(const UBattleLevel& _Other) = delete;
	UBattleLevel(UBattleLevel&& _Other) noexcept = delete;
	UBattleLevel& operator=(const UBattleLevel& _Other) = delete;
	UBattleLevel& operator=(UBattleLevel&& _Other) noexcept = delete;

	UBattler* GetPlayerBattler()
	{
		return &Player;
	}

protected:
private:
	// 데이터
	UBattler Player;
	UBattler Enemy;
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;

	// FSM
	ABattlePrepareTurnStateMachine* BattlePrepareTurnSM = nullptr;
	AWildBattleStartStateMachine* WildBattleStartSM = nullptr;
	ATrainerBattleStartStateMachine* TrainerBattleStartSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PlayerActionSelectSM = nullptr;
	ABattleTurnStateMachine* BattleTurnSM = nullptr;

	// BattlePrepareTurnSM이 내부적으로 사용하는 SM
	ABattleEnemyShiftStateMachine* BattleEnemyShiftSM = nullptr;
	ABattlePlayerShiftStateMachine* BattlePlayerShiftSM = nullptr;

	// BattleTurnSM이 내부적으로 사용하는 SM
	ABattleActionStateMachine* BattleActionSM = nullptr;
	ABattleEOTStateMachine* BattleEOTSM = nullptr;
	ABattleFaintStateMachine* BattleFaintSM = nullptr;

	// BattleActionSM이 내부적으로 사용하는 SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;

	// BattleFaintSM이 내부적으로 사용하는 SM
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart();
	void ProcessPrepareTurn1();
	void ProcessPrepareTurn2();
	void ProcessPlayerAction();
	void ProcessTurn();
	void ProcessFinishBattle();
	void ProcessRun();

	// FSM
	EState State = EState::BattleStart;
	const float MoveFailMessageShowTime = 1.5f;
	float Timer = 0.0f;

	// 원래 맵 레벨로 복귀
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName);
		State = EState::End;
	}
};

