#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"
#include "ExpCalculator.h"
#include "BattleExpGainStateMachine.h"

class ABattleFaintStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		TestFaint,
		HidePokemon,
		ShowFaintMessage,
		TestExpGain,
		ExpGain,
		End
	};
public:
	// constructor destructor
	ABattleFaintStateMachine();
	~ABattleFaintStateMachine();

	// delete Function
	ABattleFaintStateMachine(const ABattleFaintStateMachine& _Other) = delete;
	ABattleFaintStateMachine(ABattleFaintStateMachine&& _Other) noexcept = delete;
	ABattleFaintStateMachine& operator=(const ABattleFaintStateMachine& _Other) = delete;
	ABattleFaintStateMachine& operator=(ABattleFaintStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void SetEGSM(ABattleExpGainStateMachine* _EGSM)
	{
		BattleExpGainSM = _EGSM;
	}

	void Start(ABattleCanvas* _Canvas, UBattler* _Attacker, UBattler* _Defender);

protected:
private:
	void Tick(float _DeltaTime) override;

	ABattleCanvas* Canvas = nullptr;
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;

	// ���� ������
	ESubstate State = ESubstate::None;
	UBattler* Fainter = nullptr;
	std::list<UBattler*> Fainters;
	const float FaintTime = 0.5f;
	float Timer = 0.0f;

	// ���� ������ - ����ġ ȹ�� ������
	FSimExpGainResult SimExpGainResult;
	std::map<UPokemon*, int> ExpGainByFaint;

	// ���� ƽ �Լ�
	void ProcessTestFaint();
	void ProcessHidePokemon();
	void ProcessShowFaintMessage();
	void ProcessTestExpGain();
	void ProcessExpGain();

	// SM - �� ���ϸ��� ����ġ ȹ�游�� ���
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;
};

