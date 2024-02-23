#include "Squirtle.h"

USquirtle::USquirtle() 
{
	// 기본 정보 설정
	PokedexNo = EPokedexNo::Squirtle;
	Name = L"SQUIRTLE";
	Level = 1;
	AccExp = 0;

	// 기본 데이터 설정
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);
	SetRandomNature();
	SetRandomAbility({ EAbility::Torrent });

	// 능력치 설정
	BHp = 44;
	BAtk = 48;
	BDef = 65;
	BSpAtk = 50;
	BSpDef = 64;
	BSpeed = 43;
	SetRandomIVs();

	// 종족 능력치 설정
	YDef = 1;
	CatchRate = 45;
	Friendship = 70;

	Types.push_back(EPokemonType::Water);
}

USquirtle::~USquirtle() 
{
}

