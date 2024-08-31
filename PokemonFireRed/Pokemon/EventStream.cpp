#include "EventStream.h"
#include "Trainer.h"
#include "WildBattleTrigger.h"
#include "Pokemon.h"

UEventStream::UEventStream()
{
}

UEventStream::~UEventStream()
{
    for (UEventCommand* Command : EventCommands)
    {
        if (nullptr != Command)
        {
            delete Command;
        }
    }

    EventCommands.clear();
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

UEventStream& ES::Start(bool _DeactivatePlayer)
{
    return UEventStream::Start(_DeactivatePlayer);
}
