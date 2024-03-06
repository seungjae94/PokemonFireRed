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
#include "MapLevel.h"
#include "PokemonUILevel.h"

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
	BattleStartSM = SpawnActor<ABattleStartStateMachine>();
	BattlePrepareTurnSM = SpawnActor<ABattlePrepareTurnStateMachine>();
	PlayerActionSelectSM = SpawnActor<ABattlePlayerActionSelectStateMachine>();
	BattleTurnSM = SpawnActor<ABattleTurnStateMachine>();
	
	// 하위 요소
	BattleActionSM = SpawnActor<ABattleActionStateMachine>();
	BattleEOTSM = SpawnActor<ABattleEOTStateMachine>();
	BattleFaintSM = SpawnActor<ABattleFaintStateMachine>();
	BattleMoveSM = SpawnActor<ABattleMoveStateMachine>();
	BattlePlayerShiftSM = SpawnActor<ABattlePlayerShiftStateMachine>();
	BattleExpGainSM = SpawnActor<ABattleExpGainStateMachine>();

	BattleTurnSM->SetBASM(BattleActionSM);
	BattleTurnSM->SetEOTSM(BattleEOTSM);
	BattleTurnSM->SetFSM(BattleFaintSM);
	BattleActionSM->SetBMSM(BattleMoveSM);
	BattleActionSM->SetBPSSM(BattlePlayerShiftSM);
	BattleFaintSM->SetEGSM(BattleExpGainSM);
	BattlePrepareTurnSM->SetBPSSM(BattlePlayerShiftSM);
}

void UBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::BattleStart:
		ProcessBattleStart();
		break;
	case EState::PrepareTurn:
		ProcessPrepareTurn();
		break;
	case EState::PlayerActionSelect:
		ProcessPlayerAction();
		break;
	case EState::Turn:
		ProcessTurn();
		break;
	case EState::FinishBattle:
		ProcessFinishBattle();
		break;
	case EState::Run:
		ProcessRun();
		break;
	default:
		break;
	}
}

void UBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);
	UPokemonUILevel* PokemonUILevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);

	if (nullptr != PokemonUILevel)
	{
		// 포켓몬 메뉴 진입 후 복귀하는 경우 레벨 초기화를 하지 않는다.
		return;
	}

	if (nullptr != MapLevel)
	{
		// 이전 레벨이 맵 레벨인 경우 맵 레벨 이름을 기억한다.
		PrevMapName = _PrevLevel->GetName();
	}

	// 데이터 초기화
	bool PlayerFirst = true;
	Player.Clear();
	Player.SetPlayer();
	Player.InitCurPokemon(); // 기절하지 않은 첫 번째 포켓몬을 내보낸다.
	Enemy.Clear();
	Enemy.SetWildPokemon();	 // 일단 야생 포켓몬과의 전투만 구현
	Enemy.InitCurPokemon();

	Player.GetParticipants().push_back(Enemy.CurPokemon());
	Enemy.GetParticipants().push_back(Player.CurPokemon());
	
	Canvas->Init(&Player, &Enemy);

	// 배틀 레벨 상태 초기화
	State = EState::BattleStart;
	PlayerActionSelectSM->Reset();

	// BSSM 로직부터 시작
	BattleStartSM->Start(Canvas, &Player);
}

void UBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}


void UBattleLevel::ProcessBattleStart()
{
	if (true == BattleStartSM->IsEnd())
	{
		State = EState::PrepareTurn;
		BattlePrepareTurnSM->Start(Canvas, &Player, &Enemy);
	}
}

void UBattleLevel::ProcessPrepareTurn()
{
	if (true == BattlePrepareTurnSM->IsEnd())
	{
		State = EState::PlayerActionSelect;
		Canvas->SetActionBoxActive(true);
		Canvas->SetBattleMessage(L"What will\n" + Player.CurPokemon()->GetNameW() + L" do?");
		PlayerActionSelectSM->Start(Canvas, &Player, &Enemy);
	}
}

void UBattleLevel::ProcessPlayerAction()
{
	if (true == PlayerActionSelectSM->IsEnd())
	{
		switch (Player.CurAction())
		{
		case EBattleAction::None:
			break;
		case EBattleAction::Fight:
		case EBattleAction::Shift:
		{
			State = EState::Turn;
			BattleTurnSM->Start(Canvas, &Player, &Enemy);
		}
		break;
		case EBattleAction::Escape:
		{
			bool RunResult = Player.GetRunResult();
			if (true == RunResult)
			{
				State = EState::Run;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Got away safely!");
			}
			else
			{
				State = EState::Turn;
				BattleTurnSM->Start(Canvas, &Player, &Enemy);
			}
		}
		break;
		default:
			break;
		}
	}
}

void UBattleLevel::ProcessTurn()
{
	if (true == BattleTurnSM->IsEnd())
	{
		// 턴 준비 단계로 다시 돌아간다.
		if (BattleTurnSM->WhyEnd() == ABattleTurnStateMachine::EEndReason::None)
		{
			State = EState::PrepareTurn;
			BattlePrepareTurnSM->Start(Canvas, &Player, &Enemy);
		}
		else if (BattleTurnSM->WhyEnd() == ABattleTurnStateMachine::EEndReason::WinToWild)
		{
			State = EState::FinishBattle;
		}
		else
		{
			State = EState::FinishBattle;
		}
	}
}

void UBattleLevel::ProcessFinishBattle()
{
	ReturnToMapLevel();
}

void UBattleLevel::ProcessRun()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ReturnToMapLevel();
	}
}