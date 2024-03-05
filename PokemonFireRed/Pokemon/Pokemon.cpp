#include "Pokemon.h"
#include <random>
#include "PokemonMath.h"
#include "ExpCalculator.h"

UPokemon::UPokemon(EPokedexNo _Id, int _Level)
	: Level(_Level)
{
	Species = UPokemonDB::FindSpecies(_Id);
	Name = UPokemonString::ToUpperW(Species->Name);
	Level = _Level;
	AccExp = UExpCalculator::GetNeedAccExp(this, _Level);
	InitMoves();
	InitRandomIVs();
	InitRandomGender();
	InitRandomNature();
	InitRandomAbility();
	Status = UPokemonDB::FindStatus(EPokemonStatus::None);
	CurHp = GetHp();
	for (int i = 0; i < GetMoveCount(); ++i)
	{
		MoveCurPPs.push_back(Moves[i]->PP);
	}
}

UPokemon::~UPokemon()
{
}

FLevelUpData UPokemon::LevelUp()
{
	FLevelUpData Data;

	int PrevHp = GetHp();
	int PrevAtk = GetAtk();
	int PrevDef = GetAtk();
	int PrevSpAtk = GetAtk();
	int PrevSpDef = GetAtk();
	int PrevSpeed = GetAtk();

	++Level;
	
	Data.UpHp = GetHp() - PrevHp;
	Data.UpAtk = GetAtk() - PrevAtk;
	Data.UpDef = GetDef() - PrevDef;
	Data.UpSpAtk = GetSpAtk() - PrevSpAtk;
	Data.UpSpDef = GetSpDef() - PrevSpDef;
	Data.UpSpeed = GetSpeed() - PrevSpeed;

	if (true == Species->LevelUpMoves.contains(Level))
	{
		Data.Moves = Species->LevelUpMoves.at(Level);
	}

	CurHp += Data.UpHp;

	return Data;
}

std::string UPokemon::GetTypeImageName(int _Index) const
{
	if (_Index < 0 || _Index >= GetTypeCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 타입 범위를 벗어납니다");
		return "";
	}

	EPokemonType TypeId = Species->TypeIds[_Index];
	return UPokemonDB::FindType(TypeId)->ImageName;
}

std::wstring UPokemon::GetMoveNameW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다");
		return L"";
	}

	return Moves[_Index]->GetNameW();
}

std::wstring UPokemon::GetMovePPW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다");
		return L"";
	}

	return std::to_wstring(Moves[_Index]->PP);
}

std::wstring UPokemon::GetMoveCurPPW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다");
		return L"";
	}

	return std::to_wstring(MoveCurPPs[_Index]);
}

std::wstring UPokemon::GetMoveTypeW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다.");
		return L"";
	}

	const FPokemonType* Type = UPokemonDB::FindType(Moves[_Index]->TypeId);
	return Type->GetNameW();
}

std::wstring UPokemon::GetMoveTypeUpperW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다.");
		return L"";
	}

	const FPokemonType* Type = UPokemonDB::FindType(Moves[_Index]->TypeId);
	return UPokemonString::ToUpperW(Type->Name);
}

std::string UPokemon::GetMoveTypeImageName(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다.");
		return "";
	}

	const FPokemonType* Type = UPokemonDB::FindType(Moves[_Index]->TypeId);
	return Type->ImageName;
}

std::wstring UPokemon::GetMovePowerW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다");
		return L"";
	}

	return std::to_wstring(Moves[_Index]->BasePower);
}

std::wstring UPokemon::GetMoveAccuracyW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다");
		return L"";
	}

	return std::to_wstring(Moves[_Index]->Accuracy);
}

std::wstring UPokemon::GetMoveExplainW(int _Index) const
{
	if (_Index < 0 || _Index >= GetMoveCount())
	{
		MsgBoxAssert("인덱스 " + std::to_string(_Index) + "가 보유한 Move 범위를 벗어납니다");
		return L"";
	}

	return Moves[_Index]->GetExplainW();
}

void UPokemon::LearnMove(EPokemonMove _MoveId)
{
	if (Moves.size() == 4)
	{
		MsgBoxAssert("기술을 4개 배운 상태에서 기술을 추가하려고 했습니다.");
	}

	const FPokemonMove* Move = UPokemonDB::FindMove(_MoveId);
	Moves.push_back(Move);
}

void UPokemon::Heal(int _Value)
{
	if (_Value < 0)
	{
		MsgBoxAssert("음수만큼 HP를 회복할 수 없습니다.");
		return;
	}

	CurHp += _Value;
	if (CurHp > GetHp())
	{
		CurHp = GetHp();
	}
}

