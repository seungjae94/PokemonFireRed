#pragma once
#include <EngineCore/Actor.h>
#include "PlayerData.h"
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class UBattleLevel;
class ABattleCanvas;
class APokemonMsgBox;

class ABattlePlayerActionSelectStateMachine : public AActor
{
private:
	enum ECursorName
	{
		Fight,
		Bag,
		Pokemon,
		Run
	};

	enum class ESubstate
	{
		None,
		Select,
		MoveSelect,
		PokemonSelect,
		CantRunMessage1,
		CantRunMessage2,
		End
	};
public:
	// constructor destructor
	ABattlePlayerActionSelectStateMachine();
	~ABattlePlayerActionSelectStateMachine();

	// delete Function
	ABattlePlayerActionSelectStateMachine(const ABattlePlayerActionSelectStateMachine& _Other) = delete;
	ABattlePlayerActionSelectStateMachine(ABattlePlayerActionSelectStateMachine&& _Other) noexcept = delete;
	ABattlePlayerActionSelectStateMachine& operator=(const ABattlePlayerActionSelectStateMachine& _Other) = delete;
	ABattlePlayerActionSelectStateMachine& operator=(ABattlePlayerActionSelectStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}


	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy);

	void Reset();

protected:

private:
	// ��Ʋ ���� ������
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// ���� ������
	float Timer = 0.0f;
	int RunAttemptCount = 0;
	int SelectedPokemonIndex = 0;

	// ����
	ESubstate State = ESubstate::None;

	// ����
	void Tick(float _DeltaTime) override;

	// ���� ƽ �Լ�
	void ProcessSelect();
	void ProcessMoveSelect();
	void ProcessPokemonSelect();
	void ProcessCantRunMessage1();
	void ProcessCantRunMessage2();

	// ��ƿ �Լ�
	bool CalcRunResult() const;
};

