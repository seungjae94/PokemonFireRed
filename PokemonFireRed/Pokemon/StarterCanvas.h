#pragma once
#include "Canvas.h"

class AStarterCanvas : public ACanvas
{
public:
	// constructor destructor
	AStarterCanvas();
	~AStarterCanvas();

	// delete Function
	AStarterCanvas(const AStarterCanvas& _Other) = delete;
	AStarterCanvas(AStarterCanvas&& _Other) noexcept = delete;
	AStarterCanvas& operator=(const AStarterCanvas& _Other) = delete;
	AStarterCanvas& operator=(AStarterCanvas&& _Other) noexcept = delete;

	void SetOptionBoxActive(bool _Value)
	{
		OptionBox->SetActive(_Value);
	}

	void SetPokemon(EPokemonId _Id)
	{
		PokemonImage->SetPokemon(_Id);
	}

	void IncCursor()
	{
		if (GetCursor() == 0)
		{
			OptionCursor->IncCursor();
		}
	}

	void DecCursor()
	{
		if (GetCursor() == 1)
		{
			OptionCursor->DecCursor();
		}
	}

	int GetCursor()
	{
		return OptionCursor->GetCursor();
	}

protected:

private:
	void BeginPlay() override;
	AImageElement* PokemonBackground = nullptr;
	AImageElement* PokemonImage = nullptr;
	AImageElement* OptionBox = nullptr;
	ACursor* OptionCursor = nullptr;
};

