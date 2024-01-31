#pragma once

#include <EngineBase/Transform.h>
#include "ActorComponent.h"

// 트랜스폼을 갖고 있는 액터 컴포넌트
class USceneComponent : public UActorComponent
{
public:
	// constructor destructor
	USceneComponent();
	~USceneComponent();

	// delete Function
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	// 트랜스폼 관련 함수
	void SetPosition(const FVector& _Value)
	{
		Transform.SetPosition(_Value);
	}

	void SetScale(const FVector& _Value)
	{
		Transform.SetScale(_Value);
	}

	FTransform GetTransform()
	{
		return Transform;
	}

protected:

private:
	FTransform Transform;
};

