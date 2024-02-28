#include "WildBattleCanvas.h"
#include "PokemonUtil.h"
#include "PlayerData.h"

AWildBattleCanvas::AWildBattleCanvas()
{
}

AWildBattleCanvas::~AWildBattleCanvas()
{
}

void AWildBattleCanvas::BattleStart(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon)
{
	// 임시로 데이터만 보여주기
	EnemyPokemonNameText->SetText(_EnemyPokemon.GetNameW());
	EnemyPokemonLevelText->SetText(L"ℓ" + _EnemyPokemon.GetLevelW());
	EnemyPokemonHpBar->SetMaxValue(_EnemyPokemon.GetHp());
	EnemyPokemonHpBar->SetValue(_EnemyPokemon.GetCurHp());

	int EPNTPixelWidth = EnemyPokemonNameText->GetPixelLineWidth();
	EnemyPokemonGenderMark->SetRelativePos(UPokemonUtil::PixelVector(7 + EPNTPixelWidth, 5));
	EnemyPokemonGenderMark->SetImage(_EnemyPokemon.GetGenderImageName());

	PlayerPokemonNameText->SetText(_PlayerPokemon.GetNameW());
	PlayerPokemonLevelText->SetText(L"ℓ" + _PlayerPokemon.GetLevelW());
	PlayerPokemonCurHpText->SetText(_PlayerPokemon.GetCurHpW());
	PlayerPokemonHpText->SetText(_PlayerPokemon.GetHpW());
	PlayerPokemonHpBar->SetMaxValue(_PlayerPokemon.GetHp());
	PlayerPokemonHpBar->SetValue(_PlayerPokemon.GetCurHp());
	PlayerPokemonExpBar->SetMaxValue(_PlayerPokemon.GetExpSize());
	PlayerPokemonExpBar->SetValue(_PlayerPokemon.GetExp());

	int PPNTPixelWidth = PlayerPokemonNameText->GetPixelLineWidth();
	PlayerPokemonGenderMark->SetRelativePos(UPokemonUtil::PixelVector(19 + PPNTPixelWidth, 5));
	PlayerPokemonGenderMark->SetImage(_PlayerPokemon.GetGenderImageName());

	EnemyPokemonImage->SetPokemon(_EnemyPokemon);
}

void AWildBattleCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageRenderer(ERenderingOrder::UI0);
	Background->CameraEffectOff();
	Background->SetImage(RN::BattleBackground);
	UPokemonUtil::PlaceImageOnScreen(Background, EPivotType::LeftTop, 0, 0);

	// 최상위 요소
	MsgBox = CreateImageRenderer(ERenderingOrder::UI1);
	MsgBox->CameraEffectOff();
	MsgBox->SetImage(RN::BattleMsgBox);
	UPokemonUtil::PlaceImageOnScreen(MsgBox, EPivotType::LeftBot, 0, 0);

	ActionBox = CreateImageRenderer(ERenderingOrder::UI1);
	ActionBox->CameraEffectOff();
	ActionBox->SetImage(RN::BattleActionBox);
	UPokemonUtil::PlaceImageOnScreen(ActionBox, EPivotType::RightBot, 0, 0);

	EnemyPokemonBox = CreateImageRenderer(ERenderingOrder::UI2);
	EnemyPokemonBox->CameraEffectOff();
	EnemyPokemonBox->SetImage(RN::BattleEnemyPokemonBox);
	UPokemonUtil::PlaceImageOnScreen(EnemyPokemonBox, EPivotType::LeftTop, 13, 16);

	PlayerPokemonBox = CreateImageRenderer(ERenderingOrder::UI2);
	PlayerPokemonBox->CameraEffectOff();
	PlayerPokemonBox->SetImage(RN::BattlePlayerPokemonBox);
	UPokemonUtil::PlaceImageOnScreen(PlayerPokemonBox, EPivotType::RightBot, -10, -49);

	EnemyGround = CreateImageRenderer(ERenderingOrder::UI1);
	EnemyGround->CameraEffectOff();
	EnemyGround->SetImage(RN::BattleEnemyGround);
	UPokemonUtil::PlaceImageOnScreen(EnemyGround, EPivotType::RightTop, 0, 46);

	PlayerGround = CreateImageRenderer(ERenderingOrder::UI1);
	PlayerGround->CameraEffectOff();
	PlayerGround->SetImage(RN::BattlePlayerGround);
	UPokemonUtil::PlaceImageOnScreen(PlayerGround, EPivotType::LeftBot, 4, -48);

	// EnemyPokemonBox 요소
	EnemyPokemonNameText = CreateText(EnemyPokemonBox, L"UNDEFINED", EPivotType::LeftTop, EAlignType::Left, 7, 12, EFontColor::Black, EFontSize::Mini, ERenderingOrder::UI3);
	EnemyPokemonLevelText = CreateText(EnemyPokemonBox, L"Lv100", EPivotType::LeftTop, EAlignType::Right, 85, 12, EFontColor::Black, EFontSize::Mini, ERenderingOrder::UI3);
	EnemyPokemonHpBar = CreateScrollBar(EnemyPokemonBox, EScrollType::Hp, 50, 100, EPivotType::LeftTop, 39, 17, ERenderingOrder::UI3);
	EnemyPokemonGenderMark = CreateImageElement(EnemyPokemonBox, EPivotType::LeftTop, 10, 5, ERenderingOrder::UI3);

	// PlayerPokemonBox 요소
	PlayerPokemonNameText = CreateText(PlayerPokemonBox, L"UNDEFINED", EPivotType::RightTop, EAlignType::Left, -84, 12, EFontColor::Black, EFontSize::Mini, ERenderingOrder::UI3);
	PlayerPokemonLevelText = CreateText(PlayerPokemonBox, L"Lv100", EPivotType::RightTop, EAlignType::Right, -9, 12, EFontColor::Black, EFontSize::Mini, ERenderingOrder::UI3);
	PlayerPokemonCurHpText = CreateText(PlayerPokemonBox, L"999", EPivotType::RightBot, EAlignType::Right, -29, -6, EFontColor::Black, EFontSize::Mini, ERenderingOrder::UI3);
	PlayerPokemonHpText = CreateText(PlayerPokemonBox, L"999", EPivotType::RightBot, EAlignType::Right, -9, -6, EFontColor::Black, EFontSize::Mini, ERenderingOrder::UI3);
	PlayerPokemonHpBar = CreateScrollBar(PlayerPokemonBox, EScrollType::Hp, 50, 100, EPivotType::RightTop, -55, 17, ERenderingOrder::UI3);
	PlayerPokemonExpBar = CreateScrollBar(PlayerPokemonBox, EScrollType::Exp, 50, 100, EPivotType::RightTop, -71, 33, ERenderingOrder::UI3);
	PlayerPokemonGenderMark = CreateImageElement(PlayerPokemonBox, EPivotType::LeftTop, 10, 5, ERenderingOrder::UI3);

	// EnemyGround 요소
	EnemyPokemonImage = CreatePokemonElement(EnemyGround, EPokemonElementType::Front, EPivotType::LeftTop, 36, -25, ERenderingOrder::UI3);

	// PlayerGround 요소
}

void AWildBattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

