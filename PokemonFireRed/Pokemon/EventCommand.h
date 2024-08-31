#pragma once
#include "EventEnums.h"
#include <string>
#include <type_traits>
#include <vector>
#include "PokemonMath.h"
#include "EventCommand_ProcessingData.h"
#include "PlayerData.h"
#include "Text.h"

class UEventProcessor;
class UEventStream;
class ATrainer;
class AWildBattleTrigger;
class Pokemon;

class UEventCommand
{
	friend UEventProcessor;
public:
	// constructor destructor
	UEventCommand(EEventType _EventType)
		: EventType(_EventType)
	{}

	virtual ~UEventCommand() {}

private:
	virtual bool Process(float _DeltaTime) = 0;

	EEventType EventType;
};

namespace ES
{
	class SetActive : public UEventCommand
	{
	public:
		SetActive(std::string_view _MapName, std::string_view _TargetName, bool _Value)
			: UEventCommand(EEventType::SetActive), 
			MapName(_MapName), TargetName(_TargetName), Value(_Value)
		{
		}

		~SetActive() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string MapName;
		std::string TargetName;
		bool Value = true;
	};

	class Destroy : public UEventCommand
	{
		friend UEventProcessor;
	public:
		Destroy(AActor* _Actor)
			: UEventCommand(EEventType::Destroy), Actor(_Actor)
		{
		}

		~Destroy() {}
	private:
		bool Process(float _DeltaTime) override;

		AActor* Actor = nullptr;
	};

	class MoveBase : public UEventCommand
	{
	public:
		MoveBase(EEventType _Type, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true)
			: UEventCommand(_Type), CameraFollow(_CameraFollow), MoveSpeed(_MoveSpeed)
		{}

		~MoveBase()
		{
		}

	protected:
		bool MoveStart(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed);
		bool MoveUpdate(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed, bool _CameraFollow, float _DeltaTime);

		bool CameraFollow = true;
		float MoveSpeed = 3.6f;

	private:
		virtual bool Process(float _DeltaTime) = 0;
	};

	class Move : public MoveBase
	{
		friend UEventProcessor;
	public:
		Move(std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true);

		Move(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true)
			: MoveBase(EEventType::Move, _MoveSpeed, _CameraFollow), 
			TargetNames(_TargetNames), Paths(_Paths)
		{
		}

		~Move() {}
	private:
		bool Process(float _DeltaTime) override;

		std::vector<std::string> TargetNames;
		std::vector<std::vector<FTileVector>> Paths;

	};

	class MoveDynamicPath : public MoveBase
	{
		friend UEventProcessor;
	public:
		MoveDynamicPath(std::string_view _TargetName, std::vector<FTileVector>(*_Generator)(void), float _MoveSpeed = Global::CharacterWalkSpeed, bool _CameraFollow = true)
			: MoveBase(EEventType::MoveDynamicPath, _MoveSpeed, _CameraFollow), 
			TargetName(_TargetName), Generator(_Generator)
		{
		}

		~MoveDynamicPath() 
		{
		}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName;
		std::vector<FTileVector>(*Generator)(void);
	};

	class MoveWithoutRestriction : public UEventCommand
	{
		friend UEventProcessor;
	public:
		MoveWithoutRestriction(std::string_view _TargetName, const std::vector<FVector>& _Path, float _MoveSpeed = 3.6f)
			: UEventCommand(EEventType::MoveWithoutRestriction), 
			TargetName(_TargetName), Path(_Path), MoveSpeed(_MoveSpeed)
		{
		}

		~MoveWithoutRestriction() {}
	private:
		bool Process(float _DeltaTime) override;
		void ProcessEnd(AEventTarget* _Target);

		std::string TargetName;
		std::vector<FVector> Path;
		float MoveSpeed = 3.6f;

	private:
		FMoveWRData* MoveWRData = nullptr;
	};

	class Surprise : public UEventCommand
	{
		friend UEventProcessor;
	public:
		Surprise(std::string_view _TargetName)
			: UEventCommand(EEventType::Surprise), TargetName(_TargetName)
		{
		}

		~Surprise() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName;
	};

	class FadeIn : public UEventCommand
	{
		friend UEventProcessor;
	public:
		FadeIn(float _Time, EFadeType _FadeType = EFadeType::Black)
			: UEventCommand(EEventType::FadeIn), Time(_Time), FadeType(_FadeType)
		{
		}

		~FadeIn() {}
	private:
		bool Process(float _DeltaTime) override;

		float Time = 0.0f;
		EFadeType FadeType = EFadeType::Black;
	};

	class FadeOut : public UEventCommand
	{
		friend UEventProcessor;
	public:
		FadeOut(float _Time, EFadeType _FadeType = EFadeType::Black)
			: UEventCommand(EEventType::FadeOut), Time(_Time), FadeType(_FadeType)
		{
		}

