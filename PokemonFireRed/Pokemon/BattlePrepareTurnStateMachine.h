#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"

class ABattlePlayerShiftStateMachine;
class ABattleEnemyShiftStateMachine;
class ABattleCanvas;
class APokemonMsgBox;

class ABattlePrepareTurnStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		SelectPokemonForce,
		PlayerShift,				// �÷��̾� ���ϸ� ������ ����
		EnemyShift,					// �� ���ϸ� ������ ����
		End,
	};
public:
	// constructor destructor
	ABattlePrepareTurnStateMachine();
	~ABattlePrepareTurnStateMachine();

	// delete Function
	ABattlePrepareTurnStateMachine(const ABattlePrepareTurnStateMachine& _Other) = delete;
	ABattlePrepareTurnStateMachine(ABattlePrepareTurnStateMachine&& _Other) noexcept = delete;
	ABattlePrepareTurnStateMachine& operator=(const ABattlePrepareTurnStateMachine& _Other) = delete;
	ABattlePrepareTurnStateMachine& operator=(ABattlePrepareTurnStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy);

	void SetBPSSM(ABattlePlayerShiftStateMachine* _BPSSM)
	{
		BattlePlayerShiftSM = _BPSSM;
	}

	void SetBESSM(ABattleEnemyShiftStateMachine* _BESSM)
	{
		BattleEnemyShiftSM = _BESSM;
	}

protected:

private:
	// ���� ƽ
	void Tick(float _DeltaTime) override;
	void ProcessSelectPokemonForce();
	void ProcessPlayerShift();
	void ProcessEnemyShift();

	// �Է� ������
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// ����
	ESubstate State = ESubstate::None;

	// SM
	ABattlePlayerShiftStateMachine* BattlePlayerShiftSM = nullptr;
	ABattleEnemyShiftStateMachine* BattleEnemyShiftSM = nullptr;
};

