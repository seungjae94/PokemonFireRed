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

	void SetOrder(int _Order) override;

	// �浹 ������ _Order�� �浹ü�� �浹�ϴ��� ���θ� ��ȯ�Ѵ�.
	// �浹�� ��� �浹ü ���θ� _Result�� ����ش�. 
	template<typename EnumType>
	bool CollisionCheck(EnumType _Order, std::vector<UCollision*>& _Result)
	{
		return CollisionCheck(static_cast<int>(_Order), _Result);
	}

	// �浹 ������ _Order�� �浹ü�� �浹�ϴ��� ���θ� ��ȯ�Ѵ�.
	// �浹�� ��� �浹ü ���θ� _Result�� ����ش�. 
	bool CollisionCheck(int _Order, std::vector<UCollision*>& _Result);

protected:
	void BeginPlay() override;

private:
	ECollisionType ColType = ECollisionType::CirCle;
};

