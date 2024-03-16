#include "GrowlAnimator.h"
#include "ShakeEffectAnimator.h"

AGrowlAnimator::AGrowlAnimator()
{
}

AGrowlAnimator::~AGrowlAnimator()
{
}

void AGrowlAnimator::Start()
{
	AAnimator::Start();

	State = EState::CheckMore;
	LoopCount = MaxLoopCount;

	USoundManager::PlaySE(GrowlSound);
	if (true == IsPlayer)
	{
		UpEffect = PlayerUpEffect;
		MidEffect = PlayerMidEffect;
		DownEffect = PlayerDownEffect;
		UpEffectInitPos = &PlayerUpEffectInitPos;
		MidEffectInitPos = &PlayerMidEffectInitPos;
		DownEffectInitPos = &PlayerDownEffectInitPos;
		UpEffectTargetPos = &PlayerUpEffectTargetPos;
		MidEffectTargetPos = &PlayerMidEffectTargetPos;
		DownEffectTargetPos = &PlayerDownEffectTargetPos;
	}
	else
	{
		UpEffect = EnemyUpEffect;
		MidEffect = EnemyMidEffect;
		DownEffect = EnemyDownEffect;
		UpEffectInitPos = &EnemyUpEffectInitPos;
		MidEffectInitPos = &EnemyMidEffectInitPos;
		DownEffectInitPos = &EnemyDownEffectInitPos;
		UpEffectTargetPos = &EnemyUpEffectTargetPos;
		MidEffectTargetPos = &EnemyMidEffectTargetPos;
		DownEffectTargetPos = &EnemyDownEffectTargetPos;
	}
}

bool AGrowlAnimator::IsEnd()
{
	return State == EState::End;
}

void AGrowlAnimator::SetIsPlayer(bool _IsPlayer)
{
	IsPlayer = _IsPlayer;
}

void AGrowlAnimator::SetGrowlSound(std::string_view _Sound)
{
	GrowlSound = _Sound;
}

void AGrowlAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	const int SizeX = 25;
	const int SizeY = 25;

	PlayerUpEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 85, -70);
	PlayerMidEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 85, -60);
	PlayerDownEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 85, -50);

	PlayerUpEffect->SetImage(RN::GrowlEffect);
	PlayerMidEffect->SetImage(RN::GrowlEffect);
	PlayerDownEffect->SetImage(RN::GrowlEffect);

	PlayerUpEffectInitPos = PlayerUpEffect->GetRelativePosition();
	PlayerUpEffectTargetPos = PlayerUpEffectInitPos + UPokemonUtil::PixelVector(SizeX, -SizeY);
	PlayerMidEffectInitPos = PlayerMidEffect->GetRelativePosition();
	PlayerMidEffectTargetPos = PlayerMidEffectInitPos + UPokemonUtil::PixelVector(SizeX, 0);
	PlayerDownEffectInitPos = PlayerDownEffect->GetRelativePosition();
	PlayerDownEffectTargetPos = PlayerDownEffectInitPos + UPokemonUtil::PixelVector(SizeX, SizeY);

	PlayerUpEffect->CreateAnimation("Move", 0, 1, 0.05f, true);
	PlayerMidEffect->CreateAnimation("Move", 2, 3, 0.05f, true);
	PlayerDownEffect->CreateAnimation("Move", 4, 5, 0.05f, true);

	PlayerUpEffect->SetActive(false);
	PlayerMidEffect->SetActive(false);
	PlayerDownEffect->SetActive(false);

	EnemyUpEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::RightTop, -70, 20);
	EnemyMidEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::RightTop, -70, 30);
	EnemyDownEffect = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::RightTop, -70, 40);

	EnemyUpEffect->SetImage(RN::GrowlEffect);
	EnemyMidEffect->SetImage(RN::GrowlEffect);
	EnemyDownEffect->SetImage(RN::GrowlEffect);

	EnemyUpEffectInitPos = EnemyUpEffect->GetRelativePosition();
	EnemyUpEffectTargetPos = EnemyUpEffectInitPos + UPokemonUtil::PixelVector(-SizeX, -SizeY);
	EnemyMidEffectInitPos = EnemyMidEffect->GetRelativePosition();
	EnemyMidEffectTargetPos = EnemyMidEffectInitPos + UPokemonUtil::PixelVector(-SizeX, 0);
	EnemyDownEffectInitPos = EnemyDownEffect->GetRelativePosition();
	EnemyDownEffectTargetPos = EnemyDownEffectInitPos + UPokemonUtil::PixelVector(-SizeX, SizeY);

	EnemyUpEffect->CreateAnimation("Move", 4, 5, 0.05f, true);
	EnemyMidEffect->CreateAnimation("Move", 2, 3, 0.05f, true);
	EnemyDownEffect->CreateAnimation("Move", 0, 1, 0.05f, true);

	EnemyUpEffect->SetActive(false);
	EnemyMidEffect->SetActive(false);
	EnemyDownEffect->SetActive(false);
}

void AGrowlAnimator::Tick(float _DeltaTime)
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

void AGrowlAnimator::ProcessCheckMore()
{
	if (LoopCount <= 0)
	{
		State = EState::WaitShakeEffectEnd;
		ShakeEffectAnimator->Start(!IsPlayer);
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

void AGrowlAnimator::ProcessShowGrowlEffect()
{
	UpEffect->SetRelativePosition(UPokemonMath::Lerp(*UpEffectTargetPos, *UpEffectInitPos, Timer / MoveTime));
	MidEffect->SetRelativePosition(UPokemonMath::Lerp(*MidEffectTargetPos, *MidEffectInitPos, Timer / MoveTime));
	DownEffect->SetRelativePosition(UPokemonMath::Lerp(*DownEffectTargetPos, *DownEffectInitPos, Timer / MoveTime));

	if (Timer <= 0.0f)
	{
		State = EState::CheckMore;
		--LoopCount;
		UpEffect->SetActive(false);
		MidEffect->SetActive(false);
		DownEffect->SetActive(false);
	}
}

void AGrowlAnimator::ProcessWaitShakeEffectEnd()
{
	if (true == ShakeEffectAnimator->IsEnd())
	{
		State = EState::End;
	}
}
