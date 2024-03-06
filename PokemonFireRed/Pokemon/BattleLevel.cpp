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
	BattleStartSM = SpawnActor<ABattleStartStateMachine>();
	PlayerActionSelectSM = SpawnActor<ABattlePlayerActionSelectStateMachine>();
	BattleTurnSM = SpawnActor<ABattleTurnStateMachine>();
	
	// BattleTurnSM 하위 요소
	BattleActionSM = SpawnActor<ABattleActionStateMachine>();
	BattleEOTSM = SpawnActor<ABattleEOTStateMachine>();
	BattleFaintSM = SpawnActor<ABattleFaintStateMachine>();
	BattleTurnSM->SetBASM(BattleActionSM);
	BattleTurnSM->SetEOTSM(BattleEOTSM);
	BattleTurnSM->SetFSM(BattleFaintSM);
	
	// BattleActionSM 하위 요소
	BattleMoveSM = SpawnActor<ABattleMoveStateMachine>();
	BattleActionSM->SetBMSM(BattleMoveSM);
	
	// BattleFaintSM 하위 요소
	BattleExpGainSM = SpawnActor<ABattleExpGainStateMachine>();
	BattleFaintSM->SetEGSM(BattleExpGainSM);
}

void UBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EBattleState::BattleStart:
		ProcessBattleStart();
		break;
	case EBattleState::PlayerActionSelect:
		ProcessPlayerAction();
		break;
	case EBattleState::Turn:
		ProcessTurn();
		break;
	case EBattleState::BattleEnd:
		ProcessBattleEnd();
		break;
	case EBattleState::Run:
		ProcessRun();
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
	
	// TODO: Player의 Faint 상태가 아닌 포켓몬을 찾을 때까지 인덱스 증가

	Player.GetParticipants().push_back(Enemy.CurPokemon());
	Enemy.GetParticipants().push_back(Player.CurPokemon());
	
	Canvas->Init(&Player, &Enemy);

	// 배틀 레벨 상태 초기화
	State = EBattleState::BattleStart;
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
		State = EBattleState::PlayerActionSelect;
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
		{
			State = EBattleState::Turn;
			BattleTurnSM->Start(Canvas, &Player, &Enemy);
		}
		break;
		case EBattleAction::Escape:
		{
			bool RunResult = Player.GetRunResult();
			if (true == RunResult)
			{
				State = EBattleState::Run;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Got away safely!");
			}
			else
			{
				State = EBattleState::Turn;
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
		// 플레이어 액션 선택 상태로 돌아간다.
		if (BattleTurnSM->WhyEnd() == ABattleTurnStateMachine::EEndReason::None)
		{
			State = EBattleState::PlayerActionSelect;
			Canvas->SetActionBoxActive(true);
			Canvas->SetBattleMessage(L"What will\n" + Player.CurPokemon()->GetNameW() + L" do?");
			PlayerActionSelectSM->Start(Canvas, &Player, &Enemy);
		}
		else if (BattleTurnSM->WhyEnd() == ABattleTurnStateMachine::EEndReason::WinToWild)
		{
			State = EBattleState::BattleEnd;
		}
		else
		{
			State = EBattleState::BattleEnd;
		}
	}
}

void UBattleLevel::ProcessBattleEnd()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ReturnToMapLevel();
	}
}

void UBattleLevel::ProcessRun()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ReturnToMapLevel();
	}
}