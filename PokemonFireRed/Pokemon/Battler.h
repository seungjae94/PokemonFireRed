#pragma once
#include <vector>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "StatStage.h"

class UPlayerData;

class UBattler
{
public:
	// constructor destructor
	UBattler();
	~UBattler();

	// delete Function
	UBattler(const UBattler& _Other) = delete;
	UBattler(UBattler&& _Other) noexcept = delete;
	UBattler& operator=(const UBattler& _Other) = delete;
	UBattler& operator=(UBattler&& _Other) noexcept = delete;

	UStatStage StatStage;

	UPokemon* CurPokemon()
	{
		return Entry[CurPokemonIndex];
	}

	const UPokemon* CurPokemonReadonly() const
	{
		return Entry[CurPokemonIndex];
	}

	void Clear()
	{
		StatStage.Reset();
		Entry.clear();
		Action = EBattleAction::None;
		CurPokemonIndex = 0;
		CurMoveIndex = 0;
		IsPlayerValue = false;
		IsWildPokemonValue = false;
	}

	void SetPlayer();

	void SetWildPokemon();

	bool IsPlayer() const
	{
		return IsPlayerValue;
	}

	bool IsWildPokemon() const
	{
		return IsWildPokemonValue;
	}

	void SetAction(EBattleAction _Action)
	{
		Action = _Action;
	}

	void SetRunResult(bool _RunResult)
	{
		RunResult = _RunResult;
	}

	void SetMoveIndex(int _MoveIndex)
	{
		CurMoveIndex = _MoveIndex;
	}

	EPokemonMove CurMoveId() const
	{
		return CurPokemonReadonly()->GetMoveId(CurMoveIndex);
	}

	const FPokemonMove* CurMove() const
	{
		EPokemonMove MoveId = CurMoveId();
		return UPokemonDB::FindMove(MoveId);
	}

	EBattleAction CurAction() const
	{
		return Action;
	}

	bool IsRunSuccess() const
	{
		return RunResult;
	}

protected:

private:
	std::vector<UPokemon*> Entry;
	EBattleAction Action = EBattleAction::None;
	int CurPokemonIndex = 0;
	int CurMoveIndex = 0;
	bool IsPlayerValue = false;
	bool IsWildPokemonValue = false;
	bool RunResult = false;
};

