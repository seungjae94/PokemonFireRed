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
	BattleStartSM = SpawnActor<ABattleStartStateMachine>();
	BattlePrepareTurnSM = SpawnActor<ABattlePrepareTurnStateMachine>();
	PlayerActionSelectSM = SpawnActor<ABattlePlayerActionSelectStateMachine>();
	BattleTurnSM = SpawnActor<ABattleTurnStateMachine>();
	
	// ���� ���
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
		// ���ϸ� �޴� ���� �� �����ϴ� ��� ���� �ʱ�ȭ�� ���� �ʴ´�.
		return;
	}

	if (nullptr != MapLevel)
	{
		// ���� ������ �� ������ ��� �� ���� �̸��� ����Ѵ�.
		PrevMapName = _PrevLevel->GetName();
	}

	// ������ �ʱ�ȭ
	bool PlayerFirst = true;
	Player.Clear();
	Player.SetPlayer();
	Player.InitCurPokemon(); // �������� ���� ù ��° ���ϸ��� ��������.
	Enemy.Clear();
	Enemy.SetWildPokemon();	 // �ϴ� �߻� ���ϸ���� ������ ����
	Enemy.InitCurPokemon();

	Player.GetParticipants().push_back(Enemy.CurPokemon());
	Enemy.GetParticipants().push_back(Player.CurPokemon());
	
	Canvas->Init(&Player, &Enemy);

	// ��Ʋ ���� ���� �ʱ�ȭ
	State = EState::BattleStart;
	PlayerActionSelectSM->Reset();

	// BSSM �������� ����
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
		// �� �غ� �ܰ�� �ٽ� ���ư���.
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