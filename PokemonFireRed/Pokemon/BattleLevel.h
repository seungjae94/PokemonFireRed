#pragma once
#include "PokemonLevel.h"
#include "PlayerData.h"
#include "BattleCanvas.h"
#include "BattleStartStateMachine.h"
#include "BattlePlayerActionSelectStateMachine.h"
#include "BattleEnums.h"
#include "StatStage.h"

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

protected:
private:
	// 데이터
	bool PlayerFirst = true;
	int PlayerPokemonIndex = 0;
	int EnemyPokemonIndex = 0;
	int PlayerMoveIndex = 0;
	int EnemyMoveIndex = 0;
	EBattleAction PlayerAction = EBattleAction::None;
	UPokemon* PlayerPokemon = nullptr;
	UPokemon* EnemyPokemon = nullptr;
	UStatStage PlayerStatStage;
	UStatStage EnemyStatStage;

	// 렌더링, 로직
	ABattleCanvas* Canvas = nullptr;
	ABattleStartStateMachine* BSSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PASM = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart(float _DeltaTime);
	void ProcessPlayerAction(float _DeltaTime);
	void PrepareFight();
	void ProcessPlayerMoveFailed(float _DeltaTime);
	void ProcessEnemyMoveFailed(float _DeltaTime);
	void ProcessPlayerMove(float _DeltaTime);
	void ProcessEnemyMove(float _DeltaTime);

	// FSM
	EBattleState State = EBattleState::BattleStart;
	float Timer = 0.0f;

	// 유틸
	UPokemon& GetCurPlayerPokemon()
	{
		return UPlayerData::GetPokemonInEntry(PlayerPokemonIndex);
	}

	// 원래 맵 레벨로 복귀
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName);
	}
};

