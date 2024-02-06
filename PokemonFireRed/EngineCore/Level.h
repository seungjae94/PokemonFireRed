#pragma once
#include <map>
#include <list>
#include <EngineBase/NameObject.h>
#include <EngineBase\EngineMath.h>

// ���� ������ ������ ���� ������ ����, �������� ���� �����ϸ� ��ȯ ���� ������ �߻��� �� �ִ�.
// Ŭ���� ���� �������� ��ȯ ���� ������ ȸ���Ѵ�.
class AActor;
class UEngineCore;
class UImageRenderer;

// ���͵��� Ȱ���� ����
class ULevel : public UNameObject
{
	friend UEngineCore;
	friend UImageRenderer; // ���������� Renderers ���� ����
public:
	// constructor destructor
	ULevel();

	// ������ ���͸� �����ϹǷ� ������ ���͸� �������ؾ� �Ѵ�.
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	virtual void BeginPlay() 
	{
	}

	virtual void Tick(float _DeltaTime) 
	{
	}

	// ������ ���͸� �������ִ� �Լ�
	// - ������ ���带 ���� ������ �����ϰ�, ������ BeginPlay�� ȣ���Ѵ�.
	// - ���͸� map�� �����Ѵ�.
	template<typename ActorType>
	ActorType* SpawnActor(int _Order = 0)
	{
		ActorType* NewActor = new ActorType();
		ActorInit(NewActor);
		AllActor[_Order].push_back(NewActor);
		return NewActor;
	}

	virtual void LevelStart(ULevel* _PrevLevel) {};
	virtual void LevelEnd(ULevel* _NextLevel) {};

	// ī�޶� ���� �Լ�
	void SddCameraPos(FVector _CameraPos)
	{
		CameraPos = _CameraPos;
	}


	void AddCameraPos(FVector _CameraPos)
	{
		CameraPos += _CameraPos;
	}

	FVector GetCameraPos()
	{
		return CameraPos;
	}

protected:

private:
	std::map<int, std::list<AActor*>> AllActor;
	std::map<int, std::list<UImageRenderer*>> Renderers;

	// ������ ���带 ���� ������ �����ϰ�, ������ BeginPlay�� ȣ���Ѵ�.
	void ActorInit(AActor* _Actor);

	// ������Ʈ(����) ƽ ȣ��
	void LevelTick(float _DeltaTime);

	// ������: ���� ����
	void LevelRender(float _DeltaTime);

	// Destroy�� ������Ʈ(����) ������
	void LevelRelease(float _DeltaTime);

	// ī�޶� ��ġ
	FVector CameraPos = FVector::Zero;
};

