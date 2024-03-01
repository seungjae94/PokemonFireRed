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

		switch (PlayerAction)
		{
		case EBattlePlayerAction::None:
			break;
		case EBattlePlayerAction::Fight:
		{
			EBattleEnemyAction EnemyAction = UBattleEnemyActionGenerator::Generate(EEnemyType::Wild, EnemyPokemon);
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
		case EBattlePlayerAction::EscapeSuccess:
		{
			State = EBattleState::BattleEnd;
			ReturnToMapLevel();
		}
			break;
		case EBattlePlayerAction::EscapeFail:
		{
		// 지금은 레벨 틱 다음에 바로 PASM 틱 돌면서 입력을 처리하기 때문에 입력이 2번 처리된다.
		// 그래서 What will ... do? 뜨는 틱에 바로 Run 액션이 선택되면서 화면에 변화가 없어 보이는 것이다.
		// 임시 구현 상태라 발생하는 버그로 실제 구현에 장애가 되는 건 아니지만
		// 비슷한 문제 생기면 매틱마다 map에 입력 받은 키를 기억해놨다가 무시하는 식으로 처리해야 할 것 같다.
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
