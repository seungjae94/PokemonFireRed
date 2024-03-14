#include "BattlePokeBallStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "Battler.h"
#include "PokemonMsgBox.h"
#include "BattleCanvas.h"
#include "PlayerData.h"

ABattlePokeBallStateMachine::ABattlePokeBallStateMachine() 
{
}

ABattlePokeBallStateMachine::~ABattlePokeBallStateMachine() 
{
}

void ABattlePokeBallStateMachine::Start()
{
	ABattleStateMachine::Start();

	State = ESubstate::BallUseMessage;
	Canvas->InitCatchBall();
	MsgBox->SetMessage(L"RED used\nPOKé BALL!");
	MsgBox->Write();
}

bool ABattlePokeBallStateMachine::IsEnd() const
{
	return State == ESubstate::End;
}

void ABattlePokeBallStateMachine::Tick(float _DeltaTime)
{
	ABattleStateMachine::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::BallUseMessage:
		ProcessBallUseMessage();
		break;
	case ESubstate::PokeBallThrow:
		ProcessPokeBallThrow(_DeltaTime);
		break;
	case ESubstate::PokeBallBlocked:
		ProcessPokeBallBlocked(_DeltaTime);
		break;
	case ESubstate::PokeBallBlockedMessage1:
		ProcessPokeBallBlockedMessage1();
		break;
	case ESubstate::PokeBallBlockedMessage2:
		ProcessPokeBallBlockedMessage2();
		break;
	case ESubstate::PokeBallBlockedMessage3:
		ProcessPokeBallBlockedMessage3();
		break;
	case ESubstate::PokeBallBlockedMessage4:
		ProcessPokeBallBlockedMessage4();
		break;
	case ESubstate::PokeBallPullInPokemon:
		ProcessPokeBallPullInPokemon();
		break;
	case ESubstate::PokeBallClosing:
		ProcessPokeBallClosing();
		break;
	case ESubstate::PokeBallDrop:
		ProcessPokeBallDrop(_DeltaTime);
		break;
	case ESubstate::PokeBallCheckBounceMore:
		ProcessPokeBallCheckBounceMore();
		break;
	case ESubstate::PokeBallBounce:
		ProcessPokeBallBounce(_DeltaTime);
		break;
	case ESubstate::CalcCatch:
		ProcessCalcCatch();
		break;
	case ESubstate::CheckShakeMore:
		ProcessCheckShakeMore();
		break;
	case ESubstate::Shake:
		ProcessShake();
		break;
	case ESubstate::CatchFailAnim:
		ProcessCatchFailAnim();
		break;
	case ESubstate::CatchFailMessage1:
		ProcessCatchFailMessage1();
		break;
	case ESubstate::CatchFailMessage2:
		ProcessCatchFailMessage2();
		break;
	case ESubstate::CatchSuccessAnim:
		ProcessCatchSuccessAnim(_DeltaTime);
		break;
	case ESubstate::CatchSuccessMessage:
		ProcessCatchSuccessMessage();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}

}

