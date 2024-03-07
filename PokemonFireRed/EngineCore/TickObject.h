#pragma once
#include <EngineBase\EngineDebug.h>

// ������ �����ϸ鼭 �� ƽ���� Ư���� ������ �ϴ� ������Ʈ
// - Transform�� ���� ���� �� �ִٴ� ������ ���Ϳ� ���̰� �ִ�.
// - ƽ ����, ����� ���õ� API�� �����Ѵ�.
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

	// ActiveValue ���� �Լ�

	void ActiveOn()
	{
		IsActiveValue = true;
	}

	void ActiveOff()
	{
		IsActiveValue = false;
	}

	virtual void SetActive(bool _Active, float _ActiveTime = 0.0f)
	{
		ActiveTime = _ActiveTime;

		if (true == _Active && 0.0f == ActiveTime)
		{
			IsActiveValue = _Active;
			return;
		}

		if (true == _Active && 0.0f != ActiveTime)
		{
			IsActiveUpdate = true;
		}
		IsActiveValue = false;
	}

	virtual void ActiveUpdate(float _DeltaTime)
	{
		ActiveTime -= _DeltaTime;

		if (true == IsActiveUpdate)
		{
			if (0.0f >= ActiveTime)
			{
				IsActiveUpdate = false;
				IsActiveValue = true;
				return;
			}
		}
	}

	bool IsActive()
	{
		return IsActiveValue && IsDestroyValue == false;
	}

	// ������ ��� Destroy�� �� �� ���� �ִ� �������� ���� Destroy�� �Ѵ�.
	// �׷��� ���� �Լ��� �����Ѵ�.
	virtual void Destroy(float _DestroyTime = 0.0f)
	{
		IsDestroyUpdate = true;
		_DestroyTime = DestroyTime;

		if (DestroyTime <= 0.0f)
		{
			IsDestroyValue = true;
		}
	}

	// DestroyTime�� �����ϰ� Destroy �� ���� �Ǿ����� üũ�Ѵ�.
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

	// Order ���� �Լ�

	int GetOrder()
	{
		return Order;
	}

	// UTickObject�� ����� ���Ϳ� �������� ������ �����ϴ� �� �ܿ��� �߰� �۾��� �ؾ� �ϹǷ� ���� �Լ��� ����
	// - ����: ������ AllActor ���� ����
	// - ������: ������ Renderers ���� ����
	virtual void SetOrder(int _Order)
	{
		Order = _Order;
	}

	void DebugCheckOn()
	{
		IsDebugCheck = true;
	}

	void DebugCheck()
	{
		if (true == IsDebugCheck)
		{
			int a = 0;
		}
	}

protected:

private:
	bool IsDebugCheck = false;

	// [Active ����]
	
	bool IsActiveValue = true;
	float ActiveTime = 0.0f;
	bool IsActiveUpdate = false;

	// [Destroy ����]
	
	bool IsDestroyValue = false;		// ���������� �����Ǿ����� ����
	float DestroyTime = 0.0f;			// Destroy���� ���� �ð�
	bool IsDestroyUpdate = false;		// DestroyUpdate ���� ���θ� ����


	// [���� ����]
	// - ���Ϳ����� ������Ʈ ������ ���
	// - ������������ ������ ������ ���
	int Order = 0;
};

