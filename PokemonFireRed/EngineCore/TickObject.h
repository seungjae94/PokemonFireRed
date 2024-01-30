#pragma once

// 레벨에 존재하면서 매 틱마다 특정한 동작을 하는 오브젝트
// - Transform을 갖지 않을 수 있다는 점에서 액터와 차이가 있다.
// - 틱 순서, 존재와 관련된 API를 제공한다.
class UTickObject
{
public:
	// constructor destructor
	UTickObject();
	~UTickObject();

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

	// IsDestroyValue 관련 함수
	bool IsDestroy()
	{
		return IsDestroyValue;
	}

	void Destroy()
	{
		IsDestroyValue = true;
	}

	// Order 관련 함수

	int GetOrder()
	{
		return Order;
	}

	void SetOrder(int _Order)
	{
		Order = _Order;
	}

protected:

private:
	// 현재 활성화중인지 여부
	bool IsActiveValue = true;

	// 영구적으로 삭제되었는지 여부
	bool IsDestroyValue = false;

	// 업데이트 순서
	int Order = 0;
};

