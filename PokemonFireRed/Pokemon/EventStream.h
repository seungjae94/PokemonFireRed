#pragma once

#include <string>
#include <vector>
#include <list>
#include "PokemonMath.h"
#include "Text.h"
#include "PlayerData.h"

#define ES UEventStream

class UEventProcessor;
class ATrainer;
class AWildBattleTrigger;
class UPokemon;

enum class EEventType
{
	SetActive,
	Destroy,
	Move,
	MoveDynamicPath,
	MoveWithoutRestriction,
	Surprise,
	FadeIn,
	FadeOut,
	FadeInBgm,
	FadeOutBgm,
	Wait,
	Chat,
	ShowMapName,
	ChangeArea,
	ChangeLevel,
	ChangePoint,
	ChangePosition,
	ChangeDirection,
	StarePlayer,
	PlayAnimation,
	PlayBgm,
	PlaySE,
	PauseBgm,
	ResumeBgm,
	HideActor,
	ShowActor,
	CameraFocus,
	DeactivatePlayerControl,
	WildBattle,
	TrainerBattle,
	Achieve,
	Unachieve,
	GainItem,
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
	friend class UEventStreamReleaser;
public:
	UEventStream();
	~UEventStream();

private:
	class EventCommand
	{
		friend UEventProcessor;
	public:
		EventCommand(EEventType _EventType)
			: EventType(_EventType)
		{}

		virtual ~EventCommand() {}

		virtual EventCommand* Clone() const = 0;

	protected:
		EEventType EventType;
	};

public:
	UEventStream& operator>>(const EventCommand& _Data)
	{
		EventCommand* Data = _Data.Clone();
		EventCommands.push_back(Data);
		return *this;
	}

	class SetActive : public EventCommand
	{
		friend UEventProcessor;
	public:
		SetActive(std::string_view _MapName, std::string_view _TargetName, bool _Value)
			: MapName(_MapName), TargetName(_TargetName), Value(_Value), EventCommand(EEventType::SetActive)
		{
		}

		~SetActive() {}

		inline SetActive* Clone() const override 
		{ 
			return new SetActive(*this);
		}
	private:
		std::string MapName;
		std::string TargetName;
		bool Value = true;
	};

	class Destroy : public EventCommand
	{
		friend UEventProcessor;
	public:
		Destroy(AActor* _Actor)
			: Actor(_Actor), EventCommand(EEventType::Destroy)
		{
		}

		~Destroy() {}

		inline Destroy* Clone() const override
		{
			return new Destroy(*this);
		}
	private:
		AActor* Actor = nullptr;
	};

	class Move : public EventCommand
	{
		friend UEventProcessor;
	public:
		Move(std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true);

		Move(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true)
			: TargetNames(_TargetNames), Paths(_Paths), MoveSpeed(_MoveSpeed), CameraFollow(_CameraFollow), EventCommand(EEventType::Move)
		{
		}

		~Move() {}

		inline Move* Clone() const override
		{
			return new Move(*this);
		}
	private:
		std::vector<std::string> TargetNames;
		std::vector<std::vector<FTileVector>> Paths;
		bool CameraFollow = true;
		float MoveSpeed = 3.6f;
	};

	class MoveDynamicPath : public EventCommand
	{
		friend UEventProcessor;
	public:
		MoveDynamicPath(std::string_view _TargetName, std::vector<FTileVector>(*_Generator)(void), float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true)
			: TargetName(_TargetName), Generator(_Generator), MoveSpeed(_MoveSpeed), CameraFollow(_CameraFollow), EventCommand(EEventType::MoveDynamicPath)
		{
		}

		~MoveDynamicPath() {}

		inline MoveDynamicPath* Clone() const override
		{
			return new MoveDynamicPath(*this);
		}
	private:
		std::string TargetName;
		std::vector<FTileVector>(*Generator)(void);
		bool CameraFollow = true;
		float MoveSpeed = 3.6f;
	};

	class MoveWithoutRestriction : public EventCommand
	{
		friend UEventProcessor;
	public:
		MoveWithoutRestriction(std::string_view _TargetName, const std::vector<FVector>& _Path, float _MoveSpeed = 3.6f)
			: TargetName(_TargetName), Path(_Path), MoveSpeed(_MoveSpeed), EventCommand(EEventType::MoveWithoutRestriction)
		{
		}

		~MoveWithoutRestriction() {}

