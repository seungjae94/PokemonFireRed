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
	UEngineResourcesManager::GetInst().CuttingImage(RN::BattleEnemyGroundBall, 3, 1);
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath(Global::ThrowedBall));

	// 이펙트 로드
	CurDir.Move("Effect");
	std::list<UEngineFile> AllEffectFiles = CurDir.AllFile({ ".bmp", ".png" }, false);
	for (UEngineFile& File : AllEffectFiles)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	// 액터 생성
	Canvas = SpawnActor<ABattleCanvas>();
	AnimatorGenerator = SpawnActor<AAnimatorGenerator>();
	MsgBox = SpawnActor<APokemonMsgBox>();
	MsgBox->SetBaseRenderingOrder(ERenderingOrder::UI4);
	MsgBox->SetBackgroundImage(RN::BattleMsgBox);
	MsgBox->SetBackgroundLeftBotRelativePos(UPokemonUtil::PixelVector(0, 0));
	MsgBox->SetCoverImage(RN::BattleMsgBoxCover);
	MsgBox->SetTextColor(EFontColor::White);
	MsgBox->SetTextLeftTopRelativePos(UPokemonUtil::PixelVector(11, 21));
	MsgBox->SetLineSpace(16);

	WildBattleStartSM = SpawnActor<AWildBattleStartStateMachine>();
	TrainerBattleStartSM = SpawnActor<ATrainerBattleStartStateMachine>();
	BattlePrepareTurnSM = SpawnActor<ABattlePrepareTurnStateMachine>();
	PlayerActionSelectSM = SpawnActor<ABattlePlayerActionSelectStateMachine>();
	BattleTurnSM = SpawnActor<ABattleTurnStateMachine>();
	FinishBattleSM = SpawnActor<AFinishBattleStateMachine>();
	
	// 하위 요소
	BattleActionSM = SpawnActor<ABattleActionStateMachine>();
	BattleEOTSM = SpawnActor<ABattleEOTStateMachine>();
	BattleFaintSM = SpawnActor<ABattleFaintStateMachine>();
	BattleMoveSM = SpawnActor<ABattleMoveStateMachine>();
	BattlePlayerShiftSM = SpawnActor<ABattlePlayerShiftStateMachine>();
	BattleEnemyShiftSM = SpawnActor<ABattleEnemyShiftStateMachine>();
	BattleExpGainSM = SpawnActor<ABattleExpGainStateMachine>();

	BattleTurnSM->SetBASM(BattleActionSM);
	BattleTurnSM->SetEOTSM(BattleEOTSM);
	BattleTurnSM->SetFSM(BattleFaintSM);
	BattleActionSM->SetBMSM(BattleMoveSM);
	BattleActionSM->SetBPSSM(BattlePlayerShiftSM);
	BattleFaintSM->SetEGSM(BattleExpGainSM);
	BattlePrepareTurnSM->SetBPSSM(BattlePlayerShiftSM);
	BattlePrepareTurnSM->SetBESSM(BattleEnemyShiftSM);
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
	case EState::PrepareTurn1:
		ProcessPrepareTurn1();
		break;
	case EState::PrepareTurn2:
		ProcessPrepareTurn2();
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
	case EState::WaitBeforeReturn:
		ProcessWaitBeforeReturn();
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
	Player.InitPlayer();
	Player.InitCurPokemon(); // 기절하지 않은 첫 번째 포켓몬을 내보낸다.
	Enemy.Clear();

	if (true == UEventManager::IsWildPokemonBattle())
	{
		Enemy.InitWildPokemon();
	}
	else
	{
		Enemy.InitTrainer();
	}
	Enemy.InitCurPokemon();

	Player.GetParticipants().push_back(Enemy.CurPokemon());
	Enemy.GetParticipants().push_back(Player.CurPokemon());
	
	Canvas->Init(&Player, &Enemy);
	MsgBox->Init();

	// 배틀 레벨 상태 초기화
	State = EState::BattleStart;
	PlayerActionSelectSM->Reset();

	// BSSM 로직부터 시작
	if (true == Enemy.IsWildPokemon())
	{
		WildBattleStartSM->Start(Canvas, MsgBox, &Player, &Enemy);
	}
	else
	{
		TrainerBattleStartSM->Start();
	}
}

void UBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}


void UBattleLevel::ProcessBattleStart()
{
	bool ProcessEndCondition = false;
	if (true == Enemy.IsWildPokemon())
	{
		ProcessEndCondition = WildBattleStartSM->IsEnd();
	}
	else
	{
		ProcessEndCondition = TrainerBattleStartSM->IsEnd();
	}

	if (true == ProcessEndCondition)
	{
		State = EState::PrepareTurn1;
		BattlePrepareTurnSM->Start(Canvas, MsgBox, &Player, &Enemy);
	}
}

void UBattleLevel::ProcessPrepareTurn1()
{
	if (true == BattlePrepareTurnSM->IsEnd())
	{
		State = EState::PrepareTurn2;

		MsgBox->SetWriteSpeed(2.0f);
		MsgBox->SetMessage(L"What will\n" + Player.CurPokemon()->GetNameW() + L" do?");
		MsgBox->Write();
	}
}

void UBattleLevel::ProcessPrepareTurn2()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = EState::PlayerActionSelect;
		Canvas->SetActionBoxActive(true);
		MsgBox->SetWriteSpeed(1.0f);
		PlayerActionSelectSM->Start(Canvas, MsgBox, &Player, &Enemy);
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
			MsgBox->SetMessage(L"");
			BattleTurnSM->Start(Canvas, MsgBox, &Player, &Enemy);
		}
		break;
		case EBattleAction::Escape:
		{
			bool RunResult = Player.GetRunResult();
			if (true == RunResult)
			{
				State = EState::Run;
				Canvas->SetActionBoxActive(false);
				MsgBox->SetMessage(L"Got away safely!");
				MsgBox->Write();
			}
			else
			{
				State = EState::Turn;
				BattleTurnSM->Start(Canvas, MsgBox, &Player, &Enemy);
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
		// 배틀이 종료되지 않은 경우 턴 준비 단계로 다시 돌아간다.
		BattleEndReason = BattleTurnSM->WhyEnd();

		if (BattleTurnSM->WhyEnd() == EBattleEndReason::None)
		{
			State = EState::PrepareTurn1;
			BattlePrepareTurnSM->Start(Canvas, MsgBox, &Player, &Enemy);
			return;
		}

		State = EState::FinishBattle;
		FinishBattleSM->Start(Canvas, MsgBox, &Player, &Enemy, BattleEndReason);
	}
}

void UBattleLevel::ProcessFinishBattle()
{
	if (true == FinishBattleSM->IsEnd())
	{
		State = EState::WaitBeforeReturn;
		Timer = 0.0f;

		if (BattleEndReason == EBattleEndReason::WinToWild)
		{
			Timer = WaitBeforeReturnTime;
		}
	}
}

void UBattleLevel::ProcessWaitBeforeReturn()
{
	if (Timer <= 0.0f)
	{
		if (BattleEndReason == EBattleEndReason::WinToWild)
		{
			ReturnToMapLevel();
		}
		else if (BattleEndReason == EBattleEndReason::WinToTrainer)
		{
			AEventTrigger* AfterBattleTrigger = Enemy.GetAfterBattleTrigger();
			if (nullptr == AfterBattleTrigger)
			{
				ReturnToMapLevel();
			}
			else
			{
				UEventManager::TriggerEvent(AfterBattleTrigger, EEventTriggerAction::Direct);
				State = EState::End;
			}
		}
		else
		{
			// TODO: 가장 마지막에 방문한 포켓몬 센터로 복귀
			ReturnToMapLevel();
		}
	}
}

void UBattleLevel::ProcessRun()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ReturnToMapLevel();
	}
}