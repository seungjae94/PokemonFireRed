#pragma once
#include "PokemonMath.h"
#include <EnginePlatform/EngineInput.h>
#include <list>

class UPokemonInput
{
public:
	// constructor destructor
	UPokemonInput();
	~UPokemonInput();

	// delete Function
	UPokemonInput(const UPokemonInput& _Other) = delete;
	UPokemonInput(UPokemonInput&& _Other) noexcept = delete;
	UPokemonInput& operator=(const UPokemonInput& _Other) = delete;
	UPokemonInput& operator=(UPokemonInput&& _Other) noexcept = delete;

	static FTileVector GetPressDirection()
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

	static FTileVector GetDownDirection()
	{
		FTileVector NextDirection = FTileVector::Zero;
		if (UEngineInput::IsDown(VK_DOWN))
		{
			NextDirection = FTileVector::Down;
		}
		else if (UEngineInput::IsDown(VK_UP))
		{
			NextDirection = FTileVector::Up;
		}
		else if (UEngineInput::IsDown(VK_LEFT))
		{
			NextDirection = FTileVector::Left;
		}
		else if (UEngineInput::IsDown(VK_RIGHT))
		{
			NextDirection = FTileVector::Right;
		}

		return NextDirection;
	}

	static bool IsAnykeyDown()
	{
		for (int Key : AnykeyList)
		{
			if (true == UEngineInput::IsDown(Key))
			{
				return true;
			}
		}

		for (char Ch = '0'; Ch <= '9'; ++Ch)
		{
			if (true == UEngineInput::IsDown(Ch))
			{
				return true;
			}
		}

		for (char Ch = 'A'; Ch <= 'Z'; ++Ch)
		{
			if (true == UEngineInput::IsDown(Ch))
			{
				return true;
			}
		}
	}



protected:

private:
	static std::list<int> AnykeyList;
};

