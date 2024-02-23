#include "Charmander.h"

UCharmander::UCharmander() 
{
	// �⺻ ���� ����
	Level = 1;
	AccExp = 0;
	Name = L"CHARMANDER";

	// �⺻ ������ ����
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);
	SetRandomNature();
	SetRandomAbility({ EAbility::Blaze });

	// �ɷ�ġ ����
	BHp = 39;
	BAtk = 52;
	BDef = 43;
	BSpAtk = 60;
	BSpDef = 50;
	BSpeed = 65;
	SetRandomIVs();

	// ���� �ɷ�ġ ����
	YSpeed = 1;
	CatchRate = 45;
	Friendship = 70;

	Types.push_back(EPokemonType::Fire);
}

UCharmander::~UCharmander() 
{
}

