#pragma once
#include "PokemonMath.h"
#include <EnginePlatform/EngineInput.h>

class PokemonInput
{
public:
	// constructor destructor
	PokemonInput();
	~PokemonInput();

	// delete Function
	PokemonInput(const PokemonInput& _Other) = delete;
	PokemonInput(PokemonInput&& _Other) noexcept = delete;
	PokemonInput& operator=(const PokemonInput& _Other) = delete;
	PokemonInput& operator=(PokemonInput&& _Other) noexcept = delete;

	static FTileVector GetPressingDirection()
	{
		FTileVector NextDirection = FTileVector::Zero;
		if (UEngineInput::IsPress(VK_DOWN))
		{
			NextDirection = FTileVector::Down;
		}
		else if (UEngineInput::IsPress(VK_UP))
		{
			NextDirection = FTileVector::Up;
		}
		else if (UEngineInput::IsPress(VK_LEFT))
		{
			NextDirection = FTileVector::Left;
		}
		else if (UEngineInput::IsPress(VK_RIGHT))
		{
			NextDirection = FTileVector::Right;
		}

		return NextDirection;
	}

protected:

private:

};

