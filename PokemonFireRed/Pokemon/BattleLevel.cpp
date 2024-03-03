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

	// ������ �ʱ�ȭ
	bool PlayerFirst = true;
	Player.Clear();
	Player.SetPlayer();
	Enemy.Clear();
	Enemy.SetWildPokemon();	// �ϴ� �߻� ���ϸ���� ������ ����
	Canvas->Init(&Player, &Enemy);

	// ��Ʋ ���� ���� �ʱ�ȭ
	State = EBattleState::BattleStart;
	PASM->Reset();

	// BSSM �������� ����
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
		// TODO: ���� ���� ������ ���� �޴´�. (�÷��̾� �¸�, �÷��̾� �й� ��)
		
		// ��Ʋ�� ������� �ʾҴٸ� �÷��̾� �׼� ���� ���·� ���ư���.
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