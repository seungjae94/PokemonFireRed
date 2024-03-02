#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"

class UBattleLevel;
class ABattleCanvas;
 
class ABattleTurnStateMachine : public AActor
{
private:
	/*
	* DecideTurnOrder: 적의 액션을 생성하고, 누가 먼저 행동할지 결정한다. 
	*	-> PreparePlayerTurn (플레이어가 먼저 행동할 경우 PreparePlayerTurn 상태로 전환한다.)
	*	-> PrepareEnemyTurn (적이 먼저 행동할 경우 PrepareEnemyTurn 상태로 전환한다.)
	* 
	* PlayerTurnStart: 플레이어 액션에 따라 다음 상태를 결정한다.
	*	-> ShowEscapeFailMsg (Run 액션을 고른 경우)
	*	-> PlayerSwitch (Switch 액션을 고른 경우)
	*	-> PlayerUseItem (Item 액션을 고른 경우)
	*	-> PreparePlayerFight (Fight 액션을 고른 경우)
	* 
	* PlayerEscapeFail: 플레이어 입력을 대기한다.
						 플레이어 턴이 끝난 것으로 취급한다.
	*	-> EnemySwitch (적이 Switch 액션을 고른 경우)
	*	-> EnemyUseItem (적이 Item 액션을 고른 경우)
	*	-> PrepareEnemyFight (적이 Fight 액션을 고른 경우)
	* 
	* PlayerSwitch: BattleLevel로 부터 받은 데이터를 기반으로 
					포켓몬 교체 처리를 하고 포켓몬 교체 애니메이션을 재생한다.
					플레이어 턴이 끝난 것으로 취급한다.
	*	-> ShowEscapeFailMsg의 NextState와 동일
	* 
	* PlayerUseItem: BattleLevel로 부터 받은 데이터를 기반으로
					아이템 사용 처리를 하고 아이템 사용 애니메이션을 재생한다.
					플레이어 턴이 끝난 것으로 취급한다.
	*	-> ShowEscapeFailMsg의 NextState와 동일
	* 
	* PreparePlayerFight: 정확도 계산을 통해 다음 상태를 결정한다.
		-> PlayerMoveFail
		-> PlayerMove
	*
	* PlayerMove: 크리티컬, 데미지 계산을 통해 상대에게 데미지를 입히고 
				스킬 이펙트 연출 처리를 한다.
		-> PlayerMoveSecondaryEffect (부가 효과가 있을 경우)
		-> PrepareEnemyTurn (플레이어가 선공인 경우)
		-> PrepareEndOfTurn (플레이어가 후공인 경우)
	*/
	enum class ESubstate
	{
		None,
		PlayerEscapeFail,
		PlayerSwitch,
		PlayerUseItem,
		PlayerMoveFail,
		PlayerMove,
		PlayerMoveSecondaryEffect,
		EnemySwitch,
		EnemyUseItem,
		EnemyMoveFail,
		EnemyMove,
		EnemyMoveSecondaryEffect,
		PlayerEndOfTurn,
		EnemyEndOfTurn,
		End,
	};
public:
	// constructor destructor
	ABattleTurnStateMachine();
	~ABattleTurnStateMachine();

	// delete Function
	ABattleTurnStateMachine(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine(ABattleTurnStateMachine&& _Other) noexcept = delete;
	ABattleTurnStateMachine& operator=(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine& operator=(ABattleTurnStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(
		ABattleCanvas* _Canvas, 
		UPokemon* _PlayerPokemon, UPokemon* _EnemyPokemon, 
		UStatStage& _PlayerStatStage, UStatStage& _EnemyStatStage, 
		EBattleAction _PlayerAction, int _PlayerMoveIndex
	);

	void Reset() {};

protected:

private:
	// 배틀 레벨 데이터
	ABattleCanvas* Canvas = nullptr;
	UPokemon* PlayerPokemon = nullptr; 
	UPokemon* EnemyPokemon = nullptr;
	UStatStage* PlayerStatStage = nullptr; 
	UStatStage* EnemyStatStage = nullptr;
	EBattleAction PlayerAction = EBattleAction::None;
	EBattleAction EnemyAction = EBattleAction::None;
	EPokemonMove PlayerMoveId = EPokemonMove::None;
	EPokemonMove EnemyMoveId = EPokemonMove::None;

	// 고유 데이터
	ESubstate State = ESubstate::None;
	const float MoveFailMsgShowTime = 1.5f;
	float Timer = 0.0f;
	bool IsPlayerFirst = true;
	bool IsFirstTurnEnd = false;
	
	// FDamageResult { int Damage; bool IsCritical; ETypeVersus (SuperEffective||None||NotVeryEffective); }


	// 로직
	void Tick(float _DeltaTime) override;

	void GenerateEnemyAction();
	void DispatchPlayerTurn();
	void DispatchPlayerFight();
	void DispatchEnemyTurn();
	void DispatchEnemyFight();
};