		inline MoveWithoutRestriction* Clone() const override
		{
			return new MoveWithoutRestriction(*this);
		}
	private:
		std::string TargetName;
		std::vector<FVector> Path;
		float MoveSpeed = 3.6f;
	};

	class Surprise : public EventCommand
	{
		friend UEventProcessor;
	public:
		Surprise(std::string_view _TargetName)
			: TargetName(_TargetName), EventCommand(EEventType::Surprise)
		{
		}

		~Surprise() {}

		inline Surprise* Clone() const override
		{
			return new Surprise(*this);
		}
	private:
		std::string TargetName;
	};

	class FadeIn : public EventCommand
	{
		friend UEventProcessor;
	public:
		FadeIn(float _Time, EFadeType _FadeType = EFadeType::Black)
			: Time(_Time), FadeType(_FadeType), EventCommand(EEventType::FadeIn)
		{
		}

		~FadeIn() {}

		inline FadeIn* Clone() const override
		{
			return new FadeIn(*this);
		}
	private:
		float Time = 0.0f;
		EFadeType FadeType = EFadeType::Black;
	};

	class FadeOut : public EventCommand
	{
		friend UEventProcessor;
	public:
		FadeOut(float _Time, EFadeType _FadeType = EFadeType::Black)
			: Time(_Time), FadeType(_FadeType), EventCommand(EEventType::FadeOut)
		{
		}

		~FadeOut() {}

		inline FadeOut* Clone() const override
		{
			return new FadeOut(*this);
		}
	private:
		float Time = 0.0f;
		EFadeType FadeType = EFadeType::Black;
	};

	class FadeInBgm : public EventCommand
	{
		friend UEventProcessor;
	public:
		FadeInBgm(float _Time, float _TargetVolume = 1.0f)
			: Time(_Time), TargetVolume(_TargetVolume), EventCommand(EEventType::FadeInBgm)
		{
		}

		~FadeInBgm() {}

		inline FadeInBgm* Clone() const override
		{
			return new FadeInBgm(*this);
		}
	private:
		float Time = 0.0f;
		float TargetVolume = 0.0f;
	};
	 
	class FadeOutBgm : public EventCommand
	{
		friend UEventProcessor;
	public:
		FadeOutBgm(float _Time)
			: Time(_Time), EventCommand(EEventType::FadeOutBgm)
		{
		}

		~FadeOutBgm() {}

		inline FadeOutBgm* Clone() const override
		{
			return new FadeOutBgm(*this);
		}
	private:
		float Time = 0.0f;
	};

	class Wait : public EventCommand
	{
		friend UEventProcessor;
	public:
		Wait(float _Time)
			: Time(_Time), 
			EventCommand(EEventType::Wait)
		{
		}

		~Wait() {}

		inline Wait* Clone() const override
		{
			return new Wait(*this);
		}
	private:
		float Time;
	};

	class PlayAnimation : public EventCommand
	{
		friend UEventProcessor;
	public:
		PlayAnimation(std::string_view _TargetName, std::string_view _AnimName, bool _Wait = true, EAnimTarget _AnimTarget = EAnimTarget::All)
			: TargetName(_TargetName), AnimName(_AnimName), Wait(_Wait), AnimTarget(_AnimTarget), EventCommand(EEventType::PlayAnimation)
		{
			if (_AnimName.ends_with(Global::SuffixLowerBody) || _AnimName.ends_with(Global::SuffixUpperBody))
			{
				MsgBoxAssert(AnimName + "은 유효하지 않습니다. PlayAnimation 이벤트에 애니메이션 이름을 적을 땐 LowerBody, UpperBody suffix를 생략해야 합니다.");
			}
		}

		~PlayAnimation() {}

		inline PlayAnimation* Clone() const override
		{
			return new PlayAnimation(*this);
		}
	private:
		std::string TargetName = "";
		std::string AnimName = "";
		bool Wait = true;
		EAnimTarget AnimTarget = EAnimTarget::All;
	};

	class PlayBgm : public EventCommand
	{
		friend UEventProcessor;
	public:
		PlayBgm(std::string_view _Name)
			: Name(_Name), EventCommand(EEventType::PlayBgm)
		{
		}

		~PlayBgm() {}

		inline PlayBgm* Clone() const override
		{
			return new PlayBgm(*this);
		}
	private:
		std::string Name = "";
	};

