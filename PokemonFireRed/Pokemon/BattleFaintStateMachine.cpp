#include "BattleFaintStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleUtil.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "SoundManager.h"

ABattleFaintStateMachine::ABattleFaintStateMachine()
{
}

ABattleFaintStateMachine::~ABattleFaintStateMachine()
{
}

void ABattleFaintStateMachine::Start(UBattler* _Attacker, UBattler* _Defender, bool& _AttackerFaintChecked, bool& _DefenderFaintChecked)
{
	ABattleStateMachine::Start();

	Attacker = _Attacker;
	Defender = _Defender;

	const UPokemon* AttackerPokemon = Attacker->CurPokemon();
	const UPokemon* DefenderPokemon = Defender->CurPokemon();

	if (Attacker->IsPlayer())
	{
		PlayerCurPokemon = AttackerPokemon;
	}
	else
	{
		PlayerCurPokemon = DefenderPokemon;
	}

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
	case ESubstate::CryWait:
		ProcessCryWait();
		break;
	case ESubstate::HidePokemon:
		ProcessHidePokemon();
		break;
	case ESubstate::ShowFaintMessage1:
		ProcessShowFaintMessage1();
		break;
	case ESubstate::ShowFaintMessage2:
		ProcessShowFaintMessage2();
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
		State = ESubstate::CryWait;
		Fainter = Fainters.front();
		Fainters.pop_front();
		USoundManager::PlaySE(Fainter->CurPokemon()->GetCrySoundName());
		Timer = CryWaitTime;
		return;
	}

	State = ESubstate::End;
}

void ABattleFaintStateMachine::ProcessCryWait()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::HidePokemon;
		Timer = FaintTime;
		USoundManager::PlaySE(RN::SEFaint);
	}
}

void ABattleFaintStateMachine::ProcessHidePokemon()
{
	if (true == Fainter->IsPlayer())
	{
		Canvas->LerpFaintPlayerPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			State = ESubstate::ShowFaintMessage1;
			Canvas->SetPlayerPokemonBoxActive(false);
			MsgBox->SetMessage(UBattleUtil::GetPokemonFullName(Fainter) + L"\nfainted!");
			MsgBox->Write();
		}
	}
	else
	{
		Canvas->LerpFaintEnemyPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			State = ESubstate::ShowFaintMessage1;
			Canvas->SetEnemyPokemonBoxActive(false);
			MsgBox->SetMessage(UBattleUtil::GetPokemonFullName(Fainter) + L"\nfainted!");
			MsgBox->Write();
		}
	}
}

void ABattleFaintStateMachine::ProcessShowFaintMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::ShowFaintMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void ABattleFaintStateMachine::ProcessShowFaintMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		USoundManager::PlaySE(RN::SEClick);
		MsgBox->HideSkipArrow();

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
		BattleExpGainSM->Start(ExpGainer, ExpGainByFaint.at(ExpGainer), PlayerCurPokemon == ExpGainer);
		return;
	}

	State = ESubstate::TestFaint;
}

void ABattleFaintStateMachine::ProcessExpGain()
{
	if (true == BattleExpGainSM->IsEnd())
	{
		ExpGainByFaint.erase(ExpGainByFaint.begin());
		State = ESubstate::TestExpGain;
	}
}