#pragma once
#include "EventActor.h"

class AWarp : public AEventActor
{
public:
	// constructor destructor
	AWarp();
	~AWarp();

	// delete Function
	AWarp(const AWarp& _Other) = delete;
	AWarp(AWarp&& _Other) noexcept = delete;
	AWarp& operator=(const AWarp& _Other) = delete;
	AWarp& operator=(AWarp&& _Other) noexcept = delete;

protected:
	void TriggerEvent() override;
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;
	std::string TargetMapName;
	FTileVector TargetPoint;
};

