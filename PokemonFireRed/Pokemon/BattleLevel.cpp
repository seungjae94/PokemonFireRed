#include "BattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PlayerData.h"
#include "PokemonUtil.h"

UBattleLevel::UBattleLevel() 
{
}

UBattleLevel::~UBattleLevel() 
{
}

void UBattleLevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	// ��Ʋ ���� ���ҽ� �ε�
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

	// ���� ����
	Canvas = SpawnActor<ABattleCanvas>();
	BSSM = SpawnActor<ABattleStartStateManchine>();
}

void UBattleLevel::Tick(float _DeltaTime)
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

void UBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	EnemyPokemon = UPlayerData::EnemyWildPokemon;
	Canvas->Init(UPlayerData::GetPokemonInEntry(0), EnemyPokemon);

	// ��Ʋ ���� ���� �ʱ�ȭ
	State = EBattleState::BattleStart;
	BSSM->Start(Canvas);
}

void UBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}

void UBattleLevel::ProcessBattleStart(float _DeltaTime)
{
	if (true == BSSM->IsEnd())
	{
		State = EBattleState::PlayerAction;
	}
}