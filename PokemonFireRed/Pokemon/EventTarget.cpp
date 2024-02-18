#include "EventTarget.h"

AEventTarget::AEventTarget()
{
}

AEventTarget::~AEventTarget()
{
}


void AEventTarget::ChangeAnimation(ETargetMoveState _State, const FTileVector& _Direction)
{
	std::string UpperAniName = GetName(); 
	std::string LowerAniName = GetName(); 

	switch (_State)
	{
	case ETargetMoveState::Idle:
		LowerAniName += "Idle" + _Direction.ToDirectionString() + Global::SuffixLowerBody;
		UpperAniName += "Idle" + _Direction.ToDirectionString() + Global::SuffixUpperBody;
		break;
	case ETargetMoveState::Walk:
		UpperAniName += "Walk" + _Direction.ToDirectionString() + Global::SuffixUpperBody + std::to_string(MoveFootOrder);
		LowerAniName += "Walk" + _Direction.ToDirectionString() + Global::SuffixLowerBody + std::to_string(MoveFootOrder);
		IncMoveFootOrder();
		break;
	default:
		break;
	}

	UpperBodyRenderer->ChangeAnimation(UpperAniName);
	LowerBodyRenderer->ChangeAnimation(LowerAniName);
}
