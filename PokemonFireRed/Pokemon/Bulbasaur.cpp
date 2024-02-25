#include "Bulbasaur.h"

UBulbasaur::UBulbasaur()
{
	// 기본 정보 설정
	PokedexNo = EPokedexNo::Bulbasaur;
	Name = L"BULBASAUR";
	Level = 1;
	AccExp = 0;
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);

	// 능력치 설정
	BHp = 45;
	BAtk = 49;
	BDef = 49;
	BSpAtk = 65;
	BSpDef = 65;
	BSpeed = 45;
	SetRandomIVs();
	SetRandomNature();
	SetRandomAbility({ EAbility::Overgrow });
	CurHp = GetHp();

	// 종족 관련 설정
	YSpAtk = 1;
	CatchRate = 45;
	Friendship = 70;
	ExpGroup = EExperienceGroup::MediumSlow;
	Types.push_back(EPokemonType::Grass);
	Types.push_back(EPokemonType::Poison);
}

UBulbasaur::~UBulbasaur() 
{
}

