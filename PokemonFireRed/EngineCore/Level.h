#pragma once
#include <map>
#include <list>
#include <EngineBase/NameObject.h>
#include <EngineBase\EngineMath.h>

// 상위 개념인 레벨이 하위 개념인 액터, 렌더러를 직접 참조하면 순환 참조 문제가 발생할 수 있다.
// 클래스 전방 선언으로 순환 참조 문제를 회피한다.
class AActor;
class UEngineCore;
class UImageRenderer;
class UCollision;

// 액터들이 활동할 무대
class ULevel : public UNameObject
{
	friend UEngineCore;
	friend UImageRenderer; // Renderers로 찾은 Renderer를 편하게 다루기 위함
	friend UCollision;
public:
	// constructor destructor
	ULevel();

	// 레벨이 액터를 생성하므로 레벨이 액터를 릴리즈해야 한다.
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

	// 레벨에 액터를 생성해주는 함수
	// - 액터의 월드를 현재 레벨로 설정하고, 액터의 BeginPlay를 호출한다.
	// - 액터를 map에 저장한다.
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

	// 카메라 관련 함수
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

	// 액터의 월드를 현재 레벨로 설정하고, 액터의 BeginPlay를 호출한다.
	void ActorInit(AActor* _Actor);

	// 오브젝트(액터) 틱 호출
	void LevelTick(float _DeltaTime);

	// 렌더링
	void LevelRender(float _DeltaTime);

	// Destroy한 오브젝트(액터) 릴리즈
	void LevelRelease(float _DeltaTime);

	// 카메라 위치
	FVector CameraPos = FVector::Zero;
};

