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
	// ������
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

	// BattlePrepareTurnSM�� ���������� ����ϴ� SM
	ABattleEnemyShiftStateMachine* BattleEnemyShiftSM = nullptr;
	ABattlePlayerShiftStateMachine* BattlePlayerShiftSM = nullptr;

	// BattleTurnSM�� ���������� ����ϴ� SM
	ABattleActionStateMachine* BattleActionSM = nullptr;
	ABattleEOTStateMachine* BattleEOTSM = nullptr;
	ABattleFaintStateMachine* BattleFaintSM = nullptr;

	// BattleActionSM�� ���������� ����ϴ� SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;

	// BattleFaintSM�� ���������� ����ϴ� SM
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;

	// ���� �ִϸ�����
	ABlinkEffectAnimator* BlinkEffectAnimator = nullptr;
	AShakeEffectAnimator* ShakeEffectAnimator = nullptr;

	// ���� �Լ�
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	// ���� ƽ
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

	// ���� �� ������ ����
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName, true);
		State = EState::End;
	}
};

