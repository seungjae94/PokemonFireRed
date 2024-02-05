#pragma once
#include <EngineCore/Level.h>
#include "Player.h"
#include "Map.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

class UMapLevel : public ULevel
{
public:
	// constructor destructor
	UMapLevel();
	~UMapLevel();

	// delete Function
	UMapLevel(const UMapLevel& _Other) = delete;
	UMapLevel(UMapLevel&& _Other) noexcept = delete;
	UMapLevel& operator=(const UMapLevel& _Other) = delete;
	UMapLevel& operator=(UMapLevel&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:
	// 하위 클래스에서 너무 많이 사용할 것 같아서 protected로 설정
	AMap* Map = nullptr;
	APlayer* Player = nullptr;
	UEngineDirectory CurDir;
};

