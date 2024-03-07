#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"
#include "ExpCalculator.h"

class ABattleCanvas;
class APokemonMsgBox;

class ABattleExpGainStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		ExpGainMessage1,
		ExpGainMessage2,
		TestExpBarIncrease,
		ExpBarIncrease,
		LevelUpEffect,
		LevelUpMessage1,
		LevelUpMessage2,
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

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UPokemon* _ExpGainer, int _Exp, bool _IsCurPokemon);

protected:

	void Tick(float _DeltaTime) override;
private:
	ESubstate State = ESubstate::None;

	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UPokemon* ExpGainer = nullptr;
	int Exp = 0;
	bool IsCurPokemon = false;

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
	void ProcessExpGainMessage1();
	void ProcessExpGainMessage2();
	void ProcessTestExpBarIncrease();
	void ProcessExpBarIncrease();
	void ProcessLevelUpEffect();
	void ProcessLevelUpMessage1();
	void ProcessLevelUpMessage2();
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

