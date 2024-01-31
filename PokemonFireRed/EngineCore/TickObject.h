#pragma once

// ������ �����ϸ鼭 �� ƽ���� Ư���� ������ �ϴ� ������Ʈ
// - Transform�� ���� ���� �� �ִٴ� ������ ���Ϳ� ���̰� �ִ�.
// - ƽ ����, ����� ���õ� API�� �����Ѵ�.
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

	// IsDestroyValue ���� �Լ�
	bool IsDestroy()
	{
		return IsDestroyValue;
	}

	virtual void Destroy()
	{
		IsDestroyValue = true;
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
	// ���� Ȱ��ȭ������ ����
	bool IsActiveValue = true;

	// ���������� �����Ǿ����� ����
	bool IsDestroyValue = false;

	// ����
	// - ���Ϳ����� ������Ʈ ������ ���
	// - ������������ ������ ������ ���
	int Order = 0;
};

