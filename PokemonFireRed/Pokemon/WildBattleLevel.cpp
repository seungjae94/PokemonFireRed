#include "WildBattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PlayerData.h"
#include "PokemonUtil.h"

UWildBattleLevel::UWildBattleLevel() 
{
}

UWildBattleLevel::~UWildBattleLevel() 
{
}

void UWildBattleLevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	// 배틀 레벨 리소스 로드
	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move(RN::BattleLevel);

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".bmp", ".png" }, false);
	for (UEngineFile& File : AllFiles)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	UEngineResourcesManager::GetInst().CuttingImage(RN::PlayerBattler, 5, 1);
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath(Global::ThrowedBall));

	// 액터 생성
	Canvas = SpawnActor<AWildBattleCanvas>();
}

void UWildBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EBattleState::BattleStart:
		ProcessBattleStart(_DeltaTime);
		break;
	case EBattleState::PlayerAction:
		break;
	case EBattleState::PlayerMove:
		break;
	case EBattleState::EnemyMove:
		break;
	case EBattleState::PlayerSecondaryEffect:
		break;
	case EBattleState::EnemySecondaryEffect:
		break;
	case EBattleState::BattleEnd:
		break;
	default:
		break;
	}
}

void UWildBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	EnemyPokemon = UPlayerData::EnemyWildPokemon;
	Canvas->Init(UPlayerData::GetPokemonInEntry(0), EnemyPokemon);

	// 배틀 레벨 상태 초기화
	State = EBattleState::BattleStart;
	BattleStartSubstate = EBattleStartSubstate::FadeWait;
	Timer = FadeWaitTime;
}

void UWildBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}

void UWildBattleLevel::ProcessBattleStart(float _DeltaTime)
{
	switch (BattleStartSubstate)
	{
	case EBattleStartSubstate::FadeWait:
		ProcessBattleStartFadeWait(_DeltaTime);
		break;
	case EBattleStartSubstate::GroundMove:
		ProcessBattleStartGroundMove(_DeltaTime);
		break;
	case EBattleStartSubstate::EnemyPokemonBoxMove:
		ProcessBattleStartEnemyPokemonBoxMove(_DeltaTime);
		break;
	case EBattleStartSubstate::ZClickWait:
		ProcessBattleStartZClickWait(_DeltaTime);
		break;
	case EBattleStartSubstate::PlayerBattlerThrow:
		ProcessBattleStartPlayerBattlerThrow(_DeltaTime);
		break;
	case EBattleStartSubstate::PlayerPokemonTakeout:
		ProcessBattleStartPlayerPokemonTakeout(_DeltaTime);
		break;
	case EBattleStartSubstate::PlayerPokemonBoxMove:
		ProcessBattleStartPlayerPokemonBoxMove(_DeltaTime);
		break;
	default:
		break;
	}
}

void UWildBattleLevel::ProcessBattleStartFadeWait(float _DeltaTime)
{
	if (Timer <= 0.0f)
	{
		Timer = GroundMoveTime;
		BattleStartSubstate = EBattleStartSubstate::GroundMove;
	}
}

void UWildBattleLevel::ProcessBattleStartGroundMove(float _DeltaTime)
{
	Canvas->LerpShowGrounds(Timer/GroundMoveTime);

	if (Timer <= 0.0f)
	{
		Timer = EnemyPokemonBoxMoveTime;
		BattleStartSubstate = EBattleStartSubstate::EnemyPokemonBoxMove;
	}
}

void UWildBattleLevel::ProcessBattleStartEnemyPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		BattleStartSubstate = EBattleStartSubstate::ZClickWait;
	}
}

void UWildBattleLevel::ProcessBattleStartZClickWait(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		Timer = PlayerBattleThrowTime;
		BattleStartSubstate = EBattleStartSubstate::PlayerBattlerThrow;
		Canvas->PlayBattlerThrowingAnimation();
		BallThrowAnimationPlayed = false;
	}
}

void UWildBattleLevel::ProcessBattleStartPlayerBattlerThrow(float _DeltaTime)
{
	Canvas->HidePlayerBattler(PlayerBattleThrowTime, _DeltaTime);

	if (Timer <= PlayerBattleThrowTime - BallThrowMotionWaitTime && false == BallThrowAnimationPlayed)
	{
		// 일정 시간이 지난 뒤 볼이 날아가는 애니메이션을 재생한다.
		Canvas->PlayThrowedBallAnimation();
		BallThrowAnimationPlayed = true;
	}

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonTakeoutTime;
		BattleStartSubstate = EBattleStartSubstate::PlayerPokemonTakeout;
	}
}

void UWildBattleLevel::ProcessBattleStartPlayerPokemonTakeout(float _DeltaTime)
{
	Canvas->TakeOutPokemonFromBall(Timer / PlayerPokemonTakeoutTime);

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonBoxMoveTime;
		BattleStartSubstate = EBattleStartSubstate::PlayerPokemonBoxMove;
	}
}

void UWildBattleLevel::ProcessBattleStartPlayerPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowPlayerPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = EBattleState::PlayerAction;
	}
}
