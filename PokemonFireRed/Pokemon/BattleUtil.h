#pragma once
#include <string>
#include "Battler.h"

class UBattleUtil
{
public:
	// constructor destructor
	UBattleUtil();
	~UBattleUtil();

	// delete Function
	UBattleUtil(const UBattleUtil& _Other) = delete;
	UBattleUtil(UBattleUtil&& _Other) noexcept = delete;
	UBattleUtil& operator=(const UBattleUtil& _Other) = delete;
	UBattleUtil& operator=(UBattleUtil&& _Other) noexcept = delete;

	static std::wstring GetPokemonFullName(const UBattler* _Battler);
	static std::wstring GetStatStageNameW(EStatStageChangeType _StatStageId);
	static std::wstring GetStatStageChangeMessageSuffix(int _Value);

protected:

private:

};

