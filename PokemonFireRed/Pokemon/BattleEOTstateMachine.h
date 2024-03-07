#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ABattleEOTStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		TestTempStatus,
		TempStatusAnim,
		LeechSeed,
		Bind,
		TempStatusMessage,
		TestStatus,
		StatusMessage,
		StatusAnim,
		StatusDamage,
		End
	};

	enum class ELeechSeedState
	{
		None,
		Damage,
		Heal,
	};
public:
	// constructor destructor
	ABattleEOTStateMachine();
	~ABattleEOTStateMachine();

	// delete Function
	ABattleEOTStateMachine(const ABattleEOTStateMachine& _Other) = delete;
	ABattleEOTStateMachine(ABattleEOTStateMachine&& _Other) noexcept = delete;
	ABattleEOTStateMachine& operator=(const ABattleEOTStateMachine& _Other) = delete;
	ABattleEOTStateMachine& operator=(ABattleEOTStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Target, UBattler* _CounterTarget);

protected:
	void Tick(float _DeltaTime) override;
private:
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UBattler* Target = nullptr;
	UBattler* CounterTarget = nullptr;
	ESubstate State = ESubstate::None;

	// 고유 데이터
	float Timer = 0.0f;
	const float TempStatusAnimTime = 1.0f;
	const float StatusAnimTime = 1.0f;
	const float HpBarDecreaseTime = 1.0f;
	const float BattleMsgShowTime = 1.0f;
	ELeechSeedState LeechSeedState = ELeechSeedState::None;

	// 상태 틱 함수
	void ProcessTestTempStatus();
	void ProcessTempStatusAnim();
	void ProcessLeechSeed();
	void ProcessBind();
	void ProcessTempStatusMessage();
	void ProcessTestStatus();
	void ProcessStatusMessage();
	void ProcessStatusAnim();
	void ProcessStatusDamage();
};

