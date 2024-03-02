#pragma once

enum class ETypeVs
{
	Win,
	Draw,
	Lose,
};

struct FDamageResult
{
public:
	int Damage = 0;
	bool IsCritical = false;
	ETypeVs TypeVs = ETypeVs::Draw;
};

class UDamageCalculator
{
public:
	// constructor destructor
	UDamageCalculator();
	~UDamageCalculator();

	// delete Function
	UDamageCalculator(const UDamageCalculator& _Other) = delete;
	UDamageCalculator(UDamageCalculator&& _Other) noexcept = delete;
	UDamageCalculator& operator=(const UDamageCalculator& _Other) = delete;
	UDamageCalculator& operator=(UDamageCalculator&& _Other) noexcept = delete;

	static FDamageResult CalcDamage();

protected:

private:

};

