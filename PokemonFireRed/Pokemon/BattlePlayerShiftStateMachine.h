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
		Wait1,
		TakeIn,
		Wait2,
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


	// ����
	ESubstate State = ESubstate::None;
	std::wstring TakeInPokemonName;

	// ���� ������
	float Timer = 0.0f;

	// ���
	const float TakeInTime = 0.3f;
	const float SendOutTime = 0.3f;
	const float WaitTime = 0.75f;
	const float ShowPlayerBoxTime = 0.5f;

	// ���� ƽ �Լ�
	void ProcessWait1();
	void ProcessTakeIn();
	void ProcessWait2();
	void ProcessThrowBall();
	void ProcessSendOut();
	void ProcessShowPlayerBox();
	void ProcessEndWait();
};

