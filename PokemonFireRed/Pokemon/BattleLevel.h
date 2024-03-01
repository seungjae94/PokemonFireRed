#pragma once
#include "PokemonLevel.h"
#include "PlayerData.h"
#include "BattleCanvas.h"
#include "BattleStartStateMachine.h"
#include "BattlePlayerActionSelectStateMachine.h"

enum class EEnemyType
{
	Wild,
	Trainer,
	GymLeader
};

enum class EBattleState
{
	BattleStart,
	PlayerActionSelect,
	PlayerMove,
	EnemyMove,
	PlayerSecondaryEffect,
	EnemySecondaryEffect,
	BattleEnd
};

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
	// ������
	int PlayerPokemonIndex = 0;
	EBattlePlayerAction PlayerAction = EBattlePlayerAction::None;
	UPokemon* PlayerPokemon = nullptr;
	UPokemon* EnemyPokemon = nullptr;

	// ������, ����
	ABattleCanvas* Canvas = nullptr;
	ABattleStartStateMachine* BSSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PASM = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart(float _DeltaTime);
	void ProcessPlayerAction(float _DeltaTime);

	// FSM
	EBattleState State = EBattleState::BattleStart;
	float Timer = 0.0f;

	// ��ƿ
	UPokemon& GetCurPlayerPokemon()
	{
		return UPlayerData::GetPokemonInEntry(PlayerPokemonIndex);
	}

	// ���� �� ������ ����
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName);
	}
};

