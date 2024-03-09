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

protected:

private:
	void BeginPlay() override;
	AImageElement* PokemonBackground = nullptr;
	AImageElement* PokemonImage = nullptr;
	AImageElement* OptionBox = nullptr;
	ACursor* OptionCursor = nullptr;
};

