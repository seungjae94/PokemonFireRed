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

	// EnemyPokemonBox 요소
	EnemyPokemonNameText = CreateText(
		EnemyPokemonBox,
		L"UNDEFINED",
		EPivotType::LeftTop,
		EAlignType::Left,
		7, 12,
		EFontColor::Black,
		EFontSize::Mini
	);
	EnemyPokemonLevelText = CreateText(
		EnemyPokemonBox,
		L"Lv100",
		EPivotType::LeftTop,
		EAlignType::Right,
		85, 12,
		EFontColor::Black,
		EFontSize::Mini
	);
	EnemyPokemonHpBar = CreateScrollBar(
		EnemyPokemonBox,
		EScrollType::Hp,
		50, 100,
		EPivotType::LeftTop,
		39, 17
	);

	// PlayerPokemonBox 요소
	PlayerPokemonNameText = CreateText(
		PlayerPokemonBox,
		L"UNDEFINED",
		EPivotType::RightTop,
		EAlignType::Left,
		-84, 12,
		EFontColor::Black,
		EFontSize::Mini
	);
	PlayerPokemonLevelText = CreateText(
		PlayerPokemonBox,
		L"Lv100",
		EPivotType::RightTop,
		EAlignType::Right,
		-6, 12,
		EFontColor::Black,
		EFontSize::Mini
	);
	PlayerPokemonCurHpText = CreateText(
		PlayerPokemonBox,
		L"999",
		EPivotType::RightBot,
		EAlignType::Right,
		-29, -6,
		EFontColor::Black,
		EFontSize::Mini
	);
	PlayerPokemonHpText = CreateText(
		PlayerPokemonBox,
		L"999",
		EPivotType::RightBot,
		EAlignType::Right,
		-9, -6,
		EFontColor::Black,
		EFontSize::Mini
	);
	PlayerPokemonHpBar = CreateScrollBar(
		PlayerPokemonBox,
		EScrollType::Hp,
		50, 100,
		EPivotType::RightTop,
		-55, 17
	);
	PlayerPokemonExpBar = CreateScrollBar(
		PlayerPokemonBox,
		EScrollType::Exp,
		50, 100,
		EPivotType::RightTop,
		-71, 33
	);
}

void AWildBattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