		~FadeOut() {}
	private:
		bool Process(float _DeltaTime) override;

		float Time = 0.0f;
		EFadeType FadeType = EFadeType::Black;
	};

	class FadeInBgm : public UEventCommand
	{
		friend UEventProcessor;
	public:
		FadeInBgm(float _Time, float _TargetVolume = 1.0f)
			: UEventCommand(EEventType::FadeInBgm), Time(_Time), TargetVolume(_TargetVolume)
		{
		}

		~FadeInBgm() {}
	private:
		bool Process(float _DeltaTime) override;

		float Time = 0.0f;
		float TargetVolume = 0.0f;
	};

	class FadeOutBgm : public UEventCommand
	{
		friend UEventProcessor;
	public:
		FadeOutBgm(float _Time)
			: UEventCommand(EEventType::FadeOutBgm), Time(_Time)
		{
		}

		~FadeOutBgm() {}
	private:
		bool Process(float _DeltaTime) override;

		float Time = 0.0f;
	};

	class Wait : public UEventCommand
	{
		friend UEventProcessor;
	public:
		Wait(float _Time)
			: UEventCommand(EEventType::Wait), Time(_Time)
		{
		}

		~Wait() {}
	private:
		bool Process(float _DeltaTime) override;

		float Time;
	};

	class PlayAnimation : public UEventCommand
	{
		friend UEventProcessor;
	public:
		PlayAnimation(std::string_view _TargetName, std::string_view _AnimName, bool _Wait = true, EAnimTarget _AnimTarget = EAnimTarget::All)
			: TargetName(_TargetName), AnimName(_AnimName), Wait(_Wait), AnimTarget(_AnimTarget), UEventCommand(EEventType::PlayAnimation)
		{
			if (_AnimName.ends_with(Global::SuffixLowerBody) || _AnimName.ends_with(Global::SuffixUpperBody))
			{
				MsgBoxAssert(AnimName + "은 유효하지 않습니다. PlayAnimation 이벤트에 애니메이션 이름을 적을 땐 LowerBody, UpperBody suffix를 생략해야 합니다.");
			}
		}

		~PlayAnimation() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName = "";
		std::string AnimName = "";
		bool Wait = true;
		EAnimTarget AnimTarget = EAnimTarget::All;
	};

	class PlayBgm : public UEventCommand
	{
		friend UEventProcessor;
	public:
		PlayBgm(std::string_view _Name)
			: Name(_Name), UEventCommand(EEventType::PlayBgm)
		{
		}

		~PlayBgm() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string Name = "";
	};

	class PlaySE : public UEventCommand
	{
		friend UEventProcessor;
	public:
		PlaySE(std::string_view _Name, float _MuteTime = -1.0f)
			: Name(_Name), MuteTime(_MuteTime), UEventCommand(EEventType::PlaySE)
		{
		}

		~PlaySE() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string Name = "";
		float MuteTime = -1.0f;
	};

	class PauseBgm : public UEventCommand
	{
		friend UEventProcessor;
	public:
		PauseBgm()
			: UEventCommand(EEventType::PauseBgm)
		{
		}

		~PauseBgm() {}
	private:
		bool Process(float _DeltaTime) override;
	};

	class ResumeBgm : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ResumeBgm()
			: UEventCommand(EEventType::ResumeBgm)
		{
		}

