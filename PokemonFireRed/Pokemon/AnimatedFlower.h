#pragma once
#include <EngineCore/Actor.h>
#include "Global.h"

class AnimatedFlower : public AActor
{
public:
	// constructor destructor
	AnimatedFlower();
	~AnimatedFlower();

	// delete Function
	AnimatedFlower(const AnimatedFlower& _Other) = delete;
	AnimatedFlower(AnimatedFlower&& _Other) noexcept = delete;
	AnimatedFlower& operator=(const AnimatedFlower& _Other) = delete;
	AnimatedFlower& operator=(AnimatedFlower&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:
	UImageRenderer* Renderer = nullptr;
};

