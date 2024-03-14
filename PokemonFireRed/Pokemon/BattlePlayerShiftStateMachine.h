#pragma once
#include "BattleStateMachine.h"
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ABattlePlayerShiftStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		TakeInMessage1,
		TakeInMessage2,
		TakeIn,
		SendOutMessage1,
		SendOutMessage2,
		ThrowBall,
		SendOut,
		ShowPlayerBox,
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

	void Start(std::wstring_view _TakeInPokemonName);

protected:

private:
	void Start() override {};
	void Tick(float _DeltaTime) override;


	// 상태
	ESubstate State = ESubstate::None;
	std::wstring TakeInPokemonName;

	// 고유 데이터
	float Timer = 0.0f;

	// 상수
	const float TakeInTime = 0.3f;
	const float SendOutTime = 0.3f;
	const float WaitTime = 0.75f;
	const float EndWaitTime = 0.25f;
	const float ShowPlayerBoxTime = 0.5f;
	const float BattleMsgShowTime = 0.5f;

	// 상태 틱 함수
	void ProcessTakeInMessage1();
	void ProcessTakeInMessage2();
	void ProcessTakeIn();
	void ProcessSendOutMessage1();
	void ProcessSendOutMessage2();
	void ProcessThrowBall();
	void ProcessSendOut();
	void ProcessShowPlayerBox();
	void ProcessEndWait();
};

