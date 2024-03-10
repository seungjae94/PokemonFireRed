#include "BattleExpGainStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

ABattleExpGainStateMachine::ABattleExpGainStateMachine()
{
}

ABattleExpGainStateMachine::~ABattleExpGainStateMachine()
{
}

void ABattleExpGainStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UPokemon* _ExpGainer, int _Exp, bool _IsCurPokemon)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	ExpGainer = _ExpGainer;
	Exp = UPokemonMath::Floor(_Exp * Global::CheatExpBonusCoeff);
	IsCurPokemon = _IsCurPokemon;

	State = ESubstate::ExpGainMessage1;
	std::wstring BattleMsg = ExpGainer->GetNameW() + L" gained\n";
	BattleMsg += std::to_wstring(Exp);
	BattleMsg += L" Exp. Points!";
	MsgBox->SetMessage(BattleMsg);
	MsgBox->Write();
}

void ABattleExpGainStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleExpGainStateMachine::ESubstate::None:
		break;
	case ABattleExpGainStateMachine::ESubstate::ExpGainMessage1:
		ProcessExpGainMessage1();
		break;
	case ABattleExpGainStateMachine::ESubstate::ExpGainMessage2:
		ProcessExpGainMessage2();
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
	case ABattleExpGainStateMachine::ESubstate::LevelUpMessage1:
		ProcessLevelUpMessage1();
		break;
	case ABattleExpGainStateMachine::ESubstate::LevelUpMessage2:
		ProcessLevelUpMessage2();
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

void ABattleExpGainStateMachine::ProcessExpGainMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::ExpGainMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void ABattleExpGainStateMachine::ProcessExpGainMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		SimResult = UExpCalculator::SimExpGain(ExpGainer, Exp);
		State = ESubstate::TestExpBarIncrease;
		MsgBox->HideSkipArrow();
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

		if (true == IsCurPokemon)
		{
			Canvas->LerpExpBar(CurExp, DestExp, ExpSize, Timer / ExpBarIncTime);
		}
		else
		{
			Timer = -1.0f;
		}

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

		if (true == IsCurPokemon)
		{
			Canvas->LerpExpBar(CurExp, DestExp, ExpSize, Timer / ExpBarIncTime);
		}
		else
		{
			Timer = -1.0f;
		}

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
		State = ESubstate::LevelUpMessage1;

		std::wstring BattleMsg = ExpGainer->GetNameW();
		BattleMsg += L" grew to\nLV. ";
		BattleMsg += std::to_wstring(ExpGainer->GetLevel() + 1);
		BattleMsg += L"!";
		MsgBox->SetMessage(BattleMsg);
		MsgBox->Write();

		// ���� ������ ó��
		LevelUpData = ExpGainer->LevelUp();

		// ���� ���� �÷��̾� UI�� �ݿ�
		Canvas->RefreshPlayerPokemonBox();
	}
}

void ABattleExpGainStateMachine::ProcessLevelUpMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::LevelUpMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void ABattleExpGainStateMachine::ProcessLevelUpMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::StatUpDiffWindow;
		Canvas->ShowStatUpBox(LevelUpData);
		MsgBox->HideSkipArrow();
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
			MsgBox->SetMessage(BattleMsg);
			MsgBox->Write();
		}
	}
	else
	{
		State = ESubstate::TestExpBarIncrease;
	}
}

void ABattleExpGainStateMachine::ProcessLearnMoveSuccessMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState() && true == UEngineInput::IsDown('Z'))
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
