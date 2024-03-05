#pragma once
#include <EngineCore/Actor.h>
#include "BattleCanvas.h"
#include "Battler.h"

class ABattleEOTStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		TestTempStatus,
		TempStatusAnim,
		TempStatusDamage,
		TempStatusMessage,
		TestStatus,
		StatusMessage,
		StatusAnim,
		StatusDamage,
		End
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

	void Start(ABattleCanvas* _Canvas, UBattler* _Target);

protected:
	void Tick(float _DeltaTime) override;
private:
	ABattleCanvas* Canvas = nullptr;
	UBattler* Target = nullptr;
	ESubstate State = ESubstate::None;

	float Timer = 0.0f;
	const float TempStatusAnimTime = 1.0f;
	const float StatusAnimTime = 1.0f;
	const float HpBarDecreaseTime = 0.5f;
	const float BattleMsgShowTime = 1.0f;
	int PrevHp = 0;
	int NextHp = 0;

	void ProcessTestTempStatus();
	void ProcessTempStatusAnim();
	void ProcessTempStatusDamage();
	void ProcessTempStatusMessage();
	void ProcessTestStatus();
	void ProcessStatusMessage();
	void ProcessStatusAnim();
	void ProcessStatusDamage();
};

