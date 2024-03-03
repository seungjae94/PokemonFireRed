#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start(
	ABattleCanvas* _Canvas,
	UPokemon* _PlayerPokemon, UPokemon* _EnemyPokemon,
	UStatStage& _PlayerStatStage, UStatStage& _EnemyStatStage,
	EBattleAction _PlayerAction, int _PlayerMoveIndex
)
{
	// 입력 데이터 저장
	Canvas = _Canvas;
	PlayerPokemon = _PlayerPokemon;
	EnemyPokemon = _EnemyPokemon;
	PlayerStatStage = &_PlayerStatStage;
	EnemyStatStage = &_EnemyStatStage;
	PlayerAction = _PlayerAction;
	PlayerMoveId = PlayerPokemon->GetMoveId(_PlayerMoveIndex);

	// 적 액션 생성
	GenerateEnemyAction();

	// 턴 순서 결정
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(PlayerAction, EnemyAction, PlayerPokemon, EnemyPokemon, PlayerMoveId, EnemyMoveId, *PlayerStatStage, *EnemyStatStage);
	IsFirstTurn = true;

	// 상태 변경
	if (true == IsPlayerFirst)
	{
		SetPlayerAsAttacker();
	}
	else
	{
		SetEnemyAsAttacker();
	}
	DispatchTurn();
}

