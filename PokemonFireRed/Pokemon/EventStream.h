#pragma once

#include <string>
#include <vector>
#include "PokemonMath.h"
#include "PokemonText.h"

#define ES UEventStream

class UEventProcessor;

enum class EEventType
{
	MoveActor,
	FadeIn,
	FadeOut,
	Wait,
	Chat,
	ChangeLevel,
	ChangePoint,
	ChangeDirection,
	StarePlayer,
	End,
};

class UEventStream
{
	friend UEventProcessor;
public:
	UEventStream();
	~UEventStream();

	class MoveActor
	{
		friend UEventProcessor;
	public:
		MoveActor(std::string_view _MapName, std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed = 3.6f)
			: MapName(_MapName), TargetName(_TargetName), Path(_Path), MoveSpeed(_MoveSpeed)
		{
		}
	private:
		std::string MapName;
		std::string TargetName;
		std::vector<FTileVector> Path;
		float MoveSpeed = 3.6f;
	};

	UEventStream& operator>>(const MoveActor& _MoveActor)
	{
		EventTypeList.push_back(EEventType::MoveActor);
		MoveActorDataSet.push_back(_MoveActor);
		return *this;
	}

	class FadeIn
	{
		friend UEventProcessor;
	public:
		FadeIn(float _Time)
			: Time(_Time)
		{
		}
	private:
		float Time;
	};

	UEventStream& operator>>(const FadeIn& _FadeIn)
	{
		EventTypeList.push_back(EEventType::FadeIn);
		FadeInDataSet.push_back(_FadeIn);
		return *this;
	}

	class FadeOut
	{
		friend UEventProcessor;
	public:
		FadeOut(float _Time)
			: Time(_Time)
		{
		}
	private:
		float Time;
	};

	UEventStream& operator>>(const FadeOut& _FadeOut)
	{
		EventTypeList.push_back(EEventType::FadeOut);
		FadeOutDataSet.push_back(_FadeOut);
		return *this;
	}

	class Wait
	{
		friend UEventProcessor;
	public:
		Wait(float _Time)
			: Time(_Time)
		{
		}
	private:
		float Time;

	};

	UEventStream& operator>>(const Wait& _Wait)
	{
		EventTypeList.push_back(EEventType::Wait);
		WaitDataSet.push_back(_Wait);
		return *this;
	}

	class Chat
	{
		friend UEventProcessor;
	public:
		Chat(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14, bool _IsSequential = true)
			: Dialogue(_Dialogue), Color(_Color), LineSpace(_LineSpace), IsSequential(_IsSequential)
		{
		}
	private:
		std::vector<std::wstring> Dialogue;
		EFontColor Color;
		int LineSpace = 14;
		bool IsSequential = true;
	};

	UEventStream& operator>>(const Chat& _Chat)
	{
		EventTypeList.push_back(EEventType::Chat);
		ChatDataSet.push_back(_Chat);
		return *this;
	}

	class ChangeLevel
	{
		friend UEventProcessor;
	public:
		ChangeLevel(std::string_view _LevelName)
			: LevelName(_LevelName)
		{
		}
	private:
		std::string LevelName;
	};

	UEventStream& operator>>(const ChangeLevel& _ChangeLevel)
	{
		EventTypeList.push_back(EEventType::ChangeLevel);
		ChangeLevelDataSet.push_back(_ChangeLevel);
		return *this;
	}

	class ChangePoint
	{
		friend UEventProcessor;
	public:
		ChangePoint(std::string_view _LevelName,
			std::string_view _TargetName, const FTileVector& _Point)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Point(_Point)
		{
		}
	private:
		std::string LevelName;
		std::string TargetName;
		FTileVector Point;
	};

	UEventStream& operator>>(const ChangePoint& _ChangePoint)
	{
		EventTypeList.push_back(EEventType::ChangePoint);
		ChangePointDataSet.push_back(_ChangePoint);
		return *this;
	}

	class ChangeDirection
	{
		friend UEventProcessor;
	public:
		ChangeDirection(std::string_view _LevelName,
			std::string_view _TargetName, const FTileVector& _Direction)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Direction(_Direction)
		{
		}
	private:
		std::string LevelName;
		std::string TargetName;
		FTileVector Direction;
	};

	UEventStream& operator>>(const ChangeDirection& _ChangeDirection)
	{
		EventTypeList.push_back(EEventType::ChangeDirection);
		ChangeDirectionDataSet.push_back(_ChangeDirection);
		return *this;
	}

	class StarePlayer
	{
		friend UEventProcessor;
	public:
		StarePlayer(std::string_view _TargetName)
			: TargetName(_TargetName)
		{
		}
	private:
		std::string TargetName;
	};

	UEventStream& operator>>(const StarePlayer& _Data)
	{
		EventTypeList.push_back(EEventType::StarePlayer);
		StarePlayerDataSet.push_back(_Data);
		return *this;
	}

	class End
	{
		friend UEventStream;
	public:
		End(bool _ActivatePlayer)
			: ActivatePlayer(_ActivatePlayer)
		{
		}
	private:
		bool ActivatePlayer = true;
	};

	UEventStream& operator>>(const End& _End)
	{
		EventTypeList.push_back(EEventType::End);
		ActivatePlayer = _End.ActivatePlayer;
		return *this;
	}

	static UEventStream Start(bool _DeactivatePlayer)
	{
		UEventStream StartStream = UEventStream();
		StartStream.DeactivatePlayer = _DeactivatePlayer;
		return StartStream;
	}
protected:

private:
	std::vector<EEventType> EventTypeList;
	bool DeactivatePlayer = true;
	bool ActivatePlayer = true;
	std::vector<MoveActor> MoveActorDataSet;
	std::vector<FadeIn> FadeInDataSet;
	std::vector<FadeOut> FadeOutDataSet;
	std::vector<Wait> WaitDataSet;
	std::vector<Chat> ChatDataSet;
	std::vector<ChangeLevel> ChangeLevelDataSet;
	std::vector<ChangePoint> ChangePointDataSet;
	std::vector<ChangeDirection> ChangeDirectionDataSet;
	std::vector<StarePlayer> StarePlayerDataSet;
};