		~ResumeBgm() {}
	private:
		bool Process(float _DeltaTime) override;
	};

	class Chat : public UEventCommand
	{
		friend UEventProcessor;
	public:
		Chat(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14, bool _IsSequential = true)
			: Dialogue(_Dialogue), Color(_Color), LineSpace(_LineSpace), IsSequential(_IsSequential), UEventCommand(EEventType::Chat)
		{
		}

		~Chat() {}
	private:
		bool Process(float _DeltaTime) override;

		std::vector<std::wstring> Dialogue;
		EFontColor Color;
		int LineSpace = 14;
		bool IsSequential = true;
	};

	class ShowMapName : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ShowMapName(std::wstring_view _MapName)
			: MapName(_MapName), UEventCommand(EEventType::ShowMapName)
		{
		}

		~ShowMapName() {}
	private:
		bool Process(float _DeltaTime) override;

		std::wstring MapName;
	};

	class ChangeArea : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ChangeArea(std::string_view _AreaName, std::string_view _AreaBgm)
			: AreaName(_AreaName), AreaBgm(_AreaBgm), UEventCommand(EEventType::ChangeArea)
		{
		}

		~ChangeArea() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string AreaName;
		std::string AreaBgm;
	};

	class ChangeLevel : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ChangeLevel(std::string_view _LevelName)
			: LevelName(_LevelName), UEventCommand(EEventType::ChangeLevel)
		{
		}

		~ChangeLevel() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string LevelName;
	};

	class ChangePoint : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ChangePoint(std::string_view _LevelName,
			std::string_view _TargetName, const FTileVector& _Point)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Point(_Point),
			UEventCommand(EEventType::ChangePoint)
		{
		}

		~ChangePoint() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string LevelName;
		std::string TargetName;
		FTileVector Point;
	};

	class ChangePosition : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ChangePosition(std::string_view _LevelName,
			std::string_view _TargetName, const FVector& _Position)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Position(_Position),
			UEventCommand(EEventType::ChangePosition)
		{
		}

		~ChangePosition() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string LevelName;
		std::string TargetName;
		FVector Position;
	};

	class ChangeDirection : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ChangeDirection(std::string_view _LevelName,
			std::string_view _TargetName, const FTileVector& _Direction)
			: LevelName(_LevelName),
			TargetName(_TargetName),
			Direction(_Direction),
			UEventCommand(EEventType::ChangeDirection)
		{
		}

		~ChangeDirection() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string LevelName;
		std::string TargetName;
		FTileVector Direction;
	};

	class StarePlayer : public UEventCommand
	{
		friend UEventProcessor;
	public:
		StarePlayer(std::string_view _TargetName)
			: TargetName(_TargetName), UEventCommand(EEventType::StarePlayer)
		{
		}

		~StarePlayer() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName;
	};

	class HideActor : public UEventCommand
	{
		friend UEventProcessor;
	public:
		HideActor(std::string_view _TargetName)
			: TargetName(_TargetName), UEventCommand(EEventType::HideActor)
		{
		}

		~HideActor() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName;
	};

	class ShowActor : public UEventCommand
	{
		friend UEventProcessor;
	public:
		ShowActor(std::string_view _TargetName)
			: TargetName(_TargetName), UEventCommand(EEventType::ShowActor)
		{
		}

		~ShowActor() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName;
	};

	class CameraFocus : public UEventCommand
	{
		friend UEventProcessor;
	public:
		CameraFocus(std::string_view _TargetName)
			: TargetName(_TargetName), UEventCommand(EEventType::CameraFocus)
		{
		}

		~CameraFocus() {}
	private:
		bool Process(float _DeltaTime) override;

		std::string TargetName;
	};

	class DeactivatePlayerControl : public UEventCommand
	{
		friend UEventProcessor;
	public:
		DeactivatePlayerControl()
			: UEventCommand(EEventType::DeactivatePlayerControl)
		{
		}

		~DeactivatePlayerControl() {}
	private:
		bool Process(float _DeltaTime) override;
	};

	class WildBattle : public UEventCommand
	{
		friend UEventProcessor;
	public:
		WildBattle(AWildBattleTrigger* _Wild) : Entry(), UEventCommand(EEventType::WildBattle) {}

		~WildBattle() {}
	private:
		bool Process(float _DeltaTime) override;

		std::vector<UPokemon>* Entry;
	};

	class TrainerBattle : public UEventCommand
	{
		friend UEventProcessor;
	public:
		TrainerBattle(ATrainer* _Trainer)
			: Trainer(_Trainer), UEventCommand(EEventType::TrainerBattle)
		{}

		~TrainerBattle() {}
	private:
		bool Process(float _DeltaTime) override;

		ATrainer* Trainer = nullptr;
	};

	class Achieve : public UEventCommand
	{
		friend UEventProcessor;
	public:
		Achieve(EAchievement _Achievement)
			: Achievement(_Achievement), UEventCommand(EEventType::Achieve)
		{}

		~Achieve() {}
	private:
		bool Process(float _DeltaTime) override;

		EAchievement Achievement = EAchievement::None;
	};

	class Unachieve : public UEventCommand
	{
		friend UEventProcessor;
	public:
		Unachieve(EAchievement _Achievement)
			: Achievement(_Achievement), UEventCommand(EEventType::Unachieve)
		{}

		~Unachieve() {}
	private:
		bool Process(float _DeltaTime) override;

		EAchievement Achievement = EAchievement::None;
	};

	class GainItem : public UEventCommand
	{
		friend UEventProcessor;
	public:
		GainItem(EItemId _ItemId, int _Count)
			: ItemId(_ItemId), Count(_Count), UEventCommand(EEventType::GainItem)
		{
		}

		~GainItem() {}
	private:
		bool Process(float _DeltaTime) override;

		EItemId ItemId = EItemId::None;
		int Count = 0;
	};

	class End : public UEventCommand
	{
		friend UEventProcessor;
	public:
		End(bool _ActivatePlayer)
			: ActivatePlayer(_ActivatePlayer), UEventCommand(EEventType::End)
		{
		}

		~End() {}
	private:
		bool Process(float _DeltaTime) override;

		bool ActivatePlayer = true;
	};
}
