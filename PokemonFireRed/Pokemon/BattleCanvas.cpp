#include "BattleCanvas.h"
#include "PokemonUtil.h"
#include "PlayerData.h"

ABattleCanvas::ABattleCanvas()
{
}

ABattleCanvas::~ABattleCanvas()
{
}

void ABattleCanvas::Init(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon)
{
	PrepareElements(_PlayerPokemon, _EnemyPokemon);

	// 액션 박스 꺼두기
	ActionBox->SetActive(false);

	// 볼 날아가는 애니메이션 숨겨두기
	ThrowedBall->SetActive(false);

	// 아군 포켓몬 숨겨두기
	PlayerPokemonImage->SetRelativePosition(PlayerPokemonImageHidePos);
	PlayerPokemonImage->SetScaleFactor(0.0f);

	// 적 UI는 왼쪽에 숨겨두기
	EnemyGround->SetRelativePosition(EnemyGroundHidePos);
	EnemyPokemonBox->SetRelativePosition(EnemyPokemonBoxHidePos);

	// 아군 UI는 오른쪽에 숨겨두기
	PlayerPokemonBox->SetRelativePosition(PlayerPokemonBoxHidePos);
	PlayerGround->SetRelativePosition(PlayerGroundHidePos);
	PlayerBattler->SetRelativePosition(PlayerBattlerInitPos);
}

void ABattleCanvas::PrepareElements(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon)
{
	MsgText->SetText(L"Wild " + _EnemyPokemon.GetNameW() + L" appeared!");

	EnemyPokemonNameText->SetText(_EnemyPokemon.GetNameW());
	EnemyPokemonLevelText->SetText(L"ℓ" + _EnemyPokemon.GetLevelW());
	EnemyPokemonHpBar->SetMaxValue(_EnemyPokemon.GetHp());
	EnemyPokemonHpBar->SetValue(_EnemyPokemon.GetCurHp());

	int EPNTPixelWidth = EnemyPokemonNameText->GetPixelLineWidth();
	EnemyPokemonGenderMark->SetRelativePosition(UPokemonUtil::PixelVector(7 + EPNTPixelWidth, 5));
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
	PlayerPokemonGenderMark->SetRelativePosition(UPokemonUtil::PixelVector(19 + PPNTPixelWidth, 5));
	PlayerPokemonGenderMark->SetImage(_PlayerPokemon.GetGenderImageName());

	EnemyPokemonImage->SetPokemon(_EnemyPokemon);
	PlayerBattler->ChangeAnimation(Global::PlayerBattlerIdle);
	PlayerPokemonImage->SetPokemon(_PlayerPokemon);
}


void ABattleCanvas::LerpShowGrounds(float _t)
{
	FVector EnemyGroundPos = UPokemonMath::Lerp(EnemyGroundInitPos, EnemyGroundHidePos, _t);
	FVector PlayerGroundPos = UPokemonMath::Lerp(PlayerGroundInitPos, PlayerGroundHidePos, _t);
	EnemyGround->SetRelativePosition(EnemyGroundPos);
	PlayerGround->SetRelativePosition(PlayerGroundPos);
}

void ABattleCanvas::LerpShowEnemyPokemonBox(float _t)
{
	FVector EnemyPokemonBoxPos = UPokemonMath::Lerp(EnemyPokemonBoxInitPos, EnemyPokemonBoxHidePos, _t);
	EnemyPokemonBox->SetRelativePosition(EnemyPokemonBoxPos);
}

void ABattleCanvas::PlayBattlerThrowingAnimation()
{
	PlayerBattler->ChangeAnimation(Global::PlayerBattlerThrow);
}

void ABattleCanvas::LerpHidePlayerBattler(float _t)
{
	FVector PlayerBattlerRelativePos = UPokemonMath::Lerp(PlayerBattlerHidePos, PlayerBattlerInitPos, _t);
	PlayerBattler->SetRelativePosition(PlayerBattlerRelativePos);
}

void ABattleCanvas::PlayThrowedBallAnimation()
{
	ThrowedBall->SetActive(true);
	ThrowedBall->ChangeAnimation(Global::ThrowedBall, true);
}

void ABattleCanvas::TakeOutPokemonFromBall(float _t)
{
	float t = _t;
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	PlayerPokemonImage->SetScaleFactor(1.0f - t);

	FVector PlayerPokemonImagePos = UPokemonMath::Lerp(PlayerPokemonImageInitPos, PlayerPokemonImageHidePos, _t);
	PlayerPokemonImage->SetRelativePosition(PlayerPokemonImagePos);
}

void ABattleCanvas::LerpShowPlayerPokemonBox(float _t)
{
	FVector PlayerPokemonBoxPos = UPokemonMath::Lerp(PlayerPokemonBoxInitPos, PlayerPokemonBoxHidePos, _t);
	PlayerPokemonBox->SetRelativePosition(PlayerPokemonBoxPos);
}

void ABattleCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// 배경
	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::BattleBackground);

	// 최상위 요소
	MsgBox = CreateImageElement(this, ERenderingOrder::UI4, EPivotType::LeftBot, 0, 0);
	MsgBox->SetImage(RN::BattleMsgBox);

	ActionBox = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::RightBot, 0, 0);
	ActionBox->SetImage(RN::BattleActionBox);

	EnemyPokemonBox = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftTop, 13, 16);
	EnemyPokemonBox->SetImage(RN::BattleEnemyPokemonBox);
	EnemyPokemonBoxInitPos = EnemyPokemonBox->GetRelativePosition();
	EnemyPokemonBoxHidePos = EnemyPokemonBoxInitPos + FVector::Left * Global::FloatScreenX;

	PlayerPokemonBox = CreateImageElement(this, ERenderingOrder::UI4, EPivotType::RightBot, -10, -48);
	PlayerPokemonBox->SetImage(RN::BattlePlayerPokemonBox);
	PlayerPokemonBoxInitPos = PlayerPokemonBox->GetRelativePosition();
	PlayerPokemonBoxHidePos = PlayerPokemonBoxInitPos + FVector::Right * Global::FloatScreenX;

	EnemyGround = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::RightTop, 0, 46);
	EnemyGround->SetImage(RN::BattleEnemyGround);
	EnemyGroundInitPos = EnemyGround->GetRelativePosition();
	EnemyGroundHidePos = EnemyGroundInitPos + FVector::Left * Global::FloatScreenX;

	PlayerGround = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftBot, 4, -48);
	PlayerGround->SetImage(RN::BattlePlayerGround);
	PlayerGroundInitPos = PlayerGround->GetRelativePosition();
	PlayerGroundHidePos = PlayerGroundInitPos + FVector::Right * Global::FloatScreenX;

	// MsgBox 요소
	MsgText = CreateText(MsgBox, ERenderingOrder::UI5, EPivotType::LeftTop, 11, 21, EAlignType::Left, EFontColor::White, EFontSize::Normal);

	// EnemyPokemonBox 요소
	EnemyPokemonNameText = CreateText(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 7, 12, EAlignType::Left, EFontColor::Black, EFontSize::Mini);
	EnemyPokemonLevelText = CreateText(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 85, 12, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	EnemyPokemonHpBar = CreateScrollBar(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 39, 17, EScrollType::Hp);
	EnemyPokemonGenderMark = CreateImageElement(EnemyPokemonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 10, 5);

	// PlayerPokemonBox 요소
	PlayerPokemonNameText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -84, 12, EAlignType::Left, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonLevelText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -9, 12, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonCurHpText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightBot, -29, -6, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonHpText = CreateText(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightBot, -9, -6, EAlignType::Right, EFontColor::Black, EFontSize::Mini);
	PlayerPokemonHpBar = CreateScrollBar(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -55, 17, EScrollType::Hp);
	PlayerPokemonExpBar = CreateScrollBar(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::RightTop, -71, 33, EScrollType::Exp);
	PlayerPokemonGenderMark = CreateImageElement(PlayerPokemonBox, ERenderingOrder::UI5, EPivotType::LeftTop, 10, 5);

	// EnemyGround 요소
	EnemyPokemonImage = CreateImageElement(EnemyGround, ERenderingOrder::UI2, EPivotType::LeftTop, 36, -25, EImageElementType::PokemonFront);

	// PlayerGround 요소
	PlayerBattler = CreateImageElement(PlayerGround, ERenderingOrder::UI3, EPivotType::RightBot, -12, 0);
	PlayerBattler->SetImage(RN::PlayerBattler);
	PlayerBattler->CreateAnimation(Global::PlayerBattlerIdle, 0, 0, 0.0f, false);
	PlayerBattler->CreateAnimation(Global::PlayerBattlerThrow, { 1, 2, 3, 4 }, { 0.28f, 0.14f, 0.07f, 0.07f }, false);
	PlayerBattlerInitPos = PlayerBattler->GetRelativePosition();
	PlayerBattlerHidePos = PlayerBattlerInitPos + UPokemonUtil::PixelVector(-120, 0);

	ThrowedBall = CreateImageElement(PlayerGround, ERenderingOrder::UI2, EPivotType::LeftBot, 30, 0);
	ThrowedBall->SetImage(Global::ThrowedBall);
	ThrowedBall->CreateAnimation(Global::ThrowedBall, 0, 11, 0.06f, false);

	PlayerPokemonImage = CreateImageElement(PlayerGround, ERenderingOrder::UI2, EPivotType::LeftTop, 35, -49, EImageElementType::PokemonBack);
	PlayerPokemonImageInitPos = PlayerPokemonImage->GetRelativePosition();
	PlayerPokemonImageHidePos = PlayerPokemonImageInitPos + UPokemonUtil::PixelVector(0, 32);
}

void ABattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

