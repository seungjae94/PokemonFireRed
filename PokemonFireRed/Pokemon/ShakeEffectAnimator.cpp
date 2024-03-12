#include "ShakeEffectAnimator.h"

AShakeEffectAnimator::AShakeEffectAnimator()
{
}

AShakeEffectAnimator::~AShakeEffectAnimator()
{
}

void AShakeEffectAnimator::Start(bool _IsPlayer)
{
	AAnimator::Start();

	IsPlayer = _IsPlayer;
	LoopCount = MaxLoopCount;

	if (true == IsPlayer)
	{
		InitPos = &PlayerInitPos;
	}
	else
	{
		InitPos = &EnemyInitPos;
	}
	RightPos = *InitPos + UPokemonUtil::PixelVector(2, 0);
	LeftPos = *InitPos + UPokemonUtil::PixelVector(-2, 0);


	State = EState::MoveOrigin2Right;
	Timer = StateTime / 2;
}

bool AShakeEffectAnimator::IsEnd()
{
	return State == EState::End;
}

void AShakeEffectAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::None:
		break;
	case EState::MoveOrigin2Right:
		ProcessMoveOrigin2Right();
		break;
	case EState::CheckMore:
		ProcessCheckMore();
		break;
	case EState::MoveRight2Left:
		ProcessMoveRight2Left();
		break;
	case EState::MoveLeft2Right:
		ProcessMoveLeft2Right();
		break;
	case EState::FinalMoveRight2Left:
		ProcessFinalMoveRight2Left();
		break;
	case EState::MoveLeft2Origin:
		ProcessMoveLeft2Origin();
		break;
	case EState::End:
		break;
	default:
		break;
	}
}


void AShakeEffectAnimator::ProcessMoveOrigin2Right()
{
	if (true == IsPlayer)
	{
		GetPlayer()->SetRelativePosition(UPokemonMath::Lerp(RightPos, *InitPos, Timer / (StateTime / 2)));
	}
	else
	{
		GetEnemy()->SetRelativePosition(UPokemonMath::Lerp(RightPos, *InitPos, Timer / (StateTime / 2)));
	}

	if (Timer <= 0.0f)
	{
		State = EState::CheckMore;
	}
}

void AShakeEffectAnimator::ProcessCheckMore()
{
	if (LoopCount <= 0)
	{
		State = EState::FinalMoveRight2Left;
		Timer = StateTime;
		return;
	}

	State = EState::MoveRight2Left;
	Timer = StateTime;
}

void AShakeEffectAnimator::ProcessMoveRight2Left()
{
	if (true == IsPlayer)
	{
		GetPlayer()->SetRelativePosition(UPokemonMath::Lerp(LeftPos, RightPos, Timer / StateTime));
	}
	else
	{
		GetEnemy()->SetRelativePosition(UPokemonMath::Lerp(LeftPos, RightPos, Timer / StateTime));
	}

	if (Timer <= 0.0f)
	{
		State = EState::MoveLeft2Right;
		Timer = StateTime;
	}
}

void AShakeEffectAnimator::ProcessMoveLeft2Right()
{
	if (true == IsPlayer)
	{
		GetPlayer()->SetRelativePosition(UPokemonMath::Lerp(RightPos, LeftPos, Timer / StateTime));
	}
	else
	{
		GetEnemy()->SetRelativePosition(UPokemonMath::Lerp(RightPos, LeftPos, Timer / StateTime));
	}

	if (Timer <= 0.0f)
	{
		State = EState::CheckMore;
		--LoopCount;
	}
}

void AShakeEffectAnimator::ProcessFinalMoveRight2Left()
{
	if (true == IsPlayer)
	{
		GetPlayer()->SetRelativePosition(UPokemonMath::Lerp(LeftPos, RightPos, Timer / StateTime));
	}
	else
	{
		GetEnemy()->SetRelativePosition(UPokemonMath::Lerp(LeftPos, RightPos, Timer / StateTime));
	}

	if (Timer <= 0.0f)
	{
		State = EState::MoveLeft2Origin;
		Timer = StateTime / 2;
	}
}

void AShakeEffectAnimator::ProcessMoveLeft2Origin()
{
	if (true == IsPlayer)
	{
		GetPlayer()->SetRelativePosition(UPokemonMath::Lerp(*InitPos, LeftPos, Timer / (StateTime / 2)));
	}
	else
	{
		GetEnemy()->SetRelativePosition(UPokemonMath::Lerp(*InitPos, LeftPos, Timer / (StateTime / 2)));
	}

	if (Timer <= 0.0f)
	{
		State = EState::End;
	}
}
