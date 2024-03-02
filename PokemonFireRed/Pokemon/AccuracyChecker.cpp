#include "AccuracyChecker.h"

UAccuracyChecker::UAccuracyChecker()
{
}

UAccuracyChecker::~UAccuracyChecker()
{
}

bool UAccuracyChecker::Check(const UPokemon* _Attacker, const UPokemon* _Defender, const UStatStage& _AttackerStatStage, const UStatStage& _DefenderStatStage, int _AttackMoveIndex)
{
	int MoveAcc = _Attacker->GetMoveAccuracy(_AttackMoveIndex);
	float AccLHS = static_cast<float>(MoveAcc);
	AccLHS *= _AttackerStatStage.GetAccuracyMultiplier(_DefenderStatStage.GetEvasion());

	if (_Attacker->GetAbilityId() == EPokemonAbility::CompoundEyes)
	{
		AccLHS *= 1.3f;
	}

	// If _Defender->GetAbilityId() == EPokemonAbility::SandVeil && _Weather == EPokemonWeather::SandStorm, then AccLHS *= 0.8f

	int ActualAcc = UPokemonMath::Floor(AccLHS);

	int RandomNumber = UPokemonMath::RandomInt(1, 100);
	return RandomNumber <= ActualAcc;
}
