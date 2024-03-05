#include "ExpGainCalculator.h"

UExpGainCalculator::UExpGainCalculator() 
{
}

UExpGainCalculator::~UExpGainCalculator() 
{
}

std::map<const UPokemon*, int> UExpGainCalculator::CalcExpGain(const UBattler* Fainter)
{
    const UPokemon* FainterPokemon = Fainter->CurPokemonReadonly();
    float TotalExpGain = FainterPokemon->GetYieldExp() * FainterPokemon->GetLevel() / 7.0f;
 
    const std::list<UPokemon*>& Participants = Fainter->GetParticipantsReadonly();

    std::map<const UPokemon*, int> Result;
    
    for (const UPokemon* Participant : Participants)
    {
        if (Participant->GetStatusId() == EPokemonStatus::Faint)
        {
            continue;
        }
        Result.insert(std::pair<const UPokemon*, int>(Participant, 0));
    }

    TotalExpGain /= Result.size();

    // 적이 트레이너인 경우 1.5배
    if (false == Fainter->IsPlayer() && false == Fainter->IsWildPokemon())
    {
        TotalExpGain *= 1.5f;
    }

    int TotalExpGainInt = UPokemonMath::Floor(TotalExpGain);

    for (std::pair<const UPokemon* const, int>& Pair : Result)
    {
        Pair.second = TotalExpGainInt;
    }

    return Result;
}

