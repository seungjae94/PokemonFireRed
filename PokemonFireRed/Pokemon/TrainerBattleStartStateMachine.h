#pragma once
#include <EngineCore/Actor.h>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ATrainerBattleStartStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		FadeWait,
		GroundMove,
		EntryArrowMove,
		EntryBallMove,
		ZClickWait,
		EnemyPokemonAppear,
		EnemyPokemonBoxMove,
		PlayerBattlerThrow1,
		PlayerBattlerThrow2,
		PlayerPokemonTakeout,
		PlayerPokemonBoxMove,
		End
	};
public:
	// constructor destructor
	ATrainerBattleStartStateMachine();
	~ATrainerBattleStartStateMachine();

	// delete Function
	ATrainerBattleStartStateMachine(const ATrainerBattleStartStateMachine& _Other) = delete;
	ATrainerBattleStartStateMachine(ATrainerBattleStartStateMachine&& _Other) noexcept = delete;
	ATrainerBattleStartStateMachine& operator=(const ATrainerBattleStartStateMachine& _Other) = delete;
	ATrainerBattleStartStateMachine& operator=(ATrainerBattleStartStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, const UBattler* _Player, const UBattler* _Enemy)
	{
		Canvas = _Canvas;
		MsgBox = _MsgBox;
		Player = _Player;
		Enemy = _Enemy;
		State = ESubstate::FadeWait;
		Timer = FadeWaitTime;
	}

protected:

private:
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	const UBattler* Player = nullptr;
	const UBattler* Enemy = nullptr;
	float Timer = 0.0f;

	ESubstate State = ESubstate::None;
	const float FadeWaitTime = 0.5f;
	const float GroundMoveTime = 1.5f;
	const float EnemyArrowMoveTime = 0.5f;
	const float EntryBallMoveTime = 0.5f;
	const float EntryBallMoveInterval = 0.1f;
	int MovingBallIndex = 0;
	std::vector<float> BallTimers;

	const float EntryFadeTime = 0.5f;
	const float EnemyPokemonAppearTime = 1.0f;
	float EntryFadeTimer = 0.0f;
	float EnemyPokemonAppearTimer = 0.0f;

	const float EnemyPokemonBoxMoveTime = 0.5f;
	const float PlayerBattlerHideTime = 1.1f;
	const float PlayerPokemonTakeoutTime = 0.3f;
	const float PlayerPokemonBoxMoveTime = 0.5f;

	void Tick(float _DeltaTime) override;

	void ProcessFadeWait();
	void ProcessGroundMove();
	void ProcessEntryArrowMove();
	void ProcessEntryBallMove(float _DeltaTime);
	void ProcessZClickWait();
	void ProcessEnemyPokemonAppear(float _DeltaTime);
	void ProcessEnemyPokemonBoxMove();
	void ProcessPlayerBattlerThrow1(float _DeltaTime);
	void ProcessPlayerBattlerThrow2(float _DeltaTime);
	void ProcessPlayerPokemonTakeout();
	void ProcessPlayerPokemonBoxMove();
};

