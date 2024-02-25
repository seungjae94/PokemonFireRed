#include "Charmander.h"

UCharmander::UCharmander() 
{
	// �⺻ ���� ����
	PokedexNo = EPokedexNo::Charmander;
	Name = L"CHARMANDER";
	Level = 1;
	AccExp = 0;
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);

	// �ɷ�ġ ����
	BHp = 39;
	BAtk = 52;
	BDef = 43;
	BSpAtk = 60;
	BSpDef = 50;
	BSpeed = 65;
	SetRandomIVs();
	SetRandomNature();
	SetRandomAbility({ EAbility::Blaze });
	CurHp = GetHp();

	// ���� ���� ����
	YSpeed = 1;
	CatchRate = 45;
	Friendship = 70;
	ExpGroup = EExperienceGroup::MediumSlow;
	Types.push_back(EPokemonType::Fire);
}

UCharmander::~UCharmander() 
{
}

