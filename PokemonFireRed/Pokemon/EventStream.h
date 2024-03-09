#pragma once

#include <string>
#include <vector>
#include <list>
#include "PokemonMath.h"
#include "Text.h"

#define ES UEventStream

class UEventProcessor;
class ATrainer;
class AWildBattleTrigger;
class UPokemon;

enum class EEventType
{
	Move,
	MoveWithoutRestriction,
	Surprise,
	FadeIn,
	FadeOut,
	Wait,
	Chat,
	ShowMapName,
	ChangeLevel,
	ChangePoint,
	ChangePosition,
	ChangeDirection,
	StarePlayer,
	PlayAnimation,
	PlayBgm,
	HideActor,
	ShowActor,
	CameraFocus,
	DeactivatePlayerControl,
	WildBattle,
	TrainerBattle,
	End,
};

enum class EFadeType
{
	Black,
	White,
	HCurtain,
	VCurtain
};

enum class EAnimTarget
{
	All,
	UpperBodyOnly,
	LowerBodyOnly,
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
		Move(std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true);

		Move(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true)
			: TargetNames(_TargetNames), Paths(_Paths), MoveSpeed(_MoveSpeed), CameraFollow(_CameraFollow)
		{
		}
	private:
		std::vector<std::string> TargetNames;
		std::vector<std::vector<FTileVector>> Paths;
		bool CameraFollow = true;
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
		MoveWithoutRestriction(std::string_view _TargetName, const std::vector<FVector>& _Path, float _MoveSpeed = 3.6f)
			: TargetName(_TargetName), Path(_Path), MoveSpeed(_MoveSpeed)
		{
		}
	private:
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

	class PlayAnimation
	{
		friend UEventProcessor;
	public:
		PlayAnimation(std::string_view _TargetName, std::string_view _AnimName, bool _Wait = true, EAnimTarget _AnimTarget = EAnimTarget::All)
			: TargetName(_TargetName), AnimName(_AnimName), Wait(_Wait), AnimTarget(_AnimTarget)
		{
			if (_AnimName.ends_with(Global::SuffixLowerBody) || _AnimName.ends_with(Global::SuffixUpperBody))
			{
				MsgBoxAssert(AnimName + "은 유효하지 않습니다. PlayAnimation 이벤트에 애니메이션 이름을 적을 땐 LowerBody, UpperBody suffix를 생략해야 합니다.");
			}
		}
	private:
		std::string TargetName = "";
		std::string AnimName = "";
		bool Wait = true;
		EAnimTarget AnimTarget = EAnimTarget::All;
	};

	UEventStream& operator>>(const PlayAnimation& _Data)
	{
		EventTypeList.push_back(EEventType::PlayAnimation);
		PlayAnimationDataSet.push_back(_Data);
		return *this;
	}

	class PlayBgm
	{
		friend UEventProcessor;
	public:
		PlayBgm(std::string_view _Name)
			: Name(_Name)
		{
		}
	private:
		std::string Name = "";
	};

	UEventStream& operator>>(const PlayBgm& _Data)
	{
		EventTypeList.push_back(EEventType::PlayBgm);
		PlayBgmDataSet.push_back(_Data);
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

	class ShowMapName
	{
		friend UEventProcessor;
	public:
		ShowMapName(std::wstring_view _MapName)
			: MapName(_MapName)
		{
		}
	private:
		std::wstring MapName;
	};

	UEventStream& operator>>(const ShowMapName& _Data)
	{
		EventTypeList.push_back(EEventType::ShowMapName);
		ShowMapNameDataSet.push_back(_Data);
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

	class ChangePosition
	{
		friend UEventProcessor;
	public:
		ChangePosition(std::string_view _LevelName,
			std::string_view _TargetName, const FVector& _Position)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Position(_Position)
		{
		}
	private:
		std::string LevelName;
		std::string TargetName;
		FVector Position;
	};

	UEventStream& operator>>(const ChangePosition& _Data)
	{
		EventTypeList.push_back(EEventType::ChangePosition);
		ChangePositionDataSet.push_back(_Data);
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

	class HideActor
	{
		friend UEventProcessor;
	public:
		HideActor(std::string_view _TargetName)
			: TargetName(_TargetName)
		{
		}
	private:
		std::string TargetName;
	};

	UEventStream& operator>>(const HideActor& _Data)
	{
		EventTypeList.push_back(EEventType::HideActor);
		HideActorDataSet.push_back(_Data);
		return *this;
	}

	class ShowActor
	{
		friend UEventProcessor;
	public:
		ShowActor(std::string_view _TargetName)
			: TargetName(_TargetName)
		{
		}
	private:
		std::string TargetName;
	};

	UEventStream& operator>>(const ShowActor& _Data)
	{
		EventTypeList.push_back(EEventType::ShowActor);
		ShowActorDataSet.push_back(_Data);
		return *this;
	}

	class CameraFocus
	{
		friend UEventProcessor;
	public:
		CameraFocus(std::string_view _TargetName)
			: TargetName(_TargetName)
		{
		}
	private:
		std::string TargetName;
	};

	UEventStream& operator>>(const CameraFocus& _Data)
	{
		EventTypeList.push_back(EEventType::CameraFocus);
		CameraFocusDataSet.push_back(_Data);
		return *this;
	}

	class DeactivatePlayerControl
	{
		friend UEventProcessor;
	public:
		DeactivatePlayerControl()
		{
		}
	};

	UEventStream& operator>>(const DeactivatePlayerControl& _Data)
	{
		EventTypeList.push_back(EEventType::DeactivatePlayerControl);
		return *this;
	}

	class WildBattle
	{
		friend UEventProcessor;
	public:
		WildBattle(AWildBattleTrigger* _Wild) {}
	private:
		std::vector<UPokemon>* Entry;
	};

	UEventStream& operator>>(const WildBattle& _Data)
	{
		EventTypeList.push_back(EEventType::WildBattle);
		WildBattleDataSet.push_back(_Data);
		return *this;
	}

	class TrainerBattle
	{
		friend UEventProcessor;
	public:
		TrainerBattle(ATrainer* _Trainer)
			: Trainer(_Trainer)
		{}

		ATrainer* Trainer = nullptr;
	};

	UEventStream& operator>>(const TrainerBattle& _Data)
	{
		EventTypeList.push_back(EEventType::TrainerBattle);
		TrainerBattleDataSet.push_back(_Data);
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
	std::vector<ShowMapName> ShowMapNameDataSet;
	std::vector<ChangeLevel> ChangeLevelDataSet;
	std::vector<ChangePoint> ChangePointDataSet;
	std::vector<ChangePosition> ChangePositionDataSet;
	std::vector<ChangeDirection> ChangeDirectionDataSet;
	std::vector<StarePlayer> StarePlayerDataSet;
	std::vector<PlayAnimation> PlayAnimationDataSet;
	std::vector<PlayBgm> PlayBgmDataSet;
	std::vector<HideActor> HideActorDataSet;
	std::vector<ShowActor> ShowActorDataSet;
	std::vector<CameraFocus> CameraFocusDataSet;
	std::vector<WildBattle> WildBattleDataSet;
	std::vector<TrainerBattle> TrainerBattleDataSet;
};

