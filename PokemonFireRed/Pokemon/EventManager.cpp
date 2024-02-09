#include "EventManager.h"

#include <EngineBase/EngineMath.h>
#include "Movable.h"

UEventManager::UEventManager()
{
}

UEventManager::~UEventManager()
{
}

bool UEventManager::MoveActor(float _DeltaTime, AMovable* _Mover, std::vector<FTileVector> _Path, float _MoveSpeed)
{
	if (_Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동 경로의 크기가 0 이하입니다.");
	}

	// 이동 이벤트 시작
	if (MoveActorIndex == -1)
	{
		MoveTime = 1.0f / _MoveSpeed;
		Timer = 0.0f;
		_Mover->SetMoveState(EMovableState::Walk);
		_Mover->ChangeAnimation(_Mover->MoveState, _Mover->Direction);
	}

	if (Timer > 0.0f)
	{
		Timer -= _DeltaTime;

		float t = (MoveTime - Timer) / MoveTime;

		FVector TargetPos = PokemonMath::Lerp(_Mover->PrevPos, _Mover->NextPos, t);
		FVector PlayerPos = _Mover->GetActorLocation();
		FVector AddPos = TargetPos - PlayerPos;
		_Mover->AddActorLocation(AddPos);
		_Mover->GetWorld()->SetCameraPos(_Mover->GetActorLocation() - Global::HALF_SCREEN);
	}
	else if (MoveActorIndex + 1 >= _Path.size())
	{
		MoveActorIndex = -1;
		_Mover->SetMoveState(EMovableState::Idle);
		_Mover->ChangeAnimation(_Mover->MoveState, _Mover->Direction);
		return true;
	}
	else
	{
		_Mover->PrevPos = _Mover->GetActorLocation();
		_Mover->NextPos = _Mover->PrevPos + _Path[MoveActorIndex + 1].ToFVector();
		Timer = MoveTime;
		if (_Mover->Direction != _Path[MoveActorIndex + 1])
		{
			_Mover->SetDirection(_Path[MoveActorIndex + 1]);
			_Mover->ChangeAnimation(_Mover->MoveState, _Mover->Direction);
		}
		MoveActorIndex++;
	}
	return false;
}