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
	PlayerActionSelectSM = SpawnActor<ABattlePlayerActionSelectStateMachine>();
	BattleTurnSM = SpawnActor<ABattleTurnStateMachine>();
	
	// BattleTurnSM ���� ���
	BattleActionSM = SpawnActor<ABattleActionStateMachine>();
	BattleEOTSM = SpawnActor<ABattleEOTStateMachine>();
	BattleFaintSM = SpawnActor<ABattleFaintStateMachine>();
	BattleTurnSM->SetBASM(BattleActionSM);
	BattleTurnSM->SetEOTSM(BattleEOTSM);
	BattleTurnSM->SetFSM(BattleFaintSM);
	
	// BattleActionSM ���� ���
	BattleMoveSM = SpawnActor<ABattleMoveStateMachine>();
	BattleActionSM->SetBMSM(BattleMoveSM);
	
	// BattleFaintSM ���� ���
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

	// ������ �ʱ�ȭ
	bool PlayerFirst = true;
	Player.Clear();
	Player.SetPlayer();
	Enemy.Clear();
	Enemy.SetWildPokemon();	// �ϴ� �߻� ���ϸ���� ������ ����
	
	// TODO: Player�� Faint ���°� �ƴ� ���ϸ��� ã�� ������ �ε��� ����

	Player.GetParticipants().push_back(Enemy.CurPokemon());
	Enemy.GetParticipants().push_back(Player.CurPokemon());
	
	Canvas->Init(&Player, &Enemy);

	// ��Ʋ ���� ���� �ʱ�ȭ
	State = EBattleState::BattleStart;
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
		// �÷��̾� �׼� ���� ���·� ���ư���.
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