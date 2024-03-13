#include "StatStageChangeAnimator.h"

AStatStageChangeAnimator::AStatStageChangeAnimator() 
{
}

AStatStageChangeAnimator::~AStatStageChangeAnimator() 
{
}

void AStatStageChangeAnimator::Start()
{
	AAnimator::Start();

	for (int i = 0; i < 8; ++i)
	{
		StatImages[i]->ChangeAnimation(UGameDB::GetSpeciesName(PokemonId));
		StatImages[i]->SetActive(false);
	}

	State = EState::Work;
	Timer = ImageChangeInterval;
	AlphaTimer = AlphaTime;
	
	if (true == IsStatDown)
	{
		ImageIndex = 3;
	}
	else
	{
		ImageIndex = 0;
	}

	StatImages[GetEffectiveImageIndex()]->SetActive(true);
	StatImages[GetEffectiveImageIndex()]->SetAlpha(GetAlpha());
}

bool AStatStageChangeAnimator::IsEnd()
{
	return State == EState::End;
}

void AStatStageChangeAnimator::SetIsPlayer(bool _IsPlayer)
{
	IsPlayer = _IsPlayer;
}

void AStatStageChangeAnimator::SetIsStatDown(bool _IsStatDown)
{
	IsStatDown = _IsStatDown;
}

void AStatStageChangeAnimator::SetPokemonId(EPokemonId _PokemonId)
{
	PokemonId = _PokemonId;
}

void AStatStageChangeAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	std::vector<std::string> ImageNames = { 
		RN::PokemonFrontStat0, RN::PokemonFrontStat1, RN::PokemonFrontStat2, RN::PokemonFrontStat3,
		RN::PokemonBackStat0, RN::PokemonBackStat1, RN::PokemonBackStat2, RN::PokemonBackStat3,
	};

	for (int i = 0; i < 8; ++i)
	{
		AImageElement* StatImage = nullptr; 
		
		if (i < 4)
		{
			StatImage = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::RightTop, -29, 9);
		}
		else
		{
			StatImage = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::LeftBot, 39, -48);
		}

		StatImage->SetImage(ImageNames[i]);
		StatImage->SetAlpha(0.75f);
		
		for (EPokemonId No : UGameDB::GetImplementedSpeciesNo())
		{
			int Idx = static_cast<int>(No) * 2 - 2;
			StatImage->CreateAnimation(UGameDB::GetSpeciesName(No), Idx, Idx, 1.0f / 6, false);
		}

		StatImages.push_back(StatImage);
	}
}

void AStatStageChangeAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;
	AlphaTimer -= _DeltaTime;

	switch (State)
	{
	case AStatStageChangeAnimator::EState::None:
		break;
	case AStatStageChangeAnimator::EState::CheckWork:
		ProcessCheckWork();
		break;
	case AStatStageChangeAnimator::EState::Work:
		ProcessWork();
		break;
	case AStatStageChangeAnimator::EState::End:
		break;
	default:
		break;
	}
}

void AStatStageChangeAnimator::ProcessCheckWork()
{
	if (WorkCount <= 0)
	{
		State = EState::End;

		for (int i = 0; i < 8; ++i)
		{
			StatImages[i]->SetActive(false);
		}

		return;
	}

	State = EState::Work;
	Timer = ImageChangeInterval;
}

void AStatStageChangeAnimator::ProcessWork()
{
	float Alpha = GetAlpha();
	StatImages[GetEffectiveImageIndex()]->SetAlpha(Alpha);

	if (Timer <= 0.0f)
	{
		State = EState::CheckWork;

		StatImages[GetEffectiveImageIndex()]->SetActive(false);

		if (true == IsStatDown)
		{
			--ImageIndex;
		}
		else
		{
			++ImageIndex;
		}

		ImageIndex = UPokemonMath::Mod(ImageIndex, 4);
		
		StatImages[GetEffectiveImageIndex()]->SetActive(true);
		StatImages[GetEffectiveImageIndex()]->SetAlpha(Alpha);

		--WorkCount;
	}
}

int AStatStageChangeAnimator::GetEffectiveImageIndex() const
{
	if (true == IsPlayer)
	{
		return ImageIndex + 4;
	}

	return ImageIndex;
}

float AStatStageChangeAnimator::GetAlpha() const
{
	// FadeIn 
	if (AlphaTime - AlphaTimer <= FadeTime)
	{
		return MaxAlpha * (AlphaTime - AlphaTimer) / FadeTime;
	}
	// FadeOut
	else if (AlphaTimer <= FadeTime)
	{
		return MaxAlpha * AlphaTimer / FadeTime;
	}

	return MaxAlpha;
}
