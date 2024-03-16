#pragma once
#include "Canvas.h"

class ADebugCanvas : public ACanvas
{
public:
	// constructor destructor
	ADebugCanvas();
	~ADebugCanvas();

	// delete Function
	ADebugCanvas(const ADebugCanvas& _Other) = delete;
	ADebugCanvas(ADebugCanvas&& _Other) noexcept = delete;
	ADebugCanvas& operator=(const ADebugCanvas& _Other) = delete;
	ADebugCanvas& operator=(ADebugCanvas&& _Other) noexcept = delete;

	void Refresh();

protected:

private:
	void BeginPlay() override;

	AText* DmgInfo = nullptr;
	AText* ExpInfo = nullptr;
	AText* CatchRateInfo = nullptr;
	AText* EncounterRateInfo = nullptr;
};

