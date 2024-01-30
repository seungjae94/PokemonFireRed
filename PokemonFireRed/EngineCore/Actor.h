#pragma once
#include <EngineBase/NameObject.h>
#include <EngineBase/Transform.h>
#include <EngineCore/Level.h>
#include "TickObject.h"

// �̸��� �ְ�, �� ƽ���� Ư�� ������ �ϸ�, ���� �󿡼� Transform�� ���� ������Ʈ.
class AActor : public UNameObject, public UTickObject
{
	friend ULevel;		// ULevel�� SetWorld ����
public:
	// constructor destructor
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// Ʈ������ ���� �Լ�

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

	// World ���� �Լ�

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	ULevel* World = nullptr;
	FTransform Transform = FTransform();

	// ������ ���͸� ������ �� ȣ��Ǿ� ������ World�� �ش� ������ �������ִ� �Լ�
	// - ���������� ȣ���� �Լ��̹Ƿ� private���� �����Ѵ�.
	// - ������ ȣ���� �� �־�� �ϹǷ� ULevel�� friend�� �ɾ��ش�.
	void SetWorld(ULevel* _World)
	{
		World = _World;
	}
};

