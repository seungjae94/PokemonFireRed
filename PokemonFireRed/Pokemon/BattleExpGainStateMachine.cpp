#include "BattleExpGainStateMachine.h"
#include <EnginePlatform/EngineInput.h>

ABattleExpGainStateMachine::ABattleExpGainStateMachine() 
{
}

ABattleExpGainStateMachine::~ABattleExpGainStateMachine() 
{
}

void ABattleExpGainStateMachine::Start(ABattleCanvas* _Canvas, UPokemon* _ExpGainer, int _Exp)
{
	Canvas = _Canvas;
	ExpGainer = _ExpGainer;
	Exp = _Exp;

	State = ESubstate::ExpGainMessage;
	std::wstring BattleMsg = ExpGainer->GetNameW() + L" gained\n";
	BattleMsg += std::to_wstring(_Exp);
	BattleMsg += L" Exp. Points!";
	Canvas->SetBattleMessage(BattleMsg);
}

void ABattleExpGainStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleExpGainStateMachine::ESubstate::None:
		break;
	case ABattleExpGainStateMachine::ESubstate::ExpGainMessage:
		ProcessExpGainMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::TestExpBarIncrease:
		ProcessTestExpBarIncrease();
		break;
	case ABattleExpGainStateMachine::ESubstate::ExpBarIncrease:
		ProcessExpBarIncrease();
		break;
	case ABattleExpGainStateMachine::ESubstate::LevelUpEffect:
		ProcessLevelUpEffect();
		break;
	case ABattleExpGainStateMachine::ESubstate::LevelUpMessage:
		ProcessLevelUpMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::StatUpDiffWindow:
		ProcessStatUpDiffWindow();
		break;
	case ABattleExpGainStateMachine::ESubstate::StatUpAfterWindow:
		ProcessStatUpAfterWindow();
		break;
	case ABattleExpGainStateMachine::ESubstate::TestLearnMove:
		ProcessTestLearnMove();
		break;
	case ABattleExpGainStateMachine::ESubstate::LearnMoveSuccessMessage:
		ProcessLearnMoveSuccessMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::LearnMoveFailMessage:
		ProcessLearnMoveFailMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::LearnMoveEraseMessage:
		ProcessLearnMoveEraseMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::LearnMoveEraseSelection:
		ProcessLearnMoveEraseSelection();
		break;
	case ABattleExpGainStateMachine::ESubstate::LearnMoveNoEraseMessage:
		ProcessLearnMoveEraseMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::LearnMoveEraseSuccessMessage:
		ProcessLearnMoveSuccessMessage();
		break;
	case ABattleExpGainStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleExpGainStateMachine::ProcessExpGainMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		SimResult = UExpCalculator::SimExpGain(ExpGainer, Exp);
		State = ESubstate::TestExpBarIncrease;
	}
}

void ABattleExpGainStateMachine::ProcessTestExpBarIncrease()
{
	// ����ġ�� ��� �ø� ���
	if (ExpGainer->GetAccExp() == SimResult.NextAccExp)
	{
		State = ESubstate::End;
		return;
	}

	State = ESubstate::ExpBarIncrease;
	Timer = ExpBarIncTime;
}

void ABattleExpGainStateMachine::ProcessExpBarIncrease()
{
	int CurExp = ExpGainer->GetExp();
	int ExpSize = ExpGainer->GetExpSize();

	// ��ǥ ������ ������ ���
	if (ExpGainer->GetLevel() == SimResult.NextLevel)
	{
		int DestExp = SimResult.NextExp;
		Canvas->LerpExpBar(CurExp, DestExp, ExpSize, Timer / ExpBarIncTime);

		if (Timer <= 0.0f)
		{
			// ����ġ ȹ��
			ExpGainer->AddAccExp(DestExp - CurExp);
			State = ESubstate::End;
		}
	}
	// ��ǥ ������ �������� ���� ���
	else
	{
		int DestExp = ExpSize;
		Canvas->LerpExpBar(CurExp, DestExp, ExpSize, Timer / ExpBarIncTime);

		if (Timer <= 0.0f)
		{
			// ������ ����Ʈ
			State = ESubstate::LevelUpEffect;
			Timer = LevelUpEffectTime;
		}
	}
}

void ABattleExpGainStateMachine::ProcessLevelUpEffect()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::LevelUpMessage;

		std::wstring BattleMsg = ExpGainer->GetNameW();
		BattleMsg += L" grew to\nLV. ";
		BattleMsg += std::to_wstring(ExpGainer->GetLevel() + 1);
		BattleMsg += L"!";
		Canvas->SetBattleMessage(BattleMsg);

		// ���� ������ ó��
		LevelUpData = ExpGainer->LevelUp();

		// ���� ���� �÷��̾� UI�� �ݿ�
		Canvas->RefreshPlayerPokemonBox();
	}
}

void ABattleExpGainStateMachine::ProcessLevelUpMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::StatUpDiffWindow;
		Canvas->ShowStatUpBox(LevelUpData);
	}
}

void ABattleExpGainStateMachine::ProcessStatUpDiffWindow()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::StatUpAfterWindow;
		Canvas->ShowStatAfterBox(ExpGainer);
	}
}

void ABattleExpGainStateMachine::ProcessStatUpAfterWindow()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::TestLearnMove;
		Canvas->HideStatUpWindow();
	}
}

void ABattleExpGainStateMachine::ProcessTestLearnMove()
{
	// ��� ����� �ִ� ���
	if (LevelUpData.Moves.size() > 0)
	{
		// ���� ����� ������ ��� �� �ִ� ���
		if (ExpGainer->GetMoveCount() == 4)
		{
			State = ESubstate::LearnMoveEraseMessage;
		}
		// ���� ����� ������ �ʰ� ��� �� �ִ� ���
		else
		{
			State = ESubstate::LearnMoveSuccessMessage;

			EPokemonMove MoveId = LevelUpData.Moves.front();
			const FPokemonMove* Move = UPokemonDB::FindMove(MoveId);

			std::wstring BattleMsg = ExpGainer->GetNameW();
			BattleMsg += L" learned\n";
			BattleMsg += Move->GetNameW();
			BattleMsg += L"!";
			Canvas->SetBattleMessage(BattleMsg);
		}
	}
	else
	{
		State = ESubstate::TestExpBarIncrease;
	}
}

void ABattleExpGainStateMachine::ProcessLearnMoveSuccessMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		// ��� ����� �� ���� ���� �ִ�.
		State = ESubstate::TestLearnMove;
		
		// ���� ��� �н� ó��
		EPokemonMove MoveId = LevelUpData.Moves.front();
		ExpGainer->LearnMove(MoveId);

		if (LevelUpData.Moves.size() > 0)
		{
			LevelUpData.Moves.pop_front();
		}
	}
}

void ABattleExpGainStateMachine::ProcessLearnMoveFailMessage()
{
}

void ABattleExpGainStateMachine::ProcessLearnMoveEraseMessage()
{
}

void ABattleExpGainStateMachine::ProcessLearnMoveEraseSelection()
{
}

void ABattleExpGainStateMachine::ProcessLearnMoveNoEraseMessage()
{
}

void ABattleExpGainStateMachine::ProcessLearnMoveEraseSuccessMessage()
{
}
