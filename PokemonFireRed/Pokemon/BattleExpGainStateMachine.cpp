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
	// 경험치를 모두 올린 경우
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

	// 목표 레벨에 도달한 경우
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
			// 경험치 획득
			ExpGainer->AddAccExp(DestExp - CurExp);
			State = ESubstate::End;
		}
	}
	// 목표 레벨에 도달하지 못한 경우
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
			// 레벨업 이펙트
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

		// 실제 레벨업 처리
		LevelUpData = ExpGainer->LevelUp();

		// 변경 사항 플레이어 UI에 반영
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
	// 배울 기술이 있는 경우
	if (LevelUpData.Moves.size() > 0)
	{
		// 기존 기술을 지워야 배울 수 있는 경우
		if (ExpGainer->GetMoveCount() == 4)
		{
			State = ESubstate::LearnMoveEraseMessage;
		}
		// 기존 기술을 지우지 않고도 배울 수 있는 경우
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
		// 배울 기술이 더 있을 수도 있다.
		State = ESubstate::TestLearnMove;

		// 실제 기술 학습 처리
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
