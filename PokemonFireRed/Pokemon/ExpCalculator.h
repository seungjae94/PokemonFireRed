#pragma once
#include <map>
#include "Pokemon.h"
#include "Battler.h"

struct FSimExpGainResult
{
public:
	int NextLevel = 0;
	int NextExp = 0;    // 경험치 획득 후 레벨 기준
	int NextAccExp = 0;
};

class UExpCalculator
{
public:
	// constructor destructor
	UExpCalculator();
	~UExpCalculator();

	// delete Function
	UExpCalculator(const UExpCalculator& _Other) = delete;
	UExpCalculator(UExpCalculator&& _Other) noexcept = delete;
	UExpCalculator& operator=(const UExpCalculator& _Other) = delete;
	UExpCalculator& operator=(UExpCalculator&& _Other) noexcept = delete;

	static std::map<UPokemon*, int> SimFaint(const UBattler* _Fainter);

	static const FSimExpGainResult SimExpGain(const UPokemon* _Pokemon, int _Exp);

	static int GetNeedAccExp(const UPokemon* _Pokemon, int _Level);
protected:
private:
	static int GetNeedErraticAccExp(int _Level);
	static int GetNeedFluctuatingAccExp(int _Level);
};

