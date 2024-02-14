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

	// 충돌 순서가 _Order인 충돌체와 충돌하는지 여부를 반환한다.
	// 충돌할 경우 충돌체 전부를 _Result에 담아준다. 
	template<typename EnumType>
	bool CollisionCheck(EnumType _Order, std::vector<UCollision*>& _Result)
	{
		return CollisionCheck(static_cast<int>(_Order), _Result);
	}

	// 충돌 순서가 _Order인 충돌체와 충돌하는지 여부를 반환한다.
	// 충돌할 경우 충돌체 전부를 _Result에 담아준다. 
	bool CollisionCheck(int _Order, std::vector<UCollision*>& _Result);

protected:
	void BeginPlay() override;

private:
	ECollisionType ColType = ECollisionType::CirCle;
};

