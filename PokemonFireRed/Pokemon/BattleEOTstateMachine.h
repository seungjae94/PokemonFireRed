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

private:
	ABattleCanvas* Canvas = nullptr;
	UBattler* Target = nullptr;
	ESubstate State = ESubstate::None;

};

