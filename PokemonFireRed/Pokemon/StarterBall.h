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

	void SetPokemon(EPokedexNo _PokemonId);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Renderer = nullptr;
	AStarterCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	
	EState State = EState::None;

	bool IsZClickEventOccur();
	void CheckEventOccur();
	void ProcessNonEventMessage();
	void OpenWhileEvent();
};

