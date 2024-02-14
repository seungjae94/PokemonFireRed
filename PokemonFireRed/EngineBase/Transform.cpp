#include "Transform.h"
#include "EngineDebug.h"

bool (*FTransform::CollisionFunction[static_cast<int>(Rect)][static_cast<int>(Rect)])(const FTransform& _Left, const FTransform& _Right);

FTransform::FTransform()
{
}

FTransform::~FTransform()
{
}

bool FTransform::CircleToCircle(const FTransform& _Left, const FTransform& _Right)
{
	FVector _Dir = _Right.Position - _Left.Position;
	float Len = _Right.Scale.hX() + _Left.Scale.hX();

	return _Dir.Size2D() <= Len;
}

bool FTransform::Collision(ECollisionType _ThisType, ECollisionType _OtherType, const FTransform& _Other)
{
	if (nullptr == CollisionFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)])
	{
		MsgBoxAssert("아직 충돌 함수를 구현하지 않은 충돌입니다.");
	}

	return CollisionFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)](*this, _Other);
}

class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		FTransform::CollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)]
			= FTransform::CircleToCircle;
	}

	~CollisionFunctionInit()
	{

	}
};

CollisionFunctionInit CollisionFunctionInitInst;