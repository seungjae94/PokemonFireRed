#include "WildBattleCanvas.h"
#include "PokemonUtil.h"

AWildBattleCanvas::AWildBattleCanvas() 
{
}

AWildBattleCanvas::~AWildBattleCanvas() 
{
}

void AWildBattleCanvas::BattleStart(const UPokemon& _Pokemon)
{
	// �ӽ÷� �׳� ������ �����ֱ⸸ �ϱ�

}

void AWildBattleCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageRenderer(ERenderingOrder::UI0);
	Background->CameraEffectOff();
	Background->SetImage(RN::BattleBackground);
	UPokemonUtil::PlaceImageOnScreen(
		Background,
		EPivotType::LeftTop,
		0, 0
	);
}

void AWildBattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

