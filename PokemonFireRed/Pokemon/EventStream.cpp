#include "EventStream.h"
#include "Trainer.h"
#include "WildBattleTrigger.h"
#include "Pokemon.h"

UEventStream::UEventStream() 
{
}

UEventStream::~UEventStream() 
{
	for (EventCommand* Command : EventCommands)
	{
		if (nullptr != Command)
		{
			delete Command;
		}
	}

	EventCommands.clear();
}

UEventStream::Move::Move(std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed, bool _CameraFollow)
	: MoveSpeed(_MoveSpeed), CameraFollow(_CameraFollow), EventCommand(EEventType::Move)
{
	TargetNames.push_back(std::string(_TargetName));
	Paths.push_back(_Path);
}

class UEventStreamReleaser
{
public:
	~UEventStreamReleaser()
	{
		for (UEventStream* Stream : UEventStream::AllStreams)
		{
			if (nullptr != Stream)
			{
				delete Stream;
			}
		}
		UEventStream::AllStreams.clear();
	}
};

UEventStreamReleaser Releaser;
