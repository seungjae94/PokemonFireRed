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

	// ����
	ESubstate State = ESubstate::None;

	// ���� ������
	float Timer = 0.0f;

	// ���
	const float TakeInTime = 0.3f;
	const float SendOutTime = 0.3f;
	const float WaitTime = 0.5f;

	// ���� ƽ �Լ�
	void ProcessWait1();
	void ProcessTakeIn();
	void ProcessWait2();
	void ProcessThrowBall();
	void ProcessSendOut();
};

