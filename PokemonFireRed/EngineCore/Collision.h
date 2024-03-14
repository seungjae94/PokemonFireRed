#pragma once
#include "SceneComponent.h"
#include <vector>

class UCollision : public USceneComponent
{
public:
	// constrcuter destructer
	UCollision();
	~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	void SetColType(ECollisionType _Type)
	{
		ColType = _Type;
	}

	void SetOrder(int _Order) override;

	// �浹 ������ _Order�� �浹ü�� �浹�ϴ��� ���θ� ��ȯ�Ѵ�.
	// �浹�� ��� �浹ü ���θ� _Result�� ����ش�. 
	template<typename EnumType>
	bool CollisionCheck(EnumType _Order, std::vector<UCollision*>& _Result, FVector _NextPos = FVector::Zero)
	{
		return CollisionCheck(static_cast<int>(_Order), _Result, _NextPos);
	}

	// �浹 ������ _Order�� �浹ü�� �浹�ϴ��� ���θ� ��ȯ�Ѵ�.
	// �浹�� ��� �浹ü ���θ� _Result�� ����ش�. 
	bool CollisionCheck(int _Order, std::vector<UCollision*>& _Result, FVector _NextPos = FVector::Zero);

	void DebugRender(FVector _CameraPos);

protected:
	void BeginPlay() override;

private:
	ECollisionType ColType = ECollisionType::Rect;
};

