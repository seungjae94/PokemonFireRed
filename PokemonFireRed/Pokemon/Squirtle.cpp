#include "Squirtle.h"

USquirtle::USquirtle() 
{
	// �⺻ ���� ����
	PokedexNo = EPokedexNo::Squirtle;
	Name = L"SQUIRTLE";
	Level = 1;
	AccExp = 0;
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);

	// �ɷ�ġ ����
	BHp = 44;
	BAtk = 48;
	BDef = 65;
	BSpAtk = 50;
	BSpDef = 64;
	BSpeed = 43;
	SetRandomIVs();
	SetRandomNature();
	SetRandomAbility({ EAbility::Torrent });
	CurHp = GetHp();

	// ���� �ɷ�ġ ����
	YDef = 1;
	CatchRate = 45;
	Friendship = 70;
	ExpGroup = EExperienceGroup::MediumSlow;
	Types.push_back(EPokemonType::Water);
}

USquirtle::~USquirtle() 
{
}

