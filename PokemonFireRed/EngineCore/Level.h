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
class UCollision;

// ���͵��� Ȱ���� ����
class ULevel : public UNameObject
{
	friend UEngineCore;
	friend UImageRenderer; // Renderers�� ã�� Renderer�� ���ϰ� �ٷ�� ����
	friend UCollision;
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

	template<typename ActorType, typename EnumType>
	ActorType* SpawnActor(EnumType _Order)
	{
		return SpawnActor<ActorType>(static_cast<int>(_Order));
	}

	virtual void LevelStart(ULevel* _PrevLevel) {};
	virtual void LevelEnd(ULevel* _NextLevel) {};

	// ī�޶� ���� �Լ�
	void SetCameraPos(FVector _CameraPos)
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

	void SetAllTimeScale(float _Scale)
	{
		for (std::pair<const int, float>& TimeScale : TimeScale)
		{
			TimeScale.second = _Scale;
		}
	}

	template<typename EnumType>
	void SetOtherTimeScale(EnumType _Value, float _Scale)
	{
		SetOtherTimeScale(static_cast<int>(_Value), _Scale);
	}

	void SetOtherTimeScale(int _Value, float _Scale)
	{
		for (std::pair<const int, float>& TimeScale : TimeScale)
		{
			if (TimeScale.first == _Value)
			{
				continue;
			}

			TimeScale.second = _Scale;
		}
	}

	template<typename EnumType>
	void SetTimeScale(EnumType _Value, float _Scale)
	{
		SetTimeScale(static_cast<int>(_Value), _Scale);
	}

	void SetTimeScale(int _Value, float _Scale)
	{
		TimeScale[_Value] = _Scale;
	}

protected:

private:
	std::map<int, std::list<AActor*>> AllActor;
	std::map<int, float> TimeScale;

	std::map<int, std::list<UImageRenderer*>> Renderers;
	std::map<int, std::list<UCollision*>> Collisions;

	// ������ ���带 ���� ������ �����ϰ�, ������ BeginPlay�� ȣ���Ѵ�.
	void ActorInit(AActor* _Actor);

	// ������Ʈ(����) ƽ ȣ��
	void LevelTick(float _DeltaTime);

	// ������
	void LevelRender(float _DeltaTime);

	// Destroy�� ������Ʈ(����) ������
	void LevelRelease(float _DeltaTime);

	// ī�޶� ��ġ
	FVector CameraPos = FVector::Zero;
};

