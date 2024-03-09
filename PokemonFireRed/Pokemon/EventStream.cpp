#include "EventStream.h"
#include "Trainer.h"
#include "WildBattleTrigger.h"
#include "Pokemon.h"

UEventStream::UEventStream() 
{
}

UEventStream::~UEventStream() 
{
}

UEventStream::Move::Move(std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed, bool _CameraFollow)
	: MoveSpeed(_MoveSpeed), CameraFollow(_CameraFollow)
{
	TargetNames.push_back(std::string(_TargetName));
	Paths.push_back(_Path);
}
