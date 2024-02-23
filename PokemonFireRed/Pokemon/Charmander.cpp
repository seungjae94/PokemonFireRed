#include "Charmander.h"

UCharmander::UCharmander() 
{
	// 기본 정보 설정
	Level = 1;
	AccExp = 0;
	Name = L"CHARMANDER";

	// 기본 데이터 설정
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);
	SetRandomNature();
	SetRandomAbility({ EAbility::Blaze });

	// 능력치 설정
	BHp = 39;
	BAtk = 52;
	BDef = 43;
	BSpAtk = 60;
	BSpDef = 50;
	BSpeed = 65;
	SetRandomIVs();

	// 종족 능력치 설정
	YSpeed = 1;
	CatchRate = 45;
	Friendship = 70;

	Types.push_back(EPokemonType::Fire);
}

UCharmander::~UCharmander() 
{
}

