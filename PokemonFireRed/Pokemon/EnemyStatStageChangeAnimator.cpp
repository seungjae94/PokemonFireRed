#include "EnemyStatStageChangeAnimator.h"

AEnemyStatStageChangeAnimator::AEnemyStatStageChangeAnimator() 
{
}

AEnemyStatStageChangeAnimator::~AEnemyStatStageChangeAnimator() 
{
}

void AEnemyStatStageChangeAnimator::Start()
{
	AAnimator::Start();

	for (int i = 0; i < 4; ++i)
	{
		StatImages[i]->ChangeAnimation(Global::PokemonFrontPrefix + UGameDB::GetSpeciesName(PokemonId));
		StatImages[i]->SetActive(false);
	}

	State = EState::Work;
	Timer = ImageChangeInterval;
	
	if (true == IsStatDown)
	{
		ImageIndex = 3;
	}
	else
	{
		ImageIndex = 0;
	}

	StatImages[ImageIndex]->SetActive(true);
}

bool AEnemyStatStageChangeAnimator::IsEnd()
{
	return State == EState::End;
}

void AEnemyStatStageChangeAnimator::SetIsStatDown(bool _IsStatDown)
{
	IsStatDown = _IsStatDown;
}

void AEnemyStatStageChangeAnimator::SetPokemonId(EPokemonId _PokemonId)
{
	PokemonId = _PokemonId;
}

void AEnemyStatStageChangeAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	std::vector<std::string> ImageNames = { RN::PokemonFrontStat0, RN::PokemonFrontStat1, RN::PokemonFrontStat2, RN::PokemonFrontStat3 };

	for (int i = 0; i < 4; ++i)
	{
		AImageElement* StatImage = CreateImageElement(this, ERenderingOrder::UI3, EPivotType::RightTop, -29, 9);

		StatImage->SetImage(ImageNames[i]);
		StatImage->SetAlpha(0.75f);
		
		for (EPokemonId No : UGameDB::GetImplementedSpeciesNo())
		{
			int Idx = static_cast<int>(No) * 2 - 2;
			StatImage->CreateAnimation(
				Global::PokemonFrontPrefix + UGameDB::GetSpeciesName(No),
				Idx, Idx, 1.0f / 6, false);
		}

		StatImages.push_back(StatImage);
	}
}

void AEnemyStatStageChangeAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case AEnemyStatStageChangeAnimator::EState::None:
		break;
	case AEnemyStatStageChangeAnimator::EState::CheckWork:
		ProcessCheckWork();
		break;
	case AEnemyStatStageChangeAnimator::EState::Work:
		ProcessWork();
		break;
	case AEnemyStatStageChangeAnimator::EState::End:
		break;
	default:
		break;
	}
}

void AEnemyStatStageChangeAnimator::ProcessCheckWork()
{
	if (WorkCount <= 0)
	{
		State = EState::End;

		for (int i = 0; i < 4; ++i)
		{
			StatImages[i]->SetActive(false);
		}

		return;
	}

	State = EState::Work;
	Timer = ImageChangeInterval;
}

void AEnemyStatStageChangeAnimator::ProcessWork()
{
	if (Timer <= 0.0f)
	{
		State = EState::CheckWork;

		StatImages[ImageIndex]->SetActive(false);

		if (true == IsStatDown)
		{
			--ImageIndex;
		}
		else
		{
			++ImageIndex;
		}

		ImageIndex = UPokemonMath::Mod(ImageIndex, 4);
		
		StatImages[ImageIndex]->SetActive(true);

		--WorkCount;
	}

}

