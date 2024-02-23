#include "Bulbasaur.h"

UBulbasaur::UBulbasaur()
{
	// �⺻ ���� ����
	EPokedexNo PokedexNo = EPokedexNo::Bulbasaur;
	Name = L"BULBASAUR";
	Level = 1;
	AccExp = 0;

	// �⺻ ������ ����
	float MaleRatio = 7 / 8.0f;
	SetRandomGender(MaleRatio);
	SetRandomNature();
	SetRandomAbility({EAbility::Overgrow});

	// �ɷ�ġ ����
	BHp = 45;
	BAtk = 49;
	BDef = 49;
	BSpAtk = 65;
	BSpDef = 65;
	BSpeed = 45;
	SetRandomIVs();

	// ���� �ɷ�ġ ����
	YSpAtk = 1;
	CatchRate = 45;
	Friendship = 70;

	Types.push_back(EPokemonType::Grass);
	Types.push_back(EPokemonType::Poison);
}

UBulbasaur::~UBulbasaur() 
{
}

