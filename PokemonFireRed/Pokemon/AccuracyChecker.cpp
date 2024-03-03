#include "AccuracyChecker.h"

UAccuracyChecker::UAccuracyChecker()
{
}

UAccuracyChecker::~UAccuracyChecker()
{
}

bool UAccuracyChecker::Check(const UBattler* _Attacker, const UBattler* _Defender)
{
	const UPokemon* AttackerPokemon = _Attacker->CurPokemonReadonly();
	const FPokemonMove* Move = _Attacker->CurMove();

	int MoveAcc = Move->Accuracy;
	float AccLHS = static_cast<float>(MoveAcc);
	AccLHS *= _Attacker->StatStage.GetAccuracyMultiplier(_Defender->StatStage.GetEvasion());

	if (AttackerPokemon->GetAbilityId() == EPokemonAbility::CompoundEyes)
	{
		AccLHS *= 1.3f;
	}

	// If DefenderPokemon->GetAbilityId() == EPokemonAbility::SandVeil && _Weather == EPokemonWeather::SandStorm, then AccLHS *= 0.8f

	int ActualAcc = UPokemonMath::Floor(AccLHS);

	int RandomNumber = UPokemonMath::RandomInt(1, 100);
	return RandomNumber <= ActualAcc;
}
