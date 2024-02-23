#include "Bulbasaur.h"

UBulbasaur::UBulbasaur()
{
	// 기본 정보 설정
	EPokedexNo PokedexNo = EPokedexNo::Bulbasaur;
	Name = L"BULBASAUR";
	Level = 1;
	AccExp = 0;

	// 기본 데이터 설정
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);
	SetRandomNature();
	SetRandomAbility({EAbility::Overgrow});

	// 능력치 설정
	BHp = 45;
	BAtk = 49;
	BDef = 49;
	BSpAtk = 65;
	BSpDef = 65;
	BSpeed = 45;
	SetRandomIVs();

	// 종족 능력치 설정
	YSpAtk = 1;
	CatchRate = 45;
	Friendship = 70;

	Types.push_back(EPokemonType::Grass);
	Types.push_back(EPokemonType::Poison);
}

UBulbasaur::~UBulbasaur() 
{
}

