#include "EnemyTackleAnimator.h"

AEnemyTackleAnimator::AEnemyTackleAnimator() 
{
}

AEnemyTackleAnimator::~AEnemyTackleAnimator() 
{
}

void AEnemyTackleAnimator::Start()
{
	AAnimator::Start();

	State = EState::EnemyMoveLeft;
	Timer = MoveTime;

	// ÃÊ±âÈ­
	TackleEffect->SetActive(false);
	EnemyLeftPos = EnemyInitPos + UPokemonUtil::PixelVector(-10, 0);
}

bool AEnemyTackleAnimator::IsEnd()
{
	return State == EState::End;
}

void AEnemyTackleAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	TackleEffect = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::LeftBot, 55, -50);
	TackleEffect->SetImage(RN::TackleEffect);
}

void AEnemyTackleAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::None:
		break;
	case EState::EnemyMoveLeft:
		ProcessEnemyMoveLeft();
		break;
	case EState::ShowTackleEffect:
		ProcessShowTackleEffect();
		break;
	case EState::EnemyMoveRight:
		ProcessEnemyMoveRight();
		break;
	case EState::HideTackleEffect:
		ProcessHideTackleEffect();
		break;
	case EState::WaitBlinkEffectEnd:
		ProcessWaitBlinkEffectEnd();
		break;
	case EState::End:
		break;
	default:
		break;
	}
}

void AEnemyTackleAnimator::ProcessEnemyMoveLeft()
{
	FVector Pos = UPokemonMath::Lerp(EnemyLeftPos, EnemyInitPos, Timer / MoveTime);
	GetEnemy()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::ShowTackleEffect;
	}
}

void AEnemyTackleAnimator::ProcessShowTackleEffect()
{
	State = EState::EnemyMoveRight;
	Timer = MoveTime;
	TackleEffect->SetActive(true);

	// PlayerShakeEffectAnimator->Start();
}

void AEnemyTackleAnimator::ProcessEnemyMoveRight()
{
	FVector Pos = UPokemonMath::Lerp(EnemyInitPos, EnemyLeftPos, Timer / MoveTime);
	GetEnemy()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::HideTackleEffect;
	}
}

void AEnemyTackleAnimator::ProcessHideTackleEffect()
{
	State = EState::WaitBlinkEffectEnd;
	TackleEffect->SetActive(false);

	// PlayerBlinkEffectAnimator->Start();
}

void AEnemyTackleAnimator::ProcessWaitBlinkEffectEnd()
{
	//if (true == PlayerBlinkEffectAnimator()->IsEnd())
//{
	State = EState::End;
	//}
}
