#pragma once
#include <EngineCore/Actor.h>

class UImageRenderer;

class ABackground : public AActor
{
public:
	// constructor destructor
	ABackground();
	~ABackground();

	// delete Function
	ABackground(const ABackground& _Other) = delete;
	ABackground(ABackground&& _Other) noexcept = delete;
	ABackground& operator=(const ABackground& _Other) = delete;
	ABackground& operator=(ABackground&& _Other) noexcept = delete;

	void SetImageName(std::string_view _ImageName);
	void SetClip(int _X, int _Y);

protected:

private:
	int X = 0;
	int Y = 0;
	UImageRenderer* Renderer = nullptr;
};