void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleTurnStateMachine::ESubstate::None:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerEscapeFail:
		ProcessEscapeFail(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::Switch:
		break;
	case ABattleTurnStateMachine::ESubstate::UseItem:
		break;
	case ABattleTurnStateMachine::ESubstate::MoveFail:
		ProcessMoveFail(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveAnim:
		ProcessMoveAnim(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveDamage:
		ProcessMoveDamage(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveSecondaryEffectAnim:
		ProcessMoveSecondaryEffectAnim(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveSecondaryEffectDamage:
		ProcessMoveSecondaryEffectDamage(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn:
		ProcessEndOfTurn(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::GenerateEnemyAction()
{
	// 상대 행동 생성
	EnemyAction = UBattleEnemyActionGenerator::Generate(EEnemyType::Wild, EnemyPokemon);
	int EnemyMoveIndex = UBattleEnemyActionGenerator::GetGeneratedMoveIndex();
	EnemyMoveId = EnemyPokemon->GetMoveId(EnemyMoveIndex);
}

void ABattleTurnStateMachine::DispatchTurn()
{
	EBattleAction AttackerAction;

	if (Attacker == PlayerPokemon)
	{
		AttackerAction = PlayerAction;
	}
	else
	{
		AttackerAction = EnemyAction;
	}

	switch (AttackerAction)
	{
	case EBattleAction::Fight:
		DispatchFight();
		break;
	case EBattleAction::Escape:
	{
		State = ESubstate::PlayerEscapeFail;
		Canvas->SetBattleMessage(L"Can't Escape!");
	}
	break;
	case EBattleAction::Shift:
	{
		State = ESubstate::Switch;
		Canvas->SetBattleMessage(L"Debug - Attacker Changed Pokemon!");
	}
	break;
	case EBattleAction::Item:
	{
		State = ESubstate::UseItem;
		Canvas->SetBattleMessage(L"Debug - Attacker used Item!");
	}
	break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchFight()
{
	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender, *AttackerStatStage, *DefenderStatStage, AttackMoveId);

	if (false == AccuracyCheckResult)
	{
		Canvas->SetBattleMessage(Attacker->GetNameW() + L"'s\nattack failed!");
		State = ESubstate::MoveFail;
		Timer = BattleMsgShowTime;
		return;
	}

	State = ESubstate::MoveAnim;
	const FPokemonMove* Move = UPokemonDB::FindMove(AttackMoveId);
	Canvas->SetBattleMessage(Attacker->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	Result = UDamageCalculator::CalcDamage(Attacker, Defender, *AttackerStatStage, *DefenderStatStage, AttackMoveId);
}

void ABattleTurnStateMachine::DispatchEndOfTurn()
{
	Canvas->SetBattleMessage(L"Debug - End of turn");
}

void ABattleTurnStateMachine::ProcessEscapeFail(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		IsFirstTurn = false;
		SetEnemyAsAttacker();
		DispatchTurn();
		return;
	}
}

void ABattleTurnStateMachine::ProcessMoveFail(float _DeltaTime)
{
	if (Timer < 0.0f)
	{
		if (true == IsFirstTurn)
		{
			IsFirstTurn = false;
			if (Attacker == PlayerPokemon)
			{
				SetEnemyAsAttacker();
				DispatchTurn();
			}
			else
			{
				SetPlayerAsAttacker();
				DispatchTurn();
			}
			return;
		}
		else
		{
			IsFirstTurn = true;

			// (원칙적으로는) End Of Turn에 대한 순서도 따로 계산을 해줘야 한다.
			// 하지만 지금까지 구현한 내용만 고려하면 기술 사용 순서와 EOT 순서가 같다.
			IsPlayerFirstEOT = IsPlayerFirst; // UTurnOrderCalculator::IsPlayerFirstEOT(..., IsPlayerFirst);

			if (true == IsPlayerFirstEOT)
			{
				SetPlayerAsAttacker();
			}
			else
			{
				SetEnemyAsAttacker();
			}

			DispatchEndOfTurn();
			return;
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveAnim(float _DeltaTime)
{
	// 애니메이션: 차후 구현
	State = ESubstate::MoveDamage;
	Timer = DamageTime;
	MoveResultMsg = EMoveResultMsg::None;
}

void ABattleTurnStateMachine::ProcessMoveDamage(float _DeltaTime)
{
	if (MoveResultMsg == EMoveResultMsg::None)
	{
		int CurHp = Defender->GetCurHp();
		int DamagedHp = UPokemonMath::Max(CurHp - Result.Damage, 0);
		if (Defender == PlayerPokemon)
		{
			Canvas->LerpPlayerHpBar(CurHp, DamagedHp, Defender->GetHp(), Timer / DamageTime);
		}
		else
		{
			Canvas->LerpEnemyHpBar(CurHp, DamagedHp, Defender->GetHp(), Timer / DamageTime);
		}

		if (Timer <= 0.0f)
		{
			// 실제 데미지 처리
			Defender->SetCurHp(DamagedHp);
			
			// 데미지 계산 결과 렌더링
			if (Defender == PlayerPokemon)
			{
				Canvas->RefreshPlayerPokemonCurHpText();
			}


			if (Result.IsCritical == true)
			{
				MoveResultMsg = EMoveResultMsg::Critical;
				Canvas->SetBattleMessage(L"A critical hit!");
				Timer = BattleMsgShowTime;
				return;
			}
			else if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveResultMsg::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(Defender->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}
			
			// 데미지 종료
			State = ESubstate::MoveSecondaryEffectAnim;
		}
	}
	else if (MoveResultMsg == EMoveResultMsg::Critical)
	{
		if (Timer <= 0.0f)
		{
			if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveResultMsg::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(Defender->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}

			// 데미지 종료
			State = ESubstate::MoveSecondaryEffectAnim;
		}
	}
	else /*MoveResultMsg == EMoveResultMsg::TypeEffect*/
	{
		if (Timer <= 0.0f)
		{
			// 데미지 종료
			State = ESubstate::MoveSecondaryEffectAnim;
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveSecondaryEffectAnim(float _DeltaTime)
{
	// TODO
	State = ESubstate::MoveSecondaryEffectDamage;
}

void ABattleTurnStateMachine::ProcessEndOfTurn(float _DeltaTime)
{
	// TODO
	State = ESubstate::End;
}

void ABattleTurnStateMachine::ProcessMoveSecondaryEffectDamage(float _DeltaTime)
{
	// TODO
	if (true == IsFirstTurn)
	{
		if (Attacker == PlayerPokemon)
		{
			SetEnemyAsAttacker();
		}
		else
		{
			SetPlayerAsAttacker();
		}
		IsFirstTurn = false;
		DispatchTurn();
		return;
	}

	// 후공이 끝난 경우
	State = ESubstate::EndOfTurn;
}

void ABattleTurnStateMachine::SetPlayerAsAttacker()
{
	Attacker = PlayerPokemon;
	Defender = EnemyPokemon;
	AttackerStatStage = PlayerStatStage;
	DefenderStatStage = EnemyStatStage;
	AttackMoveId = PlayerMoveId;
}

void ABattleTurnStateMachine::SetEnemyAsAttacker()
{
	Attacker = EnemyPokemon;
	Defender = PlayerPokemon;
	AttackerStatStage = EnemyStatStage;
	DefenderStatStage = PlayerStatStage;
	AttackMoveId = EnemyMoveId;
}