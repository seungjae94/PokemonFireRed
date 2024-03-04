#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "MoveEffectTester.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"
#include "Battler.h"
#include "BattleUtil.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy)
{
	// 입력 데이터 저장
	Canvas = _Canvas;
	Player = _Player;
	Enemy = _Enemy;

	// 적 액션 생성
	UBattleEnemyActionGenerator::Generate(Enemy);

	// 턴 순서 결정
	bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(Player, Enemy);
	if (true == IsPlayerFirst)
	{
		SetPlayerAsAttacker();
	}
	else
	{
		SetEnemyAsAttacker();
	}

	State = ESubstate::Action1;
	Reason = EEndReason::None;
	BattleActionSM->Start(Canvas, Attacker, Defender);
}

void ABattleTurnStateMachine::ProcessAction1(float _DeltaTime)
{
	if (true == BattleActionSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 즉시 종료해야 한다.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// Attacker 또는 Defender가 죽어 있으면 EOT로 보낸다.
		const UPokemon* AttackerPokemon = Attacker->CurPokemon();
		const UPokemon* DefenderPokemon = Defender->CurPokemon();

		bool AttackerFaint = (AttackerPokemon->GetStatusId() == EPokemonStatus::Faint);
		bool DefenderFaint = (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (AttackerFaint && DefenderFaint)
		{
			// 페인트 이펙트는 BattleActionSM이 처리한다.
			State = ESubstate::End;
			return;
		}
		else if (AttackerFaint)
		{
			// 페인트 이펙트는 BattleActionSM이 처리한다.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Defender);
			return;
		}
		else if (DefenderFaint)
		{
			// 페인트 이펙트는 BattleActionSM이 처리한다.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Attacker);
		}

		// 둘 다 살아있다면 Action2로 보낸다.
		State = ESubstate::Action2;

		SwapAttackerAndDefender();
		BattleActionSM->Start(Canvas, Attacker, Defender);
	}
}

void ABattleTurnStateMachine::ProcessAction2(float _DeltaTime)
{
	if (true == BattleActionSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 즉시 종료해야 한다.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// Attacker 또는 Defender가 죽어 있으면 EOT로 보낸다.
		const UPokemon* AttackerPokemon = Attacker->CurPokemon();
		const UPokemon* DefenderPokemon = Defender->CurPokemon();

		bool AttackerFaint = (AttackerPokemon->GetStatusId() == EPokemonStatus::Faint);
		bool DefenderFaint = (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (AttackerFaint && DefenderFaint)
		{
			// 페인트 이펙트는 BattleActionSM이 처리한다.
			State = ESubstate::End;
			return;
		}
		else if (AttackerFaint)
		{
			// 페인트 이펙트는 BattleActionSM이 처리한다.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Defender);
			return;
		}
		else if (DefenderFaint)
		{
			// 페인트 이펙트는 BattleActionSM이 처리한다.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Attacker);
		}

		// 둘 다 살아있다면 EOT1로 보낸다.
		bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirstEOT(Player, Enemy);

		if (true == IsPlayerFirst)
		{
			SetPlayerAsEOTTarget();
		}
		else
		{
			SetEnemyAsEOTTarget();
		}

		State = ESubstate::EndOfTurn1;
		BattleEOTSM->Start(Canvas, EOTTarget);
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn1(float _DeltaTime)
{
	if (true == BattleEOTSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 즉시 종료해야 한다.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		const UPokemon* NextPokemon = EOTNextTarget->CurPokemon();

		bool NextPokemonFaint = (NextPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (true == NextPokemonFaint)
		{
			// 다음 포켓몬이 죽어 EOT를 적용받지 못할 경우 바로 턴을 종료한다.
			State = ESubstate::End;
			return;
		}

		State = ESubstate::EndOfTurn2;
		BattleEOTSM->Start(Canvas, EOTNextTarget);
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn2(float _DeltaTime)
{
	if (true == BattleEOTSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 즉시 종료해야 한다.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		State = ESubstate::End;
	}
}

void ABattleTurnStateMachine::SwapAttackerAndDefender()
{
	UBattler* Tmp = Attacker;
	Attacker = Defender;
	Defender = Tmp;
}

void ABattleTurnStateMachine::EndTurnWithReason()
{
	if (true == Enemy->AllFaint())
	{
		if (true == Enemy->IsWildPokemon())
		{
			Reason = EEndReason::WinToWild;
		}
		else
		{
			Reason = EEndReason::WinToTrainer;
		}
	}
	else if (true == Player->AllFaint())
	{
		if (true == Enemy->IsWildPokemon())
		{
			Reason = EEndReason::LoseToWild;
		}
		else
		{
			Reason = EEndReason::LoseToTrainer;
		}
	}

	State = ESubstate::End;
}

void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::Action1:
		ProcessAction1(_DeltaTime);
		break;
	case ESubstate::Action2:
		ProcessAction2(_DeltaTime);
		break;
	case ESubstate::EndOfTurn1:
		ProcessEndOfTurn1(_DeltaTime);
		break;
	case ESubstate::EndOfTurn2:
		ProcessEndOfTurn2(_DeltaTime);
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::SetPlayerAsAttacker()
{
	Attacker = Player;
	Defender = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsAttacker()
{
	Attacker = Enemy;
	Defender = Player;
}

void ABattleTurnStateMachine::SetPlayerAsEOTTarget()
{
	EOTTarget = Player;
	EOTNextTarget = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsEOTTarget()
{
	EOTTarget = Enemy;
	EOTNextTarget = Player;
}