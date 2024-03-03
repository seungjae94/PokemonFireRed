#pragma once
#include "PokemonLevel.h"
#include "PlayerData.h"
#include "BattleCanvas.h"
#include "BattleStartStateMachine.h"
#include "BattlePlayerActionSelectStateMachine.h"
#include "BattleTurnStateMachine.h"
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

protected:
private:
	// µ¥ÀÌÅÍ
	UBattler Player;
	UBattler Enemy;
	bool PlayerFirst = true;

	// ·»´õ¸µ, ·ÎÁ÷ Ã³¸® °´Ã¼
	ABattleCanvas* Canvas = nullptr;
	ABattleStartStateMachine* BSSM = nullptr;
	ABattlePlayerActionSelectStateMachine* PASM = nullptr;
	ABattleTurnStateMachine* BTSM = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart(float _DeltaTime);
	void ProcessPlayerAction(float _DeltaTime);
	void ProcessTurn(float _DeltaTime);
	void ProcessBattleWin(float _DeltaTime);
	void ProcessBattleLose(float _DeltaTime);
	void ProcessRun(float _DeltaTime);

	// FSM
	EBattleState State = EBattleState::BattleStart;
	const float MoveFailMessageShowTime = 1.5f;
	float Timer = 0.0f;

	// ¿ø·¡ ¸Ê ·¹º§·Î º¹±Í
	std::string PrevMapName;

	void ReturnToMapLevel()
	{
		UEventManager::FadeChangeLevel(PrevMapName);
	}
};

