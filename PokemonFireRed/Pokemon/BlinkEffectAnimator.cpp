#include "BlinkEffectAnimator.h"

ABlinkEffectAnimator::ABlinkEffectAnimator() 
{
}

ABlinkEffectAnimator::~ABlinkEffectAnimator() 
{
}

void ABlinkEffectAnimator::Start(bool _IsPlayer, float _WaitTime)
{
	AAnimator::Start();

	IsPlayer = _IsPlayer;
	LoopCount = MaxLoopCount;
	WaitTime = _WaitTime;

	State = EState::Wait;
	Timer = WaitTime;
}

bool ABlinkEffectAnimator::IsEnd()
{
	return State == EState::End;
}

void ABlinkEffectAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::None:
		break;
	case EState::Wait:
		ProcessWait();
		break;
	case EState::CheckMore:
		ProcessCheckMore();
		break;
	case EState::Hide:
		ProcessHide();
		break;
	case EState::Show:
		ProcessShow();
		break;
	case EState::End:
		break;
	default:
		break;
	}
}

void ABlinkEffectAnimator::ProcessWait()
{
	if (Timer <= 0.0f)
	{
		State = EState::CheckMore;
	}
}

void ABlinkEffectAnimator::ProcessCheckMore()
{
	if (LoopCount <= 0)
	{
		State = EState::End;
		return;
	}

	State = EState::Hide;
	Timer = StateTime;
	
	if (true == IsPlayer)
	{
		GetPlayer()->SetActive(false);
	}
	else
	{
		GetEnemy()->SetActive(false);
	}
}

void ABlinkEffectAnimator::ProcessHide()
{
	if (Timer <= 0.0f)
	{
		State = EState::Show;
		Timer = StateTime;

		if (true == IsPlayer)
		{
			GetPlayer()->SetActive(true);
		}
		else
		{
			GetEnemy()->SetActive(true);
		}
	}

}

void ABlinkEffectAnimator::ProcessShow()
{
	if (Timer <= 0.0f)
	{
		State = EState::CheckMore;
		--LoopCount;
	}
}

