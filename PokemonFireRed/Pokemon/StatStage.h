#pragma once
#include <algorithm>
#include <EngineBase/EngineDebug.h>
#include "PokemonMath.h"

class UStatStage
{
public:
	// constructor destructor
	UStatStage();
	~UStatStage();

	// delete Function
	UStatStage(const UStatStage& _Other) = delete;
	UStatStage(UStatStage&& _Other) noexcept = delete;
	UStatStage& operator=(const UStatStage& _Other) = delete;
	UStatStage& operator=(UStatStage&& _Other) noexcept = delete;

	int GetAtk() const
	{
		return Atk;
	}

	float GetAtkMultiplier() const
	{
		int Numerator = UPokemonMath::Max(2, 2 + Atk);
		int Denominator = UPokemonMath::Max(2, 2 - Atk);
		return static_cast<float>(Numerator) / Denominator;
	}

	int GetDef() const
	{
		return Def;
	}

	float GetDefMultiplier() const
	{
		int Numerator = UPokemonMath::Max(2, 2 + Def);
		int Denominator = UPokemonMath::Max(2, 2 - Def);
		return static_cast<float>(Numerator) / Denominator;
	}

	int GetSpAtk() const
	{
		return SpAtk;
	}

	float GetSpAtkMultiplier() const
	{
		int Numerator = UPokemonMath::Max(2, 2 + SpAtk);
		int Denominator = UPokemonMath::Max(2, 2 - SpAtk);
		return static_cast<float>(Numerator) / Denominator;
	}

	int GetSpDef() const
	{
		return SpDef;
	}

	float GetSpDefMultiplier() const
	{
		int Numerator = UPokemonMath::Max(2, 2 + SpDef);
		int Denominator = UPokemonMath::Max(2, 2 - SpDef);
		return static_cast<float>(Numerator) / Denominator;
	}

	int GetSpeed() const
	{
		return Speed;
	}

	float GetSpeedMultiplier() const
	{
		int Numerator = UPokemonMath::Max(2, 2 + Speed);
		int Denominator = UPokemonMath::Max(2, 2 - Speed);
		return static_cast<float>(Numerator) / Denominator;
	}

	int GetAccuracy() const
	{
		return Accuracy;
	}


	int GetEvasion() const
	{
		return Evasion;
	}

	float GetAccuracyMultiplier(int _EnemyEvasion) const
	{
		int ActualAccuracy = Accuracy - _EnemyEvasion;
		ActualAccuracy = UPokemonMath::Cap(ActualAccuracy, -6, 6);

		int Numerator = UPokemonMath::Max(3, 3 + ActualAccuracy);
		int Denominator = UPokemonMath::Max(3, 3 - ActualAccuracy);
		return static_cast<float>(Numerator) / Denominator;
	}

	int GetCriticalHit() const
	{
		return CriticalHit;
	}

	float GetCriticalHitMultiplier() const
	{
		switch (CriticalHit)
		{
		case 0:
			return 1.0f / 16;
		case 1:
			return 1.0f / 8;
		case 2:
			return 1.0f / 4;
		case 3:
			return 1.0f / 3;
		case 4:
			return 1.0f / 2;
		default:
			MsgBoxAssert("Critical Hit stage 값이 0 이상 4 이하의 범위를 벗어납니다.");
			return 0.0f;
		}
	}

	void AddAtk(int _Stage)
	{
		Atk += _Stage;
		Atk = UPokemonMath::Cap(Atk, -6, 6);
	}

	void AddDef(int _Stage)
	{
		Def += _Stage;
		Def = UPokemonMath::Cap(Def, -6, 6);
	}

	void AddSpAtk(int _Stage)
	{
		SpAtk += _Stage;
		SpAtk = UPokemonMath::Cap(SpAtk, -6, 6);
	}

	void AddSpDef(int _Stage)
	{
		SpDef += _Stage;
		SpDef = UPokemonMath::Cap(SpDef, -6, 6);
	}

	void AddSpeed(int _Stage)
	{
		Speed += _Stage;
		Speed = UPokemonMath::Cap(Speed, -6, 6);
	}

	void AddAccuracy(int _Stage)
	{
		Accuracy += _Stage;
		Accuracy = UPokemonMath::Cap(Accuracy, -6, 6);
	}

	void AddEvasion(int _Stage)
	{
		Evasion += _Stage;
		Evasion = UPokemonMath::Cap(Evasion, -6, 6);
	}

	void AddCriticalHit(int _Stage)
	{
		if (_Stage < 0)
		{
			MsgBoxAssert("크리티컬 확률은 감소할 수 없습니다.");
			return;
		}

		CriticalHit += _Stage;
		CriticalHit = UPokemonMath::Cap(CriticalHit, 0, 4);
	}

	void Reset()
	{
		Atk = 0;
		Def = 0;
		SpAtk = 0;
		SpDef = 0;
		Speed = 0;
		Accuracy = 0;
		Evasion = 0;
		CriticalHit = 0;
	}

protected:

private:
	int Atk = 0;
	int Def = 0;
	int SpAtk = 0;
	int SpDef = 0;
	int Speed = 0;
	int Accuracy = 0;
	int Evasion = 0;
	int CriticalHit = 0;
};

