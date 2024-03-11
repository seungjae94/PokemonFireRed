#pragma once
#include "Canvas.h"

class APokemonCenterCanvas : public ACanvas
{
public:
	// constructor destructor
	APokemonCenterCanvas();
	~APokemonCenterCanvas();

	// delete Function
	APokemonCenterCanvas(const APokemonCenterCanvas& _Other) = delete;
	APokemonCenterCanvas(APokemonCenterCanvas&& _Other) noexcept = delete;
	APokemonCenterCanvas& operator=(const APokemonCenterCanvas& _Other) = delete;
	APokemonCenterCanvas& operator=(APokemonCenterCanvas&& _Other) noexcept = delete;

protected:

private:

};

