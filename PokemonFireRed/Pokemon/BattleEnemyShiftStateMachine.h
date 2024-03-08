#pragma once
#include <EngineCore/Actor.h>

class ABattleCanvas;
class APokemonMsgBox;
class UBattler;

class ABattleEnemyShiftStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		EntryArrowMove,
		PokemonAppearWait,
		PokemonAppear,
		PokemonBoxMove,
		End,
	};
public:
	// constructor destructor
	ABattleEnemyShiftStateMachine();
	~ABattleEnemyShiftStateMachine();

	// delete Function
	ABattleEnemyShiftStateMachine(const ABattleEnemyShiftStateMachine& _Other) = delete;
	ABattleEnemyShiftStateMachine(ABattleEnemyShiftStateMachine&& _Other) noexcept = delete;
	ABattleEnemyShiftStateMachine& operator=(const ABattleEnemyShiftStateMachine& _Other) = delete;
	ABattleEnemyShiftStateMachine& operator=(ABattleEnemyShiftStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, const UBattler* _Enemy);

protected:

private:
	ESubstate State = ESubstate::None;
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	const UBattler* Enemy = nullptr;

	// ���� ������ - ���
	const float EnemyArrowMoveTime = 0.5f;
	const float EntryFadeTime = 0.5f;
	const float EnemyPokemonAppearWaitTime = 0.75f;
	const float EnemyPokemonAppearTime = 0.25f;
	const float EnemyPokemonBoxMoveTime = 0.5f;

	// ���� ������ - Ÿ�̸�
	float Timer = 0.0f;
	float EntryFadeTimer = 0.0f;
	float EnemyPokemonAppearTimer = 0.0f;


	// ƽ
	void Tick(float _DeltaTime) override;

	void ProcessEntryArrowMove();
	void ProcessPokemonAppear(float _DeltaTime);
	void ProcessPokemonAppearWait();
	void ProcessPokemonBoxMove();
};

