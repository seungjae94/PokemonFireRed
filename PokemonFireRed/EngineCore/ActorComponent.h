#pragma once

#include "TickObject.h"

class AActor;

// 액터가 갖고 있는 컴포넌트
class UActorComponent : public UTickObject
{
	friend AActor;
public:
	// constructor destructor
	UActorComponent();
	~UActorComponent();

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	AActor* GetOwner()
	{
		return Owner;
	}

protected:

private:
	AActor* Owner = nullptr;

	// Owner는 Actor만 변경할 수 있다.
	void SetOwner(AActor* _Owner)
	{
		Owner = _Owner;
	}
};

