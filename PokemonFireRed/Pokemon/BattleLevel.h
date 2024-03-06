#pragma once
#include "PokemonLevel.h"
#include "PlayerData.h"
#include "BattleCanvas.h"
#include "BattleStartStateMachine.h"
#include "BattlePlayerActionSelectStateMachine.h"
#include "BattleTurnStateMachine.h"
#include "BattleActionStateMachine.h"
#include "BattleEOTStateMachine.h"
#include "BattleFaintStateMachine.h"
#include "BattleExpGainStateMachine.h"
#include "BattlePlayerShiftStateMachine.h"
#include "BattlePrepareTurnStateMachine.h"
#include "Battler.h"

class UBattleLevel : public UPokemonLevel
{
private:
	enum class EState
	{
		BattleStart,
		PrepareTurn,
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
	// ������
	UBattler Player;
	UBattler Enemy;
	ABattleCanvas* Canvas = nullptr;

	// FSM
	ABattlePrepareTurnStateMachine* BattlePrepareTurnSM = nullptr;
	ABattleStartStateMachine* BattleStartSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PlayerActionSelectSM = nullptr;
	ABattleTurnStateMachine* BattleTurnSM = nullptr;

	// BattleTurnSM�� ���������� ����ϴ� SM
	ABattleActionStateMachine* BattleActionSM = nullptr;
	ABattleEOTStateMachine* BattleEOTSM = nullptr;
	ABattleFaintStateMachine* BattleFaintSM = nullptr;

	// BattleActionSM�� ���������� ����ϴ� SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;
	ABattlePlayerShiftStateMachine* BattlePlayerShiftSM = nullptr;

	// BattleFaintSM�� ���������� ����ϴ� SM
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart();
	void ProcessPrepareTurn();
	void ProcessPlayerAction();
	void ProcessTurn();
	void ProcessFinishBattle();
	void ProcessRun();

	// FSM
	EState State = EState::BattleStart;
	const float MoveFailMessageShowTime = 1.5f;
	float Timer = 0.0f;

	// ���� �� ������ ����
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName);
		State = EState::End;
	}
};

