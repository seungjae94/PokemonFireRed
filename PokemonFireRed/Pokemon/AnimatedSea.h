#pragma once
#include <EngineCore/Actor.h>
#include "Global.h"

class AnimatedSea : public AActor
{
public:
	// constructor destructor
	AnimatedSea();
	~AnimatedSea();

	// delete Function
	AnimatedSea(const AnimatedSea& _Other) = delete;
	AnimatedSea(AnimatedSea&& _Other) noexcept = delete;
	AnimatedSea& operator=(const AnimatedSea& _Other) = delete;
	AnimatedSea& operator=(AnimatedSea&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:
	UImageRenderer* Renderer = nullptr;
};

