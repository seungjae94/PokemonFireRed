#pragma once
#include <EngineBase/NameObject.h>
#include <EngineBase/Transform.h>
#include <EngineCore/Level.h>
#include "TickObject.h"

// 이름이 있고, 매 틱마다 특정 동작을 하며, 레벨 상에서 Transform을 갖는 오브젝트.
class AActor : public UNameObject, public UTickObject
{
public:
	// constructor destructor
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// 트랜스폼 변경 함수

	FVector GetActorLocation()
	{
		return Transform.GetPosition();
	}

	void SetActorLocation(FVector _Value)
	{
		Transform.SetPosition(_Value);
	}

	void AddActorLocation(FVector _Value)
	{
		Transform.AddPosition(_Value);
	}

	void SetActorScale(FVector _Value)
	{
		Transform.SetScale(_Value);
	}

	FTransform GetTransform()
	{
		return Transform;
	}

	// World 관련 함수

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	ULevel* World = nullptr;
	FTransform Transform = FTransform();

	void SetWorld(ULevel* _World)
	{
		World = _World;
	}
};

