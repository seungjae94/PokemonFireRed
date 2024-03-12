#include "PlayerTackleAnimator.h"

APlayerTackleAnimator::APlayerTackleAnimator() 
{
}

APlayerTackleAnimator::~APlayerTackleAnimator() 
{
}

void APlayerTackleAnimator::Start()
{
	AAnimator::Start();

	State = EState::PlayerMoveRight;
	Timer = MoveTime;

	// ÃÊ±âÈ­
	TackleEffect->SetActive(false);
	PlayerRightPos = PlayerInitPos + UPokemonUtil::PixelVector(10, 0);
}

bool APlayerTackleAnimator::IsEnd()
{
	return State == EState::End;
}

void APlayerTackleAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	TackleEffect = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::RightTop, 30, 15);
	TackleEffect->SetImage(RN::TackleEffect);
}

void APlayerTackleAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::None:
		break;
	case EState::PlayerMoveRight:
		ProcessPlayerMoveRight();
		break;
	case EState::ShowTackleEffect:
		ProcessShowTackleEffect();
		break;
	case EState::PlayerMoveLeft:
		ProcessPlayerMoveLeft();
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

void APlayerTackleAnimator::ProcessPlayerMoveRight()
{
	FVector Pos = UPokemonMath::Lerp(PlayerRightPos, PlayerInitPos, Timer / MoveTime);
	GetPlayer()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::ShowTackleEffect;
	}
}

void APlayerTackleAnimator::ProcessShowTackleEffect()
{
	State = EState::PlayerMoveLeft;
	Timer = MoveTime;
	TackleEffect->SetActive(true);

	// EnemyShakeEffectAnimator->Start();
}

void APlayerTackleAnimator::ProcessPlayerMoveLeft()
{
	FVector Pos = UPokemonMath::Lerp(PlayerInitPos, PlayerRightPos, Timer / MoveTime);
	GetPlayer()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::HideTackleEffect;
	}
}

void APlayerTackleAnimator::ProcessHideTackleEffect()
{
	State = EState::WaitBlinkEffectEnd;
	TackleEffect->SetActive(false);

	// EnemyBlinkEffectAnimator->Start();
}

void APlayerTackleAnimator::ProcessWaitBlinkEffectEnd()
{
	//if (true == EnemyBlinkEffectAnimator()->IsEnd())
	//{
		State = EState::End;
	//}
}

