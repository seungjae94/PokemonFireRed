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
#include "Battler.h"

class UBattleLevel : public UPokemonLevel
{
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
	bool PlayerFirst = true;

	// 렌더링, 로직 처리 객체
	ABattleCanvas* Canvas = nullptr;
	ABattleStartStateMachine* BattleStartSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PlayerActionSelectSM = nullptr;
	ABattleTurnStateMachine* BattleTurnSM = nullptr;
	ABattleActionStateMachine* BattleActionSM = nullptr;		// BattleTurnSM이 내부적으로 사용하는 SM
	ABattleEOTStateMachine* BattleEOTSM = nullptr;				// BattleTurnSM이 내부적으로 사용하는 SM
	ABattleFaintStateMachine* BattleFaintSM = nullptr;			// BattleTurnSM이 내부적으로 사용하는 SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;			// BattleActionSM이 내부적으로 사용하는 SM
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;		// BattleFaintSM이 내부적으로 사용하는 SM

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart();
	void ProcessPlayerAction();
	void ProcessTurn();
	void ProcessBattleEnd();
	void ProcessRun();

	// FSM
	EBattleState State = EBattleState::BattleStart;
	const float MoveFailMessageShowTime = 1.5f;
	float Timer = 0.0f;

	// 원래 맵 레벨로 복귀
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName);
	}
};

