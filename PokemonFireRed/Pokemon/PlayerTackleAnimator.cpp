#include "PlayerTackleAnimator.h"
#include "BlinkEffectAnimator.h"
#include "ShakeEffectAnimator.h"

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

	// �ʱ�ȭ
	TackleEffect->SetActive(false);
	PlayerRightPos = PlayerInitPos + UPokemonUtil::PixelVector(10, 0);
}

bool APlayerTackleAnimator::IsEnd()
{
	return State == EState::End;
}

void APlayerTackleAnimator::SetTypeVs(ETypeVs _TypeVs)
{
	TypeVs = _TypeVs;
}

void APlayerTackleAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	TackleEffect = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::RightTop, -45, 30);
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
		USoundManager::PlaySE(RN::SETackle);
	}
}

void APlayerTackleAnimator::ProcessShowTackleEffect()
{
	State = EState::PlayerMoveLeft;
	Timer = MoveTime;
	TackleEffect->SetActive(true);
	ShakeEffectAnimator->Start(false);
}

void APlayerTackleAnimator::ProcessPlayerMoveLeft()
{
	FVector Pos = UPokemonMath::Lerp(PlayerInitPos, PlayerRightPos, Timer / MoveTime);
	GetPlayer()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::HideTackleEffect;
		TackleEffect->SetActive(false);
	}
}

void APlayerTackleAnimator::ProcessHideTackleEffect()
{
	if (true == ShakeEffectAnimator->IsEnd())
	{
		State = EState::WaitBlinkEffectEnd;
		BlinkEffectAnimator->Start(false, 0.5f);

		Timer = DamageSoundWaitTime;
		IsDamageSoundPlayed = false;
	}
}

void APlayerTackleAnimator::ProcessWaitBlinkEffectEnd()
{
	if (false == IsDamageSoundPlayed && Timer <= 0.0f)
	{
		switch (TypeVs)
		{
		case ETypeVs::NotVeryEffective:
			USoundManager::PlaySE(RN::SENotVeryEffectiveDamage);
			break;
		case ETypeVs::NormallyEffective:
			USoundManager::PlaySE(RN::SENormalDamage);
			break;
		case ETypeVs::SuperEffective:
			USoundManager::PlaySE(RN::SESuperEffectiveDamage);
			break;
		default:
			break;
		}

		IsDamageSoundPlayed = true;
	}

	if (true == BlinkEffectAnimator->IsEnd())
	{
		State = EState::End;
	}
}

