#pragma once
#include "PokemonLevel.h"
#include "PlayerData.h"
#include "Battler.h"
#include "BattleCanvas.h"
#include "AnimatorGenerator.h"
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
#include "FinishBattleStateMachine.h"
#include "BlinkEffectAnimator.h"
#include "ShakeEffectAnimator.h"

class UBattleLevel : public UPokemonLevel
{
	friend class ABattleStateMachine;
	friend class AAnimator;
private:
	enum class EState
	{
		BattleStart,
		PrepareTurn1,
		PrepareTurn2,
		PlayerActionSelect,
		Turn,
		FinishBattle,
		WaitBeforeReturn,
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
	AAnimatorGenerator* AnimatorGenerator = nullptr;

	EBattleEndReason BattleEndReason = EBattleEndReason::None;
	const float WaitBeforeReturnTime = 1.0f;

	// FSM
	ABattlePrepareTurnStateMachine* BattlePrepareTurnSM = nullptr;
	AWildBattleStartStateMachine* WildBattleStartSM = nullptr;
	ATrainerBattleStartStateMachine* TrainerBattleStartSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PlayerActionSelectSM = nullptr;
	ABattleTurnStateMachine* BattleTurnSM = nullptr;
	AFinishBattleStateMachine* FinishBattleSM = nullptr;

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

	// 공용 애니메이터
	ABlinkEffectAnimator* BlinkEffectAnimator = nullptr;
	AShakeEffectAnimator* ShakeEffectAnimator = nullptr;

	// 레벨 함수
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	// 상태 틱
	void ProcessBattleStart();
	void ProcessPrepareTurn1();
	void ProcessPrepareTurn2();
	void ProcessPlayerAction();
	void ProcessTurn();
	void ProcessFinishBattle();
	void ProcessWaitBeforeReturn();
	void ProcessRun();

	// FSM
	EState State = EState::BattleStart;
	const float MoveFailMessageShowTime = 1.5f;
	float Timer = 0.0f;

	// 원래 맵 레벨로 복귀
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName, true);
		State = EState::End;
	}
};

