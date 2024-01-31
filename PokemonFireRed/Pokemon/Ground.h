#pragma once
#include <EngineCore/Actor.h>

// 플레이어가 돌아다닐 바닥
class AGround : public AActor
{
public:
	// constructor destructor
	AGround();
	~AGround();

	// delete Function
	AGround(const AGround& _Other) = delete;
	AGround(AGround&& _Other) noexcept = delete;
	AGround& operator=(const AGround& _Other) = delete;
	AGround& operator=(AGround&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;
};

