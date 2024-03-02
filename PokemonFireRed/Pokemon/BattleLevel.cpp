#include "BattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonUtil.h"
#include "SoundManager.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"

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
		ProcessPlayerMove(_DeltaTime);
		break;
	case EBattleState::EnemyMove:
		ProcessEnemyMove(_DeltaTime);
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

	// ��Ʋ ���� ���� �ʱ�ȭ
	State = EBattleState::BattleStart;
	PASM->Reset();

	// BSSM �������� ����
	BSSM->Start(Canvas, PlayerPokemon);
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

		switch (PlayerAction)
		{
		case EBattleAction::None:
			break;
		case EBattleAction::Fight:
		{
			EBattleAction EnemyAction = UBattleEnemyActionGenerator::Generate(EEnemyType::Wild, EnemyPokemon);
			int EnemyMoveIndex = UBattleEnemyActionGenerator::GetGeneratedMoveIndex();
			int PlayerMoveIndex = PASM->GetSelectedMoveIndex();
			PlayerFirst = UTurnOrderCalculator::IsPlayerFirst(PlayerAction, EnemyAction, PlayerPokemon, EnemyPokemon, PlayerMoveIndex, EnemyMoveIndex);

			if (true == PlayerFirst)
			{
				Canvas->SetBattleMessage(L"You are first.");
				State = EBattleState::PlayerMove;
			}
			else
			{
				Canvas->SetBattleMessage(L"Enemy is first.");
				State = EBattleState::EnemyMove;
			}
		}
			break;
		case EBattleAction::EscapeSuccess:
		{
			State = EBattleState::BattleEnd;
			ReturnToMapLevel();
		}
			break;
		case EBattleAction::EscapeFail:
		{
		// ������ ���� ƽ ������ �ٷ� PASM ƽ ���鼭 �Է��� ó���ϱ� ������ �Է��� 2�� ó���ȴ�.
		// �׷��� What will ... do? �ߴ� ƽ�� �ٷ� Run �׼��� ���õǸ鼭 ȭ�鿡 ��ȭ�� ���� ���̴� ���̴�.
		// �ӽ� ���� ���¶� �߻��ϴ� ���׷� ���� ������ ��ְ� �Ǵ� �� �ƴ�����
		// ����� ���� ����� ��ƽ���� map�� �Է� ���� Ű�� ����س��ٰ� �����ϴ� ������ ó���ؾ� �� �� ����.
			PlayerFirst = true;
			State = EBattleState::PlayerMove;
		}
			break;
		default:
			break;
		}
	}
}

void UBattleLevel::ProcessPlayerMove(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EBattleState::PlayerActionSelect;
		Canvas->SetActionBoxActive(true);
		Canvas->SetBattleMessage(L"What will\n" + GetCurPlayerPokemon().GetNameW() + L" do?");
		PASM->Start(Canvas, PlayerPokemon, EnemyPokemon);
	}
}

void UBattleLevel::ProcessEnemyMove(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EBattleState::PlayerActionSelect;
		Canvas->SetActionBoxActive(true);
		Canvas->SetBattleMessage(L"What will\n" + GetCurPlayerPokemon().GetNameW() + L" do?");
		PASM->Start(Canvas, PlayerPokemon, EnemyPokemon);
	}
}
