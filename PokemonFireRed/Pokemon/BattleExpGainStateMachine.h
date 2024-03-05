#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"
#include "BattleCanvas.h"
#include "ExpCalculator.h"

class ABattleExpGainStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		ExpGainMessage,
		TestExpBarIncrease,
		ExpBarIncrease,
		LevelUpEffect,
		LevelUpMessage,
		StatUpDiffWindow,
		StatUpAfterWindow,
		TestLearnMove,
		LearnMoveSuccessMessage,
		LearnMoveFailMessage,
		LearnMoveEraseMessage,
		LearnMoveEraseSelection,
		LearnMoveNoEraseMessage,
		LearnMoveEraseSuccessMessage,
		End,
	};
public:
	// constructor destructor
	ABattleExpGainStateMachine();
	~ABattleExpGainStateMachine();

	// delete Function
	ABattleExpGainStateMachine(const ABattleExpGainStateMachine& _Other) = delete;
	ABattleExpGainStateMachine(ABattleExpGainStateMachine&& _Other) noexcept = delete;
	ABattleExpGainStateMachine& operator=(const ABattleExpGainStateMachine& _Other) = delete;
	ABattleExpGainStateMachine& operator=(ABattleExpGainStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, UPokemon* _ExpGainer, int _Exp);

protected:

	void Tick(float _DeltaTime) override;
private:
	ESubstate State = ESubstate::None;

	ABattleCanvas* Canvas = nullptr;
	UPokemon* ExpGainer = nullptr;
	int Exp = 0;

	// 고유 데이터
	float Timer = 0.0f;
	FSimExpGainResult SimResult;
	int PrevCurLevelExp = 0;
	int NextCurLevelExp = 0;
	int LevelUpCount = 0;
	const float ExpBarIncTime = 1.0f;
	const float LevelUpEffectTime = 0.5f;
	FLevelUpData LevelUpData;

	// 상태 틱 함수
	void ProcessExpGainMessage();
	void ProcessExpBarIncrease();
	void ProcessLevelUpEffect();
	void ProcessLevelUpMessage();
	void ProcessStatUpDiffWindow();
	void ProcessStatUpAfterWindow();
	void ProcessTestLearnMove();
	void ProcessLearnMoveSuccessMessage();
	void ProcessLearnMoveFailMessage();
	void ProcessLearnMoveEraseMessage();
	void ProcessLearnMoveEraseSelection();
	void ProcessLearnMoveNoEraseMessage();
	void ProcessLearnMoveEraseSuccessMessage();
};

