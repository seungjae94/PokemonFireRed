#pragma once
#include <map>
#include "Pokemon.h"
#include "Battler.h"

class UExpGainCalculator
{
public:
	// constructor destructor
	UExpGainCalculator();
	~UExpGainCalculator();

	// delete Function
	UExpGainCalculator(const UExpGainCalculator& _Other) = delete;
	UExpGainCalculator(UExpGainCalculator&& _Other) noexcept = delete;
	UExpGainCalculator& operator=(const UExpGainCalculator& _Other) = delete;
	UExpGainCalculator& operator=(UExpGainCalculator&& _Other) noexcept = delete;

	static std::map<const UPokemon*, int> CalcExpGain(const UBattler* Fainter);

protected:

private:

};

