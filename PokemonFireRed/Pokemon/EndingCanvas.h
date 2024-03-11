#pragma once
#include "Canvas.h"

class AEndingCanvas : public ACanvas
{
public:
	// constructor destructor
	AEndingCanvas();
	~AEndingCanvas();

	// delete Function
	AEndingCanvas(const AEndingCanvas& _Other) = delete;
	AEndingCanvas(AEndingCanvas&& _Other) noexcept = delete;
	AEndingCanvas& operator=(const AEndingCanvas& _Other) = delete;
	AEndingCanvas& operator=(AEndingCanvas&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;

	AImageElement* Background = nullptr;
	AImageElement* Message = nullptr;
	AImageElement* RunnerGround = nullptr;
	AImageElement* Runner = nullptr;
};

