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
	// 임시로 그냥 데이터 보여주기만 하기

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

	// 최상위 요소
	MsgBox = CreateImageRenderer(ERenderingOrder::UI1);
	MsgBox->CameraEffectOff();
	MsgBox->SetImage(RN::BattleMsgBox);
	UPokemonUtil::PlaceImageOnScreen(
		MsgBox,
		EPivotType::LeftBot,
		0, 0
	);

	ActionBox = CreateImageRenderer(ERenderingOrder::UI1);
	ActionBox->CameraEffectOff();
	ActionBox->SetImage(RN::BattleActionBox);
	UPokemonUtil::PlaceImageOnScreen(
		ActionBox,
		EPivotType::RightBot,
		0, 0
	);

	EnemyPokemonBox = CreateImageRenderer(ERenderingOrder::UI1);
	EnemyPokemonBox->CameraEffectOff();
	EnemyPokemonBox->SetImage(RN::BattleEnemyPokemonBox);
	UPokemonUtil::PlaceImageOnScreen(
		EnemyPokemonBox,
		EPivotType::LeftTop,
		13, 16
	);

	PlayerPokemonBox = CreateImageRenderer(ERenderingOrder::UI1);
	PlayerPokemonBox->CameraEffectOff();
	PlayerPokemonBox->SetImage(RN::BattlePlayerPokemonBox);
	UPokemonUtil::PlaceImageOnScreen(
		PlayerPokemonBox,
		EPivotType::RightBot,
		-10, -49
	);
}

void AWildBattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

