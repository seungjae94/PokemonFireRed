#include "UIParent.h"
#include "UIElement.h"

AUIParent::AUIParent() 
{
}

AUIParent::~AUIParent() 
{
}

void AUIParent::AppendChild(AUIElement* _Element)
{
	Children.push_back(_Element);
}

void AUIParent::SetActive(bool _Active, float _ActiveTime)
{
	AActor::SetActive(_Active, _ActiveTime);

	for (AUIElement* Child : Children)
	{
		if (true == Child->GetIsFollowParentActive())
		{
			Child->SetActive(_Active, _ActiveTime);
		}
	}
}

void AUIParent::Tick(float _DeltaTime)
{
	for (AUIElement* Child : Children)
	{
		Child->FollowParentPosition();
	}
}

