#pragma once
#include <EngineBase\Transform.h>
#include "TickObject.h"
#include <EngineBase\NameObject.h>
#include "ImageRenderer.h"
#include "Collision.h"
#include "Level.h"

class ULevel;
class UActorComponent;
class UImageRenderer;

// �̸��� �ְ�, �� ƽ���� Ư�� ������ �ϸ�, ���� �󿡼� Transform�� ���� ������Ʈ.
class AActor : public UNameObject, public UTickObject
{
	friend ULevel;		// ULevel�� SetWorld ����
public:
	// constructor destructor
	AActor();

	// �������� �����Ѱ� �����̹Ƿ� �������� �������ϴ� å���� ���Ϳ��� �ִ�.
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

	template<typename EnumType>
	UCollision* CreateCollision(EnumType _Order = 0)
	{
		return CreateCollision(static_cast<int>(_Order));
	}

	template<typename EnumType>
	UImageRenderer* CreateImageRenderer(EnumType _Order = 0)
	{
		return CreateImageRenderer(static_cast<int>(_Order));
	}

	UCollision* CreateCollision(int _Order = 0);

	// �������� �����Ѵ�.
	// - �������� Owner�� ���� ���ͷ� �����Ѵ�.
	// - �������� SetOrder�� ȣ���Ѵ�. �� �� �������� ������ ���� �ִ� ������ �ʿ� �ڽ��� �߰��Ѵ�.
	// - �������� BeginPlay�� ȣ���Ѵ�.
	// - ���Ͱ� ���� �ִ� ������ ����Ʈ�� �������� �߰��Ѵ�.
	UImageRenderer* CreateImageRenderer(int _Order = 0);


	// �ڽ��� Destroy�� �� ���� �ִ� �������� Destroy �ؾ� �Ѵ�.
	// - ������ ������ ���� ���� �ֱ� ������ �������� �޸𸮿��� ����� ���� ������ ������ �ʿ��� ������� �Ѵ�.
	void Destroy(float _DestroyTime = 0.0f) override;

	void DestroyUpdate(float _DeltaTime) override;

	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;
	void ActiveUpdate(float _DeltaTime) override;

	// Actor�� ������� ��, �������� �ݸ����� Destory �ȴٸ� ���Ͱ� ���� �����Ѵ�.
	void CheckReleaseChild();

	void AllRenderersActiveOff();
	void AllRenderersActiveOn();

protected:
	void Tick(float _DeltaTime) override;
	void ChildTick(float _DeltaTime);
private:
	std::list<UImageRenderer*> Renderers;
	std::list<UCollision*> Collisions;

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