	class PlaySE : public EventCommand
	{
		friend UEventProcessor;
	public:
		PlaySE(std::string_view _Name, float _MuteTime = -1.0f)
			: Name(_Name), MuteTime(_MuteTime), EventCommand(EEventType::PlaySE)
		{
		}

		~PlaySE() {}

		inline PlaySE* Clone() const override
		{
			return new PlaySE(*this);
		}
	private:
		std::string Name = "";
		float MuteTime = -1.0f;
	};

	class PauseBgm : public EventCommand
	{
		friend UEventProcessor;
	public:
		PauseBgm()
			: EventCommand(EEventType::PauseBgm)
		{
		}

		~PauseBgm() {}

		inline PauseBgm* Clone() const override
		{
			return new PauseBgm(*this);
		}
	private:
	};

	class ResumeBgm : public EventCommand
	{
		friend UEventProcessor;
	public:
		ResumeBgm()
			: EventCommand(EEventType::ResumeBgm)
		{
		}

		~ResumeBgm() {}

		inline ResumeBgm* Clone() const override
		{
			return new ResumeBgm(*this);
		}
	private:
	};

	class Chat : public EventCommand
	{
		friend UEventProcessor;
	public:
		Chat(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14, bool _IsSequential = true)
			: Dialogue(_Dialogue), Color(_Color), LineSpace(_LineSpace), IsSequential(_IsSequential), EventCommand(EEventType::Chat)
		{
		}

		~Chat() {}

		inline Chat* Clone() const override
		{
			return new Chat(*this);
		}
	private:
		std::vector<std::wstring> Dialogue;
		EFontColor Color;
		int LineSpace = 14;
		bool IsSequential = true;
	};

	class ShowMapName : public EventCommand
	{
		friend UEventProcessor;
	public:
		ShowMapName(std::wstring_view _MapName)
			: MapName(_MapName), EventCommand(EEventType::ShowMapName)
		{
		}

		~ShowMapName() {}

		inline ShowMapName* Clone() const override
		{
			return new ShowMapName(*this);
		}
	private:
		std::wstring MapName;
	};

	class ChangeArea : public EventCommand
	{
		friend UEventProcessor;
	public:
		ChangeArea(std::string_view _AreaName, std::string_view _AreaBgm)
			: AreaName(_AreaName), AreaBgm(_AreaBgm), EventCommand(EEventType::ChangeArea)
		{
		}

		~ChangeArea() {}

		inline ChangeArea* Clone() const override
		{
			return new ChangeArea(*this);
		}
	private:
		std::string AreaName;
		std::string AreaBgm;
	};

	class ChangeLevel : public EventCommand
	{
		friend UEventProcessor;
	public:
		ChangeLevel(std::string_view _LevelName)
			: LevelName(_LevelName), EventCommand(EEventType::ChangeLevel)
		{
		}

		~ChangeLevel() {}

		inline ChangeLevel* Clone() const override
		{
			return new ChangeLevel(*this);
		}
	private:
		std::string LevelName;
	};

	class ChangePoint : public EventCommand
	{
		friend UEventProcessor;
	public:
		ChangePoint(std::string_view _LevelName,
			std::string_view _TargetName, const FTileVector& _Point)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Point(_Point),
			EventCommand(EEventType::ChangePoint)
		{
		}

		~ChangePoint() {}

		inline ChangePoint* Clone() const override
		{
			return new ChangePoint(*this);
		}
	private:
		std::string LevelName;
		std::string TargetName;
		FTileVector Point;
	};

	class ChangePosition : public EventCommand
	{
		friend UEventProcessor;
	public:
		ChangePosition(std::string_view _LevelName,
			std::string_view _TargetName, const FVector& _Position)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Position(_Position),
			EventCommand(EEventType::ChangePosition)
		{
		}

		~ChangePosition() {}

		inline ChangePosition* Clone() const override
		{
			return new ChangePosition(*this);
		}
	private:
		std::string LevelName;
		std::string TargetName;
		FVector Position;
	};

	class ChangeDirection : public EventCommand
	{
		friend UEventProcessor;
	public:
		ChangeDirection(std::string_view _LevelName,
			std::string_view _TargetName, const FTileVector& _Direction)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Direction(_Direction),
			EventCommand(EEventType::ChangeDirection)
		{
		}

		~ChangeDirection() {}

		inline ChangeDirection* Clone() const override
		{
			return new ChangeDirection(*this);
		}
	private:
		std::string LevelName;
		std::string TargetName;
		FTileVector Direction;
	};