void ABattlePokeBallStateMachine::ProcessBallUseMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::PokeBallThrow;

		Timer = BallThrowTime;
		Canvas->SetCatchBallActive(true);
		Canvas->HideCatchBallStars();
		BallVelocity = ThrowVelocity;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallThrow(float _DeltaTime)
{
	BallVelocity += UPokemonUtil::PixelVector(0, Gravity) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallVelocity * _DeltaTime);

	if (Timer <= 0.0f)
	{
		if (true == Enemy->IsTrainer())
		{
			State = ESubstate::PokeBallBlocked;
			Timer = BlockTime;
			return;
		}
		else
		{
			State = ESubstate::PokeBallPullInPokemon;
			Canvas->PlayCatchBallOpenAnimation();
			Timer = PullInTime;
			return;
		}
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlocked(float _DeltaTime)
{
	Canvas->AddCatchBallPosition(UPokemonUtil::PixelVector(-300, 400) * _DeltaTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallBlockedMessage1;
		MsgBox->SetMessage(L"The TRAINER blocked the BALL!");
		MsgBox->Write();
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlockedMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::PokeBallBlockedMessage2;
		Timer = BlockMessageShowTime;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlockedMessage2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallBlockedMessage3;
		MsgBox->SetMessage(L"Don't be a thief!");
		MsgBox->Write();
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlockedMessage3()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::PokeBallBlockedMessage4;
		Timer = BlockMessageShowTime;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlockedMessage4()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
		MsgBox->SetMessage(L"");
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallPullInPokemon()
{
	Canvas->LerpCatchPullInEnemyPokemon(Timer / PullInTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallClosing;
		Timer = ClosingTime;
		Canvas->PlayCatchBallCloseAnimation();
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallClosing()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallDrop;
		BallVelocity = FVector::Zero;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallDrop(float _DeltaTime)
{
	BallVelocity += UPokemonUtil::PixelVector(0, Gravity) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallVelocity * _DeltaTime);

	// 땅 높이와 비슷해진 경우
	FVector BallPos = Canvas->GetCatchBallPosition();
	if (BallPos.Y >= BallGroundY)
	{
		State = ESubstate::PokeBallCheckBounceMore;
		BounceCount = MaxBounceCount;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallCheckBounceMore()
{
	// 다 튕긴 경우
	if (BounceCount <= 0)
	{
		State = ESubstate::CalcCatch;
		return;
	}

	State = ESubstate::PokeBallBounce;
	BallVelocity = -BallVelocity * Elasticity;
	Timer = BounceWaitTime;
}

void ABattlePokeBallStateMachine::ProcessPokeBallBounce(float _DeltaTime)
{
	BallVelocity += UPokemonUtil::PixelVector(0, Gravity) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallVelocity * _DeltaTime);

	// 일정 시간이 지난 뒤(튕기기 시작하는 시점에는 땅 높이와 비슷하기 때문) 땅 높이와 비슷해진 경우
	FVector BallPos = Canvas->GetCatchBallPosition();
	if (Timer <= 0.0f &&  BallPos.Y >= BallGroundY)
	{
		State = ESubstate::PokeBallCheckBounceMore;
		--BounceCount;
		Timer = CalcCatchWaitTime;
	}
}

void ABattlePokeBallStateMachine::ProcessCalcCatch()
{
	if (Timer > 0.0f)
	{
		return;
	}

	const UPokemon* EnemyPokemon = Enemy->CurPokemonReadonly();
	EffectiveCatchRate = (3 * EnemyPokemon->GetHp() - 2 * EnemyPokemon->GetCurHp()) * EnemyPokemon->GetCatchRate() / (3 * EnemyPokemon->GetHp()) * Global::CatchRateBonusCoeff;

	EffectiveCatchRate = UPokemonMath::Max(EffectiveCatchRate, 1);

	if (EffectiveCatchRate >= 255)
	{
		// 포획 성공
		State = ESubstate::CheckShakeMore;
		TestSuccessCount = MaxTestSuccessCount;
		ShakeCount = MaxTestSuccessCount - 1;
		return;
	}
	
	// 흔들림 공식 계산
	int Supremum = UPokemonMath::Round(65535 * std::powf(EffectiveCatchRate / 255.0f, 0.25f));
	TestSuccessCount = 0;

	for (int i = 0; i < 4; ++i)
	{
		int RandomInt = UPokemonMath::RandomInt(0, 65535);

		if (RandomInt <= Supremum)
		{
			++TestSuccessCount;
		}
	}

	State = ESubstate::CheckShakeMore;
	ShakeCount = UPokemonMath::Min(TestSuccessCount, 3);
}

void ABattlePokeBallStateMachine::ProcessCheckShakeMore()
{
	if (ShakeCount <= 0)
	{
		// 포획 성공
		if (TestSuccessCount == MaxTestSuccessCount)
		{
			State = ESubstate::CatchSuccessAnim;
			Canvas->ShowCatchBallStars();
			Timer = CatchSuccessAnimTime;
		}
		// 포획 실패
		else
		{
			State = ESubstate::CatchFailAnim;
			Canvas->PlayCatchBallOpenAnimation();
			Timer = CatchFailAnimTime;
		}
		return;
	}

	State = ESubstate::Shake;

	if (ShakeCount % 2 == 0)
	{
		Canvas->PlayCatchBallShakeLeftAnimation();
	}
	else
	{
		Canvas->PlayCatchBallShakeRightAnimation();
	}

	Timer = ShakeTime;
}

void ABattlePokeBallStateMachine::ProcessShake()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::CheckShakeMore;
		--ShakeCount;
	}
}

void ABattlePokeBallStateMachine::ProcessCatchFailAnim()
{
	Canvas->LerpCatchFailEnemyPokemon(Timer / CatchFailAnimTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::CatchFailMessage1;
		Canvas->SetCatchBallActive(false);
		Player->SetCatchResult(false);

		if (0 == TestSuccessCount)
		{
			MsgBox->SetMessage(L"Oh no!\nThe POKéMON broke free!");
		}
		else if (1 == TestSuccessCount)
		{
			MsgBox->SetMessage(L"Aww!\nIt appeared to be caught!");
		}
		else if (2 == TestSuccessCount)
		{
			MsgBox->SetMessage(L"Aargh!\nAlmost had it!");
		}
		else if (3 == TestSuccessCount)
		{
			MsgBox->SetMessage(L"Shoot!\nIt was so close, too!");
		}
		else
		{
			MsgBoxAssert("볼 흔들림 횟수가 0 이상 3 이하가 아닌데 포획 실패로 처리되었습니다.");
			return;
		}

		MsgBox->Write();
	}
}

void ABattlePokeBallStateMachine::ProcessCatchFailMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::CatchFailMessage2;
		Timer = CatchFailMessageShowTime;
	}
}

void ABattlePokeBallStateMachine::ProcessCatchFailMessage2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattlePokeBallStateMachine::ProcessCatchSuccessAnim(float _DeltaTime)
{
	// (0, 1), (0.5, 0.5), (1, 1)을 지나는 2차 함수 
	Canvas->SetCatchBallAlpha(2.0f * (Timer - 0.5f) * (Timer - 0.5f) + 0.5f);
	Canvas->AddCatchBallStarPos(0, UPokemonUtil::PixelVector(-20, 10) * _DeltaTime);
	Canvas->AddCatchBallStarPos(1, UPokemonUtil::PixelVector(6, 20) * _DeltaTime);
	Canvas->AddCatchBallStarPos(2, UPokemonUtil::PixelVector(20, 10) * _DeltaTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::CatchSuccessMessage;
		Canvas->HideCatchBallStars();
		Player->SetCatchResult(true);
		MsgBox->SetMessage(L"Gotcha!\n" + Enemy->CurPokemon()->GetNameW() + L" was caught!");
		MsgBox->Write();
	}
}

void ABattlePokeBallStateMachine::ProcessCatchSuccessMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState() && true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::End;

		// 실제 포켓몬 획득
		UPlayerData::AddPokemonToEntry(*Enemy->CurPokemon());
	}
}

