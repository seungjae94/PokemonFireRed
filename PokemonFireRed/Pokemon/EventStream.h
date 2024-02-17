#pragma once

#include <string>
#include <vector>
#include "PokemonMath.h"
#include "PokemonText.h"

#define ES UEventStream

class UEventProcessor;

enum class EEventType
{
	Move,
	MoveWithoutRestriction,
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

enum class EFadeType
{
	Black,
	White,
	Curtain
};

class UEventStream
{
	friend UEventProcessor;
public:
	UEventStream();
	~UEventStream();

	class Move
	{
		friend UEventProcessor;
	public:
		Move(std::string_view _MapName, std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed = 3.6f)
			: MapName(_MapName), TargetName(_TargetName), Path(_Path), MoveSpeed(_MoveSpeed)
		{
		}
	private:
		std::string MapName;
		std::string TargetName;
		std::vector<FTileVector> Path;
		float MoveSpeed = 3.6f;
	};

	UEventStream& operator>>(const Move& _Data)
	{
		EventTypeList.push_back(EEventType::Move);
		MoveDataSet.push_back(_Data);
		return *this;
	}

	class MoveWithoutRestriction
	{
		friend UEventProcessor;
	public:
		MoveWithoutRestriction(std::string_view _MapName, std::string_view _TargetName, const std::vector<FVector>& _Path, float _MoveSpeed = 3.6f)
			: MapName(_MapName), TargetName(_TargetName), Path(_Path), MoveSpeed(_MoveSpeed)
		{
		}
	private:
		std::string MapName;
		std::string TargetName;
		std::vector<FVector> Path;
		float MoveSpeed = 3.6f;
	};

	UEventStream& operator>>(const MoveWithoutRestriction& _Data)
	{
		EventTypeList.push_back(EEventType::MoveWithoutRestriction);
		MoveWithoutRestrictionDataSet.push_back(_Data);
		return *this;
	}

	class FadeIn
	{
		friend UEventProcessor;
	public:
		FadeIn(float _Time, EFadeType _FadeType = EFadeType::Black)
			: Time(_Time), FadeType(_FadeType)
		{
		}
	private:
		float Time = 0.0f;
		EFadeType FadeType = EFadeType::Black;
	};

	UEventStream& operator>>(const FadeIn& _Data)
	{
		EventTypeList.push_back(EEventType::FadeIn);
		FadeInDataSet.push_back(_Data);
		return *this;
	}

	class FadeOut
	{
		friend UEventProcessor;
	public:
		FadeOut(float _Time, EFadeType _FadeType = EFadeType::Black)
			: Time(_Time), FadeType(_FadeType)
		{
		}
	private:
		float Time = 0.0f;
		EFadeType FadeType = EFadeType::Black;
	};

	UEventStream& operator>>(const FadeOut& _Data)
	{
		EventTypeList.push_back(EEventType::FadeOut);
		FadeOutDataSet.push_back(_Data);
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

	UEventStream& operator>>(const Wait& _Data)
	{
		EventTypeList.push_back(EEventType::Wait);
		WaitDataSet.push_back(_Data);
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

	UEventStream& operator>>(const Chat& _Data)
	{
		EventTypeList.push_back(EEventType::Chat);
		ChatDataSet.push_back(_Data);
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

	UEventStream& operator>>(const ChangeLevel& _Data)
	{
		EventTypeList.push_back(EEventType::ChangeLevel);
		ChangeLevelDataSet.push_back(_Data);
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

	UEventStream& operator>>(const ChangePoint& _Data)
	{
		EventTypeList.push_back(EEventType::ChangePoint);
		ChangePointDataSet.push_back(_Data);
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

	UEventStream& operator>>(const ChangeDirection& _Data)
	{
		EventTypeList.push_back(EEventType::ChangeDirection);
		ChangeDirectionDataSet.push_back(_Data);
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
	std::vector<Move> MoveDataSet;
	std::vector<MoveWithoutRestriction> MoveWithoutRestrictionDataSet;
	std::vector<FadeIn> FadeInDataSet;
	std::vector<FadeOut> FadeOutDataSet;
	std::vector<Wait> WaitDataSet;
	std::vector<Chat> ChatDataSet;
	std::vector<ChangeLevel> ChangeLevelDataSet;
	std::vector<ChangePoint> ChangePointDataSet;
	std::vector<ChangeDirection> ChangeDirectionDataSet;
	std::vector<StarePlayer> StarePlayerDataSet;
};

