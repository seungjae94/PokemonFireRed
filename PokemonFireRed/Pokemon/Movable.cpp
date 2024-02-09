#include "Movable.h"

AMovable::AMovable()
{
}

AMovable::~AMovable()
{
}


void AMovable::ChangeAnimation(EMovableState _State, const FTileVector& _Direction)
{
	std::string StateName;

	switch (_State)
	{
	case EMovableState::Idle:
		StateName = "Idle";
		break;
	case EMovableState::Walk:
		StateName = "Walk";
		break;
	default:
		break;
	}

	std::string AniName = GetName() + StateName + _Direction.ToDirectionString();
	Renderer->ChangeAnimation(AniName);
}
