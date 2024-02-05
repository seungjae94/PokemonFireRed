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
	// ���� Ŭ�������� �ʹ� ���� ����� �� ���Ƽ� protected�� ����
	AMap* Map = nullptr;
	APlayer* Player = nullptr;
	UEngineDirectory CurDir;
};

