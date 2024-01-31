#pragma once
#include <EngineBase\EngineDebug.h>

// 레벨에 존재하면서 매 틱마다 특정한 동작을 하는 오브젝트
// - Transform을 갖지 않을 수 있다는 점에서 액터와 차이가 있다.
// - 틱 순서, 존재와 관련된 API를 제공한다.
class UTickObject
{
public:
	// constructor destructor
	UTickObject();
	virtual ~UTickObject();

	// delete Function
	UTickObject(const UTickObject& _Other) = delete;
	UTickObject(UTickObject&& _Other) noexcept = delete;
	UTickObject& operator=(const UTickObject& _Other) = delete;
	UTickObject& operator=(UTickObject&& _Other) noexcept = delete;

	virtual void BeginPlay() 
	{
	}

	virtual void Tick(float _DeltaTime)
	{
	}

	// ActiveValue 관련 함수

	void ActiveOn()
	{
		IsActiveValue = true;
	}

	void ActiveOff()
	{
		IsActiveValue = false;
	}

	void SetActive(bool _IsActiveValue)
	{
		IsActiveValue = _IsActiveValue;
	}

	bool IsActive()
	{
		return IsActiveValue && IsDestroyValue == false;
	}

	// 액터의 경우 Destroy를 할 때 갖고 있는 렌더러도 전부 Destroy를 한다.
	// 그래서 가상 함수로 선언한다.
	virtual void Destroy(float _DestroyTime = 0.0f)
	{
		IsDestroyUpdate = true;
		_DestroyTime = DestroyTime;

		if (DestroyTime <= 0.0f)
		{
			IsDestroyValue = true;
		}
	}

	// DestroyTime을 갱신하고 Destroy 할 때가 되었는지 체크한다.
	virtual void DestroyUpdate(float _DeltaTime)
	{
		if (IsDestroyUpdate == false)
		{
			return;
		}

		DestroyTime -= _DeltaTime;

		if (DestroyTime <= 0.0f)
		{
			Destroy(0.0f);
		}
	}

	bool IsDestroy()
	{
		return IsDestroyValue;
	}

	// Order 관련 함수

	int GetOrder()
	{
		return Order;
	}

	// UTickObject를 상속한 액터와 렌더러는 순서를 변경하는 것 외에도 추가 작업을 해야 하므로 가상 함수로 선언
	// - 액터: 레벨의 AllActor 맵을 수정
	// - 렌더러: 레벨의 Renderers 맵을 수정
	virtual void SetOrder(int _Order)
	{
		Order = _Order;
	}

protected:

private:
	// [Active 관련]
	
	// 현재 활성화중인지 여부
	bool IsActiveValue = true;

	// [Destroy 관련]
	
	bool IsDestroyValue = false;		// 영구적으로 삭제되었는지 여부
	float DestroyTime = 0.0f;			// Destroy까지 남은 시간
	bool IsDestroyUpdate = false;		// IsDestroyUpdate == true인 경우 DestroyUpdate의 로직이 실행되어 IsDestroyValue가 변경된다.

	// [순서 관련]
	// - 액터에서는 업데이트 순서로 사용
	// - 렌더러에서는 렌더링 순서로 사용
	int Order = 0;
};