	class StarePlayer : public EventCommand
	{
		friend UEventProcessor;
	public:
		StarePlayer(std::string_view _TargetName)
			: TargetName(_TargetName), EventCommand(EEventType::StarePlayer)
		{
		}

		~StarePlayer() {}

		inline StarePlayer* Clone() const override
		{
			return new StarePlayer(*this);
		}
	private:
		std::string TargetName;
	};

	class HideActor : public EventCommand
	{
		friend UEventProcessor;
	public:
		HideActor(std::string_view _TargetName)
			: TargetName(_TargetName), EventCommand(EEventType::HideActor)
		{
		}

		~HideActor() {}

		inline HideActor* Clone() const override
		{
			return new HideActor(*this);
		}
	private:
		std::string TargetName;
	};

	class ShowActor : public EventCommand
	{
		friend UEventProcessor;
	public:
		ShowActor(std::string_view _TargetName)
			: TargetName(_TargetName), EventCommand(EEventType::ShowActor)
		{
		}

		~ShowActor() {}

		inline ShowActor* Clone() const override
		{
			return new ShowActor(*this);
		}
	private:
		std::string TargetName;
	};

	class CameraFocus : public EventCommand
	{
		friend UEventProcessor;
	public:
		CameraFocus(std::string_view _TargetName)
			: TargetName(_TargetName), EventCommand(EEventType::CameraFocus)
		{
		}

		~CameraFocus() {}

		inline CameraFocus* Clone() const override
		{
			return new CameraFocus(*this);
		}
	private:
		std::string TargetName;
	};

	class DeactivatePlayerControl : public EventCommand
	{
		friend UEventProcessor;
	public:
		DeactivatePlayerControl()
			: EventCommand(EEventType::DeactivatePlayerControl)
		{
		}

		~DeactivatePlayerControl() {}

		inline DeactivatePlayerControl* Clone() const override
		{
			return new DeactivatePlayerControl(*this);
		}
	};

	class WildBattle : public EventCommand
	{
		friend UEventProcessor;
	public:
		WildBattle(AWildBattleTrigger* _Wild) : Entry(), EventCommand(EEventType::WildBattle) {}

		~WildBattle() {}

		inline WildBattle* Clone() const override
		{
			return new WildBattle(*this);
		}
	private:
		std::vector<UPokemon>* Entry;
	};

	class TrainerBattle : public EventCommand
	{
		friend UEventProcessor;
	public:
		TrainerBattle(ATrainer* _Trainer)
			: Trainer(_Trainer), EventCommand(EEventType::TrainerBattle)
		{}

		~TrainerBattle() {}

		inline TrainerBattle* Clone() const override
		{
			return new TrainerBattle(*this);
		}
	private:
		ATrainer* Trainer = nullptr;
	};

	class Achieve : public EventCommand
	{
		friend UEventProcessor;
	public:
		Achieve(EAchievement _Achievement)
			: Achievement(_Achievement), EventCommand(EEventType::Achieve)
		{}

		~Achieve() {}

		inline Achieve* Clone() const override
		{
			return new Achieve(*this);
		}
	private:
		EAchievement Achievement = EAchievement::None;
	};

	class Unachieve : public EventCommand
	{
		friend UEventProcessor;
	public:
		Unachieve(EAchievement _Achievement)
			: Achievement(_Achievement), EventCommand(EEventType::Unachieve)
		{}

		~Unachieve() {}

		inline Unachieve* Clone() const override
		{
			return new Unachieve(*this);
		}
	private:
		EAchievement Achievement = EAchievement::None;
	};

	class GainItem : public EventCommand
	{
		friend UEventProcessor;
	public:
		GainItem(EItemId _ItemId, int _Count)
			: ItemId(_ItemId), Count(_Count), EventCommand(EEventType::GainItem)
		{
		}

		~GainItem() {}

		inline GainItem* Clone() const override
		{
			return new GainItem(*this);
		}
	private:
		EItemId ItemId = EItemId::None;
		int Count = 0;
	};

	class End : public EventCommand
	{
		friend UEventProcessor;
	public:
		End(bool _ActivatePlayer)
			: ActivatePlayer(_ActivatePlayer), EventCommand(EEventType::End)
		{
		}

		~End() {}

		inline End* Clone() const override
		{
			return new End(*this);
		}
	private:
		bool ActivatePlayer = true;
	};

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
	std::vector<EventCommand*> EventCommands;
	bool DeactivatePlayer = true;
};

