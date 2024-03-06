#pragma once
#include <EngineCore/Actor.h>
#include "BattleCanvas.h"
#include "Battler.h"

class ABattleShiftStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		Wait1,
		TakeIn,
		Wait2,
		ThrowBall,
		SendOut,
		End,
	};
public:
	// constructor destructor
	ABattleShiftStateMachine();
	~ABattleShiftStateMachine();

	// delete Function
	ABattleShiftStateMachine(const ABattleShiftStateMachine& _Other) = delete;
	ABattleShiftStateMachine(ABattleShiftStateMachine&& _Other) noexcept = delete;
	ABattleShiftStateMachine& operator=(const ABattleShiftStateMachine& _Other) = delete;
	ABattleShiftStateMachine& operator=(ABattleShiftStateMachine&& _Other) noexcept = delete;

	void Start(std::wstring_view _TakeInPokemonName, ABattleCanvas* _Canvas, const UBattler* _Player);

protected:

private:
	void Tick(float _DeltaTime) override;

	std::wstring TakeInPokemonName;
	ABattleCanvas* Canvas = nullptr;
	const UBattler* Player = nullptr;

	// 상태
	ESubstate State = ESubstate::None;

	// 고유 데이터
	float Timer = 0.0f;

	// 상수
	const float TakeInTime = 0.3f;
	const float SendOutTime = 0.3f;
	const float WaitTime = 0.5f;

	// 상태 틱 함수
	void ProcessWait1();
	void ProcessTakeIn();
	void ProcessWait2();
	void ProcessThrowBall();
	void ProcessSendOut();
};

