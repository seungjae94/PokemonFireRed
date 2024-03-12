#pragma once
#include "BattleStateMachine.h"
#include "Battler.h"

class ABattlePlayerShiftStateMachine;
class ABattleEnemyShiftStateMachine;
class ABattleCanvas;
class APokemonMsgBox;

class ABattlePrepareTurnStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		SelectPokemonForce,
		PlayerShift,				// 플레이어 포켓몬 꺼내기 연출
		EnemyShift,					// 적 포켓몬 꺼내기 연출
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

	void Start() override;

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
	// 상태 틱
	void Tick(float _DeltaTime) override;
	void ProcessSelectPokemonForce();
	void ProcessPlayerShift();
	void ProcessEnemyShift();

	// 상태
	ESubstate State = ESubstate::None;

	// SM
	ABattlePlayerShiftStateMachine* BattlePlayerShiftSM = nullptr;
	ABattleEnemyShiftStateMachine* BattleEnemyShiftSM = nullptr;
};

