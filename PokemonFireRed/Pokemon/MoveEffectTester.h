#pragma once
#include "Battler.h"

enum class EMoveEffectTestFailureReason
{
	None,
	StatStageCap,
	StatusOverlap,
	RandomTest,
};

struct FMoveEffectTestResult
{
public:
	bool Success = false;
	EMoveEffectTestFailureReason Reason = EMoveEffectTestFailureReason::None;
	std::wstring Message;
};

class UMoveEffectTester
{
public:
	// constructor destructor
	UMoveEffectTester();
	~UMoveEffectTester();

	// delete Function
	UMoveEffectTester(const UMoveEffectTester& _Other) = delete;
	UMoveEffectTester(UMoveEffectTester&& _Other) noexcept = delete;
	UMoveEffectTester& operator=(const UMoveEffectTester& _Other) = delete;
	UMoveEffectTester& operator=(UMoveEffectTester&& _Other) noexcept = delete;

	static FMoveEffectTestResult TestBE(const UBattler* _Attacker, const UBattler* _Defender);
	static FMoveEffectTestResult TestSE(const UBattler* _Attacker, const UBattler* _Defender);

protected:

private:
	static FMoveEffectTestResult Test(const UBattler* _Attacker, const UBattler* _Target, EStatStageChangeType _StatStageId,  int _StatStageValue, EPokemonStatus _StatusId, int _SuccessRate, EMoveEffectTarget _TargetType);
	static std::wstring GenerateStatStageFailMessage(const UBattler* _Target, EStatStageChangeType _StatStageId, int _StatStageValue);
	static std::wstring GenerateStatStageSuccessMessage(const UBattler* _Target, EStatStageChangeType _StatStageId, int _StatStageValue);
	static std::wstring GenerateStatusSuccessMessage(const UBattler* _Target, EPokemonStatus _StatusId);
	static std::wstring GenerateStatusFailMessage(const UBattler* _Attacker);
};

