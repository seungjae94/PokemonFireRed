#include "BattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonUtil.h"
#include "SoundManager.h"

UBattleLevel::UBattleLevel() 
{
}

UBattleLevel::~UBattleLevel() 
{
}

void UBattleLevel::BeginPlay()
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
	Canvas = SpawnActor<ABattleCanvas>();
	BSSM = SpawnActor<ABattleStartStateMachine>();
	PASM = SpawnActor<ABattlePlayerActionSelectStateMachine>();
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
	case EBattleState::PlayerActionSelect:
		ProcessPlayerAction(_DeltaTime);
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
	PrevMapName = _PrevLevel->GetName();

	PlayerPokemonIndex = 0;
	PlayerPokemon = &UPlayerData::GetPokemonInEntry(0);
	EnemyPokemon = UPlayerData::GetEnemyWildPokemon();
	Canvas->Init(PlayerPokemon, EnemyPokemon);

	// 배틀 레벨 상태 초기화
	State = EBattleState::BattleStart;
	PASM->Reset();

	// BSSM 로직부터 시작
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
		State = EBattleState::PlayerActionSelect;
		Canvas->SetActionBoxActive(true);
		Canvas->SetBattleMessage(L"What will\n" + GetCurPlayerPokemon().GetNameW() + L" do?");
		PASM->Start(Canvas, PlayerPokemon, EnemyPokemon);
	}
}

void UBattleLevel::ProcessPlayerAction(float _DeltaTime)
{
	if (true == PASM->IsEnd())
	{
		PlayerAction = PASM->GetPlayerActionResult();
		// 액션 결과에 따라 행동...
		switch (PlayerAction)
		{
		case EBattlePlayerAction::None:
			break;
		case EBattlePlayerAction::Fight:
			State = EBattleState::PlayerMove;
			Canvas->SetBattleMessage(L"You Selected To Fight.");
			break;
		case EBattlePlayerAction::EscapeSuccess:
			State = EBattleState::BattleEnd;
			ReturnToMapLevel();
			break;
		case EBattlePlayerAction::EscapeFail:
			State = EBattleState::PlayerMove;
			break;
		default:
			break;
		}
	}
}
