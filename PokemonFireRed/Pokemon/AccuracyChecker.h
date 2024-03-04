#pragma once
#include "Battler.h"

class UAccuracyChecker
{
public:
	// constructor destructor
	UAccuracyChecker();
	~UAccuracyChecker();

	// delete Function
	UAccuracyChecker(const UAccuracyChecker& _Other) = delete;
	UAccuracyChecker(UAccuracyChecker&& _Other) noexcept = delete;
	UAccuracyChecker& operator=(const UAccuracyChecker& _Other) = delete;
	UAccuracyChecker& operator=(UAccuracyChecker&& _Other) noexcept = delete;

	static bool Check(const UBattler* _Attacker, const UBattler* _Defender);

protected:

private:

};

