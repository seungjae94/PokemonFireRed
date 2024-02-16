#include "EventTarget.h"

AEventTarget::AEventTarget()
{
}

AEventTarget::~AEventTarget()
{
}


void AEventTarget::ChangeAnimation(ETargetMoveState _State, const FTileVector& _Direction)
{
	std::string StateName;

	switch (_State)
	{
	case ETargetMoveState::Idle:
		StateName = "Idle";
		break;
	case ETargetMoveState::Walk:
		StateName = "Walk";
		break;
	default:
		break;
	}

	std::string AniName = GetName() + StateName + _Direction.ToDirectionString();
	UpperBodyRenderer->ChangeAnimation(AniName + Global::SuffixUpperBody);
	LowerBodyRenderer->ChangeAnimation(AniName + Global::SuffixLowerBody);
}
