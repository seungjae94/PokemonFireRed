#pragma once
#include "EventTarget.h"
#include "Pokemon.h"
#include "StarterCanvas.h"
#include "PokemonMsgBox.h"

class AStarterBall : public AEventTarget
{
private:
	enum class EState
	{
		None,
		NonEventMessage,
		EventMessage1,
		EventMessage2,
		EventMessage3,
		Select,
		End,
	};
public:
	// constructor destructor
	AStarterBall();
	~AStarterBall();

	// delete Function
	AStarterBall(const AStarterBall& _Other) = delete;
	AStarterBall(AStarterBall&& _Other) noexcept = delete;
	AStarterBall& operator=(const AStarterBall& _Other) = delete;
	AStarterBall& operator=(AStarterBall&& _Other) noexcept = delete;

	void SetPokemon(EPokemonId _PokemonId)
	{
		PokemonId = _PokemonId;
	}

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Renderer = nullptr;
	AStarterCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	
	EState State = EState::None;
	EPokemonId PokemonId = EPokemonId::None;

	bool IsZClickEventOccur();
	void CheckEventOccur();

	// 상태 틱
	void ProcessNonEventMessage();
	void ProcessEventMessage1();
	void ProcessEventMessage2();
	void ProcessEventMessage3();
	void ProcessSelect();

	// 상태 전이
	void StateChangeToEventMessage1();
	void StateChangeToEventMessage3();
};