int UPokemon::GetHp() const
{
	return (2 * Species->BHp + IHp + EHp / 4) * Level / 100 + Level + 10;
}

int UPokemon::GetAtk() const
{
	return UPokemonMath::Floor(((2 * Species->BAtk + IAtk + EAtk / 4) * Level / 100 + 5) * Nature->NAtk);
}

int UPokemon::GetDef() const
{
	return UPokemonMath::Floor(((2 * Species->BDef + IDef + EDef / 4) * Level / 100 + 5) * Nature->NDef);
}

int UPokemon::GetSpAtk() const
{
	return UPokemonMath::Floor(((2 * Species->BSpAtk + ISpAtk + ESpAtk / 4) * Level / 100 + 5) * Nature->NSpAtk);
}

int UPokemon::GetSpDef() const
{
	return UPokemonMath::Floor(((2 * Species->BSpDef + ISpDef + ESpDef / 4) * Level / 100 + 5) * Nature->NSpDef);
}

int UPokemon::GetSpeed() const
{
	return UPokemonMath::Floor(((2 * Species->BSpeed + ISpeed + ESpeed / 4) * Level / 100 + 5) * Nature->NSpeed);
}

void UPokemon::AddAccExp(int _Exp)
{
	if (Level >= 100)
	{
		return;
	}

	AccExp += _Exp;
}

int UPokemon::GetExpSize() const
{
	if (Level >= 100 || Level <= 0)
	{
		return 0;
	}

	return UExpCalculator::GetNeedAccExp(this, Level + 1) - UExpCalculator::GetNeedAccExp(this, Level);
}

int UPokemon::GetNextLevelExp() const
{
	if (Level >= 100 || Level <= 0)
	{
		return 0;
	}

	return UExpCalculator::GetNeedAccExp(this, Level + 1) - AccExp;
}

int UPokemon::GetExp() const
{
	if (Level >= 100 || Level <= 0)
	{
		return 0;
	}

	return GetExpSize() - GetNextLevelExp();
}

void UPokemon::InitMoves()
{
	for (std::pair<const int, std::list<EPokemonMove>> Pair : Species->LevelUpMoves)
	{
		int LearnLevel = Pair.first;

		// 더이상 배울 수 있는 스킬이 없다.
		if (LearnLevel > Level)
		{
			break;
		}

		std::list<EPokemonMove>& MoveIds = Pair.second;

		for (EPokemonMove MoveId : MoveIds)
		{
			const FPokemonMove* Move = UPokemonDB::FindMove(MoveId);

			if (Moves.size() == 4)
			{
				// 가장 앞에 있는 Move를 지운다.
				Moves.erase(Moves.begin());

			}
			Moves.push_back(Move);
		}

	}

	if (Moves.size() == 0 || Moves.size() > 4)
	{
		MsgBoxAssert("포켓몬 기술 초기화가 잘못되었습니다. Moves의 크기가 0이거나 4 초과입니다.");
		return;
	}
}

void UPokemon::InitRandomIVs()
{
	IHp = UPokemonMath::RandomInt(0, 31);
	IAtk = UPokemonMath::RandomInt(0, 31);
	IDef = UPokemonMath::RandomInt(0, 31);
	ISpAtk = UPokemonMath::RandomInt(0, 31);
	ISpDef = UPokemonMath::RandomInt(0, 31);
	ISpeed = UPokemonMath::RandomInt(0, 31);
}

void UPokemon::InitRandomGender()
{
	float GenderInt = UPokemonMath::Random(0.0f, 1.0f);
	float MaleRatio = Species->MaleRatio;

	if (GenderInt <= MaleRatio)
	{
		Gender = UPokemonDB::FindGender(EPokemonGender::Male);
	}
	else
	{
		Gender = UPokemonDB::FindGender(EPokemonGender::Female);
	}
}

void UPokemon::InitRandomNature()
{
	int NatureInt = UPokemonMath::RandomInt(1, static_cast<int>(EPokemonNature::MAX) - 1);
	EPokemonNature NatureId = static_cast<EPokemonNature>(NatureInt);
	Nature = UPokemonDB::FindNature(NatureId);
}

void UPokemon::InitRandomAbility()
{
	int AbilitySize = static_cast<int>(Species->AbilityCandidateIds.size());
	int AbilityInt = UPokemonMath::RandomInt(0, AbilitySize - 1);
	EPokemonAbility AbilityId = Species->AbilityCandidateIds[AbilityInt];
	Ability = UPokemonDB::FindAbility(AbilityId);
}