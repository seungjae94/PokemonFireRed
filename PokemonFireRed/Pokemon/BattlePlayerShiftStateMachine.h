#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ABattlePlayerShiftStateMachine : public AActor
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
		EndWait,
		End,
	};
public:
	// constructor destructor
	ABattlePlayerShiftStateMachine();
	~ABattlePlayerShiftStateMachine();

	// delete Function
	ABattlePlayerShiftStateMachine(const ABattlePlayerShiftStateMachine& _Other) = delete;
	ABattlePlayerShiftStateMachine(ABattlePlayerShiftStateMachine&& _Other) noexcept = delete;
	ABattlePlayerShiftStateMachine& operator=(const ABattlePlayerShiftStateMachine& _Other) = delete;
	ABattlePlayerShiftStateMachine& operator=(ABattlePlayerShiftStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(std::wstring_view _TakeInPokemonName, ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, const UBattler* _Player);

protected:

private:
	void Tick(float _DeltaTime) override;

	std::wstring TakeInPokemonName;
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	const UBattler* Player = nullptr;

	// 상태
	ESubstate State = ESubstate::None;

	// 고유 데이터
	float Timer = 0.0f;

	// 상수
	const float TakeInTime = 0.3f;
	const float SendOutTime = 0.3f;
	const float WaitTime = 0.75f;

	// 상태 틱 함수
	void ProcessWait1();
	void ProcessTakeIn();
	void ProcessWait2();
	void ProcessThrowBall();
	void ProcessSendOut();
	void ProcessEndWait();
};

