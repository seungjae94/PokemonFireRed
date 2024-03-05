#include "ExpCalculator.h"

UExpCalculator::UExpCalculator() 
{
}

UExpCalculator::~UExpCalculator() 
{
}

std::map<UPokemon*, int> UExpCalculator::SimFaint(const UBattler* _Fainter)
{
    const UPokemon* FainterPokemon = _Fainter->CurPokemonReadonly();
    float TotalExpGain = FainterPokemon->GetYieldExp() * FainterPokemon->GetLevel() / 7.0f;
 
    const std::list<UPokemon*>& Participants = _Fainter->GetParticipantsReadonly();

    std::map<UPokemon*, int> Result;
    
    for (UPokemon* Participant : Participants)
    {
        if (Participant->GetStatusId() == EPokemonStatus::Faint)
        {
            continue;
        }

        Result.insert(std::pair<UPokemon*, int>(Participant, 0));
    }

    TotalExpGain /= Result.size();

    // 적이 트레이너인 경우 1.5배
    if (false == _Fainter->IsPlayer() && false == _Fainter->IsWildPokemon())
    {
        TotalExpGain *= 1.5f;
    }

    int TotalExpGainInt = UPokemonMath::Floor(TotalExpGain);

    for (std::pair<UPokemon* const, int>& Pair : Result)
    {
        Pair.second = TotalExpGainInt;
    }

    return Result;
}

const FSimExpGainResult UExpCalculator::SimExpGain(const UPokemon* _Pokemon, int _Exp)
{
	int PrevLevel = _Pokemon->GetLevel();
    int PrevAccExp = _Pokemon->GetAccExp();

	int NextAccExp = PrevAccExp + _Exp;
	int Level = PrevLevel;
	while (true)
	{
		int NeedAccExp = GetNeedAccExp(_Pokemon, Level + 1);

		if (NextAccExp >= NeedAccExp)
		{
			++Level;
		}
		else
		{
			break;
		}
	}

	FSimExpGainResult Result;
	Result.NextLevel = Level;
	Result.NextExp = NextAccExp - GetNeedAccExp(_Pokemon, Level);
	Result.NextAccExp = NextAccExp;
    return Result;
}

int UExpCalculator::GetNeedAccExp(const UPokemon* _Pokemon, int _Level)
{
	if (_Level == 1)
	{
		return 0;
	}

	double Result = 0;

	switch (_Pokemon->GetExpGroup())
	{
	case EExperienceGroup::Erratic:
		return GetNeedErraticAccExp(_Level);
	case EExperienceGroup::Fast:
		Result = 0.8 * std::pow(_Level, 3);
		break;
	case EExperienceGroup::MediumFast:
		Result = std::pow(_Level, 3);
		break;
	case EExperienceGroup::MediumSlow:
		Result = 1.2 * std::pow(_Level, 3) - 15.0 * std::pow(_Level, 2) + 100 * _Level - 140;
		break;
	case EExperienceGroup::Slow:
		Result = 1.25 * std::pow(_Level, 3);
		break;
	case EExperienceGroup::Fluctuating:
		return GetNeedFluctuatingAccExp(_Level);
	default:
		break;
	}

	return UPokemonMath::Floor(Result);
}

int UExpCalculator::GetNeedErraticAccExp(int _Level)
{
	if (_Level < 50)
	{
		return UPokemonMath::Pow(_Level, 3) * (100 - _Level) / 50;
	}
	else if (_Level < 68)
	{
		return UPokemonMath::Pow(_Level, 3) * (150 - _Level) / 100;
	}
	else if (_Level < 98)
	{
		return UPokemonMath::Pow(_Level, 3) * ((1911 - 10 * _Level) / 3) / 500;
	}

	return UPokemonMath::Pow(_Level, 3) * (160 - _Level) / 100;
}

int UExpCalculator::GetNeedFluctuatingAccExp(int _Level)
{
	if (_Level < 15)
	{
		return UPokemonMath::Pow(_Level, 3) * ((_Level + 1) / 3 + 24) / 50;
	}
	else if (_Level < 36)
	{
		return UPokemonMath::Pow(_Level, 3) * (_Level + 14) / 50;
	}

	return UPokemonMath::Pow(_Level, 3) * ((_Level / 2) + 32) / 50;
}
