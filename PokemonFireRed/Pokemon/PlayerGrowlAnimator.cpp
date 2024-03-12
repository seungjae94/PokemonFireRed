#include "PlayerGrowlAnimator.h"
#include "ShakeEffectAnimator.h"

APlayerGrowlAnimator::APlayerGrowlAnimator() 
{
}

APlayerGrowlAnimator::~APlayerGrowlAnimator() 
{
}

void APlayerGrowlAnimator::Start()
{
	AAnimator::Start();

	State = EState::CheckMore;
	LoopCount = MaxLoopCount;
}

bool APlayerGrowlAnimator::IsEnd()
{
	return State == EState::End;
}

void APlayerGrowlAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	UpEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 85, -70);
	MidEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 85, -60);
	DownEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 85, -50);

	UpEffect->SetImage(RN::GrowlEffect);
	MidEffect->SetImage(RN::GrowlEffect);
	DownEffect->SetImage(RN::GrowlEffect);

	const int SizeX = 25;
	const int SizeY = 25;

	UpEffectInitPos = UpEffect->GetRelativePosition();
	UpEffectTargetPos = UpEffectInitPos + UPokemonUtil::PixelVector(SizeX, -SizeY);
	MidEffectInitPos = MidEffect->GetRelativePosition();
	MidEffectTargetPos = MidEffectInitPos + UPokemonUtil::PixelVector(SizeX, 0);
	DownEffectInitPos = DownEffect->GetRelativePosition();
	DownEffectTargetPos = DownEffectInitPos + UPokemonUtil::PixelVector(SizeX, SizeY);

	UpEffect->CreateAnimation("Move", 0, 1, 0.05f, true);
	MidEffect->CreateAnimation("Move", 2, 3, 0.05f, true);
	DownEffect->CreateAnimation("Move", 4, 5, 0.05f, true);

	UpEffect->SetActive(false);
	MidEffect->SetActive(false);
	DownEffect->SetActive(false);
}

void APlayerGrowlAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::None:
		break;
	case EState::CheckMore:
		ProcessCheckMore();
		break;
	case EState::ShowGrowlEffect:
		ProcessShowGrowlEffect();
		break;
	case EState::WaitShakeEffectEnd:
		ProcessWaitShakeEffectEnd();
		break;
	case EState::End:
		break;
	default:
		break;
	}
}

void APlayerGrowlAnimator::ProcessCheckMore()
{
	if (LoopCount <= 0)
	{
		State = EState::WaitShakeEffectEnd;
		ShakeEffectAnimator->Start(false);
		return;
	}

	State = EState::ShowGrowlEffect;
	Timer = MoveTime;

	UpEffect->SetActive(true);
	MidEffect->SetActive(true);
	DownEffect->SetActive(true);
	UpEffect->ChangeAnimation("Move", true);
	MidEffect->ChangeAnimation("Move", true);
	DownEffect->ChangeAnimation("Move", true);
}

void APlayerGrowlAnimator::ProcessShowGrowlEffect()
{

	UpEffect->SetRelativePosition(UPokemonMath::Lerp(UpEffectTargetPos, UpEffectInitPos, Timer / MoveTime));
	MidEffect->SetRelativePosition(UPokemonMath::Lerp(MidEffectTargetPos, MidEffectInitPos, Timer / MoveTime));
	DownEffect->SetRelativePosition(UPokemonMath::Lerp(DownEffectTargetPos, DownEffectInitPos, Timer / MoveTime));

	if (Timer <= 0.0f)
	{
		State = EState::CheckMore;
		--LoopCount;
		UpEffect->SetActive(false);
		MidEffect->SetActive(false);
		DownEffect->SetActive(false);
	}
}

void APlayerGrowlAnimator::ProcessWaitShakeEffectEnd()
{
	if (true == ShakeEffectAnimator->IsEnd())
	{
		State = EState::End;
	}
}
