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

	// 고유 데이터
	ESubstate State = ESubstate::None;
	UBattler* Fainter = nullptr;
	std::list<UBattler*> Fainters;
	const float FaintTime = 0.5f;
	float Timer = 0.0f;

	// 고유 데이터 - 경험치 획득 데이터
	FSimExpGainResult SimExpGainResult;
	std::map<UPokemon*, int> ExpGainByFaint;

	// 상태 틱 함수
	void ProcessTestFaint();
	void ProcessHidePokemon();
	void ProcessShowFaintMessage();
	void ProcessTestExpGain();
	void ProcessExpGain();

	// SM - 한 포켓몬의 경험치 획득만을 담당
	ABattleExpGainStateMachine* BattleExpGainSM = nullptr;
};

