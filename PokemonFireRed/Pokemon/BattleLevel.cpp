#include "BattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonUtil.h"
#include "SoundManager.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"

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
	BTSM = SpawnActor<ABattleTurnStateMachine>();
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
	case EBattleState::Turn:
		ProcessTurn(_DeltaTime);
		break;
	case EBattleState::BattleWin:
		break;
	case EBattleState::BattleLose:
		break;
	case EBattleState::Run:
		ProcessRun(_DeltaTime);
		break;
	default:
		break;
	}
}

void UBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
	PrevMapName = _PrevLevel->GetName();

	// 데이터 초기화
	bool PlayerFirst = true;
	Player.Clear();
	Player.SetPlayer();
	Enemy.Clear();
	Enemy.SetWildPokemon();	// 일단 야생 포켓몬과의 전투만 구현
	Canvas->Init(&Player, &Enemy);

	// 배틀 레벨 상태 초기화
	State = EBattleState::BattleStart;
	PASM->Reset();

	// BSSM 로직부터 시작
	BSSM->Start(Canvas, &Player);
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
		Canvas->SetBattleMessage(L"What will\n" + Player.CurPokemon()->GetNameW() + L" do?");
		PASM->Start(Canvas, &Player, &Enemy);
	}
}

void UBattleLevel::ProcessPlayerAction(float _DeltaTime)
{
	if (true == PASM->IsEnd())
	{
		switch (Player.CurAction())
		{
		case EBattleAction::None:
			break;
		case EBattleAction::Fight:
		{
			State = EBattleState::Turn;
			BTSM->Start(Canvas, &Player, &Enemy);
		}
		break;
		case EBattleAction::Escape:
		{
			bool RunResult = Player.IsRunSuccess();
			if (true == RunResult)
			{
				State = EBattleState::Run;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Got away safely!");
			}
			else
			{
				State = EBattleState::Turn;
				BTSM->Start(Canvas, &Player, &Enemy);
			}
		}
		break;
		default:
			break;
		}
	}
}

void UBattleLevel::ProcessTurn(float _DeltaTime)
{
	if (true == BTSM->IsEnd())
	{
		// TODO: 턴이 끝난 이유를 보고 받는다. (플레이어 승리, 플레이어 패배 등)
		
		// 배틀이 종료되지 않았다면 플레이어 액션 선택 상태로 돌아간다.
		State = EBattleState::PlayerActionSelect;
		Canvas->SetActionBoxActive(true);
		Canvas->SetBattleMessage(L"What will\n" + Player.CurPokemon()->GetNameW() + L" do?");
		PASM->Start(Canvas, &Player, &Enemy);
	}
}

void UBattleLevel::ProcessBattleWin(float _DeltaTime)
{
}

void UBattleLevel::ProcessBattleLose(float _DeltaTime)
{
}

void UBattleLevel::ProcessRun(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ReturnToMapLevel();
	}
}