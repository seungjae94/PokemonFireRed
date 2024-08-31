#pragma once

#include <string>
#include <vector>
#include <list>

#include "Text.h"
#include "PlayerData.h"
#include "EventCommand.h"
#include "EventEnums.h"

class UEventProcessor;

class UEventStream
{
	friend UEventProcessor;
	friend class UEventStreamReleaser;
public:
	UEventStream();
	~UEventStream();

public:
	template<typename CommandType>
	UEventStream& operator>>(const CommandType& _Command)
	{
		static_assert(std::is_base_of<UEventCommand, CommandType>::value);

		UEventCommand* Command = new CommandType(_Command);
		EventCommands.push_back(Command);
		return *this;
	}

	static UEventStream& Start(bool _DeactivatePlayer)
	{
		UEventStream* NewStream = new UEventStream();
		NewStream->DeactivatePlayer = _DeactivatePlayer;
		AllStreams.push_back(NewStream);
		return *NewStream;
	}
protected:

private:
	inline static std::list<UEventStream*> AllStreams;
	std::vector<UEventCommand*> EventCommands;
	bool DeactivatePlayer = true;
};

namespace ES
{
	UEventStream& Start(bool _DeactivatePlayer);
}