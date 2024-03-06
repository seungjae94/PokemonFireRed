#include "BattleFaintStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleUtil.h"

ABattleFaintStateMachine::ABattleFaintStateMachine()
{
}

ABattleFaintStateMachine::~ABattleFaintStateMachine()
{
}

void ABattleFaintStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Attacker, UBattler* _Defender, bool& _AttackerFaintChecked, bool& _DefenderFaintChecked)
{
	Canvas = _Canvas;
	Attacker = _Attacker;
	Defender = _Defender;
	
	const UPokemon* AttackerPokemon = Attacker->CurPokemon();
	const UPokemon* DefenderPokemon = Defender->CurPokemon();

	Fainters.clear();
	ExpGainByFaint.clear();

	if (AttackerPokemon->GetStatusId() == EPokemonStatus::Faint && _AttackerFaintChecked == false)
	{
		Fainters.push_back(Attacker);
		_AttackerFaintChecked = true;
	}

	if (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint && _DefenderFaintChecked == false)
	{
		Fainters.push_back(Defender);
		_DefenderFaintChecked = true;
	}

	State = ESubstate::TestFaint;
}

void ABattleFaintStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::TestFaint:
		ProcessTestFaint();
		break;
	case ESubstate::HidePokemon:
		ProcessHidePokemon();
		break;
	case ESubstate::ShowFaintMessage:
		ProcessShowFaintMessage();
		break;
	case ESubstate::TestExpGain:
		ProcessTestExpGain();
		break;
	case ESubstate::ExpGain:
		ProcessExpGain();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleFaintStateMachine::ProcessTestFaint()
{
	if (Fainters.size() > 0)
	{
		State = ESubstate::HidePokemon;
		Timer = FaintTime;
		Fainter = Fainters.front();
		Fainters.pop_front();
		return;
	}

	State = ESubstate::End;
}

void ABattleFaintStateMachine::ProcessHidePokemon()
{
	if (true == Fainter->IsPlayer())
	{
		Canvas->LerpFaintPlayerPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			State = ESubstate::ShowFaintMessage;
			Canvas->SetPlayerPokemonBoxActive(false);
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Fainter) + L"\nfainted!");
		}
	}
	else
	{
		Canvas->LerpFaintEnemyPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			State = ESubstate::ShowFaintMessage;
			Canvas->SetEnemyPokemonBoxActive(false);
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Fainter) + L"\nfainted!");
		}
	}
}

void ABattleFaintStateMachine::ProcessShowFaintMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		// 플레이어 포켓몬이 쓰러진 경우 경험치 획득 X
		if (true == Fainter->IsPlayer())
		{
			State = ESubstate::TestFaint;
		}
		// 적 포켓몬이 쓰러진 경우 경험치 획득 O
		else
		{
			ExpGainByFaint = UExpCalculator::SimFaint(Fainter);
			State = ESubstate::TestExpGain;
		}
	}
}

void ABattleFaintStateMachine::ProcessTestExpGain()
{
	if (ExpGainByFaint.size() > 0)
	{
		State = ESubstate::ExpGain;
		UPokemon* ExpGainer = ExpGainByFaint.begin()->first;
		BattleExpGainSM->Start(Canvas, ExpGainer, ExpGainByFaint.at(ExpGainer));
		return;
	}

	State = ESubstate::TestFaint;
}

void ABattleFaintStateMachine::ProcessExpGain()
{
	if (true == BattleExpGainSM->IsEnd())
	{
		ExpGainByFaint.erase(ExpGainByFaint.begin());
		State = ESubstate::TestFaint;
	}
}