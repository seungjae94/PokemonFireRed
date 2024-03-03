#include "BattleCanvas.h"
#include "PokemonUtil.h"
#include "PlayerData.h"

ABattleCanvas::ABattleCanvas()
{
}

ABattleCanvas::~ABattleCanvas()
{
}

void ABattleCanvas::Init(const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon)
{
	PlayerPokemon = _PlayerPokemon;
	EnemyPokemon = _EnemyPokemon;

	MsgText->SetText(L"Wild " + EnemyPokemon->GetNameW() + L" appeared!");
	RefreshPlayerPokemonBox();
	RefreshEnemyPokemonBox();
	RefreshMoveSelectBox();
	// 커서 초기화
	ActionCursor->SetCursor(0);
	MoveSelectCursor->SetCursor(0);

	ActionBox->SetActive(false);
	ThrowedBall->SetActive(false);
	MoveSelectBox->SetActive(false);

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

void ABattleCanvas::RefreshEnemyPokemonBox()
{
	EnemyPokemonNameText->SetText(EnemyPokemon->GetNameW());
	EnemyPokemonLevelText->SetText(L"ℓ" + EnemyPokemon->GetLevelW());
	EnemyPokemonHpBar->SetMaxValue(HpBarMaxValue);
	EnemyPokemonHpBar->SetValue(UPokemonMath::Floor(static_cast<float>(HpBarMaxValue) * EnemyPokemon->GetCurHp() / EnemyPokemon->GetHp()));

	int EPNTPixelWidth = EnemyPokemonNameText->GetPixelLineWidth();
	EnemyPokemonGenderMark->SetRelativePosition(UPokemonUtil::PixelVector(7 + EPNTPixelWidth, 5));
	EnemyPokemonGenderMark->SetImage(EnemyPokemon->GetGenderImageName());

	EnemyPokemonImage->SetPokemon(EnemyPokemon);
}

void ABattleCanvas::RefreshPlayerPokemonBox()
{
	PlayerPokemonNameText->SetText(PlayerPokemon->GetNameW());
	PlayerPokemonLevelText->SetText(L"ℓ" + PlayerPokemon->GetLevelW());
	PlayerPokemonCurHpText->SetText(PlayerPokemon->GetCurHpW());
	PlayerPokemonHpText->SetText(PlayerPokemon->GetHpW());
	PlayerPokemonHpBar->SetMaxValue(HpBarMaxValue);
	PlayerPokemonHpBar->SetValue(UPokemonMath::Floor(static_cast<float>(HpBarMaxValue) * PlayerPokemon->GetCurHp() / PlayerPokemon->GetHp()));
	PlayerPokemonExpBar->SetMaxValue(PlayerPokemon->GetExpSize());
	PlayerPokemonExpBar->SetValue(PlayerPokemon->GetExp());

	int PPNTPixelWidth = PlayerPokemonNameText->GetPixelLineWidth();
	PlayerPokemonGenderMark->SetRelativePosition(UPokemonUtil::PixelVector(19 + PPNTPixelWidth, 5));
	PlayerPokemonGenderMark->SetImage(PlayerPokemon->GetGenderImageName());

	PlayerBattler->ChangeAnimation(Global::PlayerBattlerIdle);
	PlayerPokemonImage->SetPokemon(PlayerPokemon);
}

void ABattleCanvas::RefreshMoveSelectBox()
{
	MoveSelectCursor->SetCursor(0);

	for (int i = 0; i < PlayerPokemon->GetMoveCount(); ++i)
	{
		MoveTexts[i]->SetText(PlayerPokemon->GetMoveNameW(i));
	}
	for (int i = PlayerPokemon->GetMoveCount(); i < 4; ++i)
	{
		MoveTexts[i]->SetText(L"-");
	}

	CurPPText->SetText(PlayerPokemon->GetMoveCurPPW(0));
	MaxPPText->SetText(PlayerPokemon->GetMovePPW(0));
	MoveTypeText->SetText(PlayerPokemon->GetMoveTypeUpperW(0));
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

bool ABattleCanvas::IsThrowedBallAnimationEnd()
{
	return ThrowedBall->IsCurAnimationEnd();
}

void ABattleCanvas::SetThrowedBallActive(bool _Value)
{
	ThrowedBall->SetActive(_Value);
}

void ABattleCanvas::TakeOutPokemonFromBall(float _t)
{
	float t = _t;
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	PlayerPokemonImage->SetScaleFactor(1.0f - t);
	PlayerPokemonImage->SetAlpha(1.0f - t / 2.0f);

	FVector PlayerPokemonImagePos = UPokemonMath::Lerp(PlayerPokemonImageInitPos, PlayerPokemonImageHidePos, _t);
	PlayerPokemonImage->SetRelativePosition(PlayerPokemonImagePos);
}

void ABattleCanvas::LerpShowPlayerPokemonBox(float _t)
{
	FVector PlayerPokemonBoxPos = UPokemonMath::Lerp(PlayerPokemonBoxInitPos, PlayerPokemonBoxHidePos, _t);
	PlayerPokemonBox->SetRelativePosition(PlayerPokemonBoxPos);
}

void ABattleCanvas::SetActionBoxActive(bool _Value)
{
	ActionBox->SetActive(_Value);
}

void ABattleCanvas::SetMoveSelectBoxActive(bool _Value)
{
	MoveSelectBox->SetActive(_Value);
}

void ABattleCanvas::SetBattleMessage(std::wstring_view _Msg)
{
	MsgText->SetText(_Msg);
}

void ABattleCanvas::LerpPlayerHpInfo(int _BeforeHp, int _AfterHp, int _MaxHp, float _t)
{
	int LerpedHp = UPokemonMath::Round(UPokemonMath::Lerp(static_cast<float>(_AfterHp), static_cast<float>(_BeforeHp), _t));
	PlayerPokemonCurHpText->SetText(std::to_wstring(LerpedHp));

	float LerpedBarValue = UPokemonMath::Lerp(static_cast<float>(HpBarMaxValue) * _AfterHp / _MaxHp, static_cast<float>(HpBarMaxValue) * _BeforeHp / _MaxHp, _t);
	PlayerPokemonHpBar->SetValue(UPokemonMath::Round(LerpedBarValue));
}

void ABattleCanvas::LerpEnemyHpInfo(int _BeforeHp, int _AfterHp, int _MaxHp, float _t)
{
	float LerpedBarValue = UPokemonMath::Lerp(static_cast<float>(HpBarMaxValue) * _AfterHp / _MaxHp, static_cast<float>(HpBarMaxValue) * _BeforeHp / _MaxHp, _t);
	EnemyPokemonHpBar->SetValue(UPokemonMath::Round(LerpedBarValue));
}

void ABattleCanvas::RefreshPlayerPokemonCurHpText()
{
	PlayerPokemonCurHpText->SetText(PlayerPokemon->GetCurHpW());
}

void ABattleCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// 최상위 요소
	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::BattleBackground);

	MsgBox = CreateImageElement(this, ERenderingOrder::UI4, EPivotType::LeftBot, 0, 0);
	MsgBox->SetImage(RN::BattleMsgBox);

	ActionBox = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::RightBot, 0, 0);
	ActionBox->SetImage(RN::BattleActionBox);

	MoveSelectBox = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::LeftBot, 0, 0);
	MoveSelectBox->SetImage(RN::BattleMoveSelectBox);

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

	ThrowedBall = CreateImageElement(this, ERenderingOrder::UI2, EPivotType::LeftTop, 42, 56);
	ThrowedBall->SetImage(Global::ThrowedBall);
	ThrowedBall->CreateAnimation(Global::ThrowedBall, 0, 41, 1.0f / 60, false);

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
	PlayerBattler->CreateAnimation(Global::PlayerBattlerThrow, { 1, 2, 3, 4 }, { 16.0f/60, 8.0f/60, 4.0f/60, 4.0f/60 }, false);
	PlayerBattlerInitPos = PlayerBattler->GetRelativePosition();
	PlayerBattlerHidePos = PlayerBattlerInitPos + UPokemonUtil::PixelVector(-120, 0);

	PlayerPokemonImage = CreateImageElement(PlayerGround, ERenderingOrder::UI2, EPivotType::LeftTop, 35, -49, EImageElementType::PokemonBack);
	PlayerPokemonImageInitPos = PlayerPokemonImage->GetRelativePosition();
	PlayerPokemonImageHidePos = PlayerPokemonImageInitPos + UPokemonUtil::PixelVector(0, 32);

	// ActionBox 요소
	ActionCursor = CreateCursor(ActionBox, ERenderingOrder::UI7, EPivotType::LeftTop, 9, 12);
	ActionCursor->SetOptionCount(4);

	int CursorHorGap = 56;
	int CursorVerGap = 16;
	ActionCursor->SetCursorPositions({ {0, 0}, {CursorHorGap, 0}, {0, CursorVerGap}, {CursorHorGap, CursorVerGap} });

	// MoveSelectBox 요소
	MoveSelectCursor = CreateCursor(MoveSelectBox, ERenderingOrder::UI7, EPivotType::LeftTop, 9, 12);
	MoveSelectCursor->SetOptionCount(4);

	int MoveHorGap = 71;
	int MoveVerGap = 17;
	MoveSelectCursor->SetCursorPositions({ {0, 0}, {MoveHorGap, 0}, {0, MoveVerGap}, {MoveHorGap, MoveVerGap} });

	CurPPText = CreateText(MoveSelectBox, ERenderingOrder::UI7, EPivotType::RightTop, -26, 21, EAlignType::Right, EFontColor::Black2);
	MaxPPText = CreateText(MoveSelectBox, ERenderingOrder::UI7, EPivotType::RightTop, -9, 21, EAlignType::Right, EFontColor::Black2);
	MoveTypeText = CreateText(MoveSelectBox, ERenderingOrder::UI7, EPivotType::RightTop, -48, 37, EAlignType::Left, EFontColor::Black3);

	for (int i = 0; i < 4; ++i)
	{
		int RowIndex = i / 2;
		int ColIndex = i % 2;
		AText* MoveText = CreateText(MoveSelectBox, ERenderingOrder::UI7, EPivotType::LeftTop, 
			17 + ColIndex * MoveHorGap, 20 + RowIndex * MoveVerGap, EAlignType::Left, EFontColor::Black3, EFontSize::Mini);
		MoveTexts.push_back(MoveText);
	}

}

void ABattleCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}

