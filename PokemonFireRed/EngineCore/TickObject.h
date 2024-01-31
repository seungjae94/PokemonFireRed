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

	void SetActive(bool _IsActiveValue)
	{
		IsActiveValue = _IsActiveValue;
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

protected:

private:
	// [Active ����]
	
	// ���� Ȱ��ȭ������ ����
	bool IsActiveValue = true;

	// [Destroy ����]
	
	bool IsDestroyValue = false;		// ���������� �����Ǿ����� ����
	float DestroyTime = 0.0f;			// Destroy���� ���� �ð�
	bool IsDestroyUpdate = false;		// IsDestroyUpdate == true�� ��� DestroyUpdate�� ������ ����Ǿ� IsDestroyValue�� ����ȴ�.

	// [���� ����]
	// - ���Ϳ����� ������Ʈ ������ ���
	// - ������������ ������ ������ ���
	int Order = 0;
};

