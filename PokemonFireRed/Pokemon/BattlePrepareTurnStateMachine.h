#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"

class ABattlePlayerShiftStateMachine;
class ABattleCanvas;
class APokemonMsgBox;

class ABattlePrepareTurnStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		SelectPokemonForce,
		EnemyAboutToUseMessage1,	// BUG CATCHER RICK is about to use CATERPIE.
		EnemyAboutToUseMessage2,	// Will RED change POKeMON? (Yes, No)	
		SelectPokemonOptional,
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

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy);

	void SetBPSSM(ABattlePlayerShiftStateMachine* _BPSSM)
	{
		BattlePlayerShiftSM = _BPSSM;
	}

protected:

private:
	// 상태 틱
	void Tick(float _DeltaTime) override;
	void ProcessSelectPokemonForce();
	void ProcessPlayerShift();

	// 입력 데이터
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// 상태
	ESubstate State = ESubstate::None;

	// SM
	ABattlePlayerShiftStateMachine* BattlePlayerShiftSM = nullptr;
};

