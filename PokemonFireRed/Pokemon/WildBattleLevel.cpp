#include "WildBattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
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
	UPokemonUtil::LoadAllResourcesIn(RN::BattleLevel);

	UEngineResourcesManager::GetInst().CuttingImage(RN::PlayerBattler, 5, 1);

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
	case EBattleStartSubstate::SendOutFirstPokemon:
		ProcessBattleStartSendOutFirstPokemon(_DeltaTime);
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
	Canvas->LerpGrounds(Timer/GroundMoveTime);

	if (Timer < 0.0f)
	{
		Timer = EnemyPokemonBoxMoveTime;
		BattleStartSubstate = EBattleStartSubstate::EnemyPokemonBoxMove;
	}
}

void UWildBattleLevel::ProcessBattleStartEnemyPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer < 0.0f)
	{
		Timer = PlayerPokemonBoxMoveTime;
		BattleStartSubstate = EBattleStartSubstate::ZClickWait;
	}
}

void UWildBattleLevel::ProcessBattleStartZClickWait(float _DeltaTime)
{
}

void UWildBattleLevel::ProcessBattleStartSendOutFirstPokemon(float _DeltaTime)
{
}

void UWildBattleLevel::ProcessBattleStartPlayerPokemonBoxMove(float _DeltaTime)
{
}
