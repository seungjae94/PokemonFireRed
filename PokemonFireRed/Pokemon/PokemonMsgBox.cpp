#include "PokemonMsgBox.h"

APokemonMsgBox::APokemonMsgBox() 
{
}

APokemonMsgBox::~APokemonMsgBox() 
{
}

void APokemonMsgBox::SetBaseRenderingOrder(ERenderingOrder _Order)
{
	ERenderingOrder PlusOrder = static_cast<ERenderingOrder>(static_cast<int>(_Order) + 1);
	ERenderingOrder Plus2Order = static_cast<ERenderingOrder>(static_cast<int>(_Order) + 2);

	Background->SetRenderingOrder(_Order);
	NextMsgArrow->SetRenderingOrder(PlusOrder);
	Text->SetRenderingOrder(PlusOrder);
	Cover->SetRenderingOrder(Plus2Order);
}

void APokemonMsgBox::SetBackgroundImage(std::string_view _ImageName)
{
	Background->SetImage(_ImageName);
}

void APokemonMsgBox::SetCoverImage(std::string_view _ImageName)
{
	Cover->SetImage(_ImageName);
}

void APokemonMsgBox::SetTextColor(EFontColor _Color)
{
	Text->SetColor(_Color);
}

void APokemonMsgBox::SetWriteSpeed(float _WriteSpeed)
{
	WriteSpeed = _WriteSpeed;
}

void APokemonMsgBox::SetLineSpace(int _LineSpace)
{
	Text->SetLineSpace(_LineSpace);
}

void APokemonMsgBox::SetMessage(std::wstring _Message)
{
	Text->SetText(_Message);
	Text->SetRelativePosition(TextInitPos);
	State = EWriteState::None;
	CurLine = 0;
	GlyphIndex = 0;
}

void APokemonMsgBox::ShowSkipArrow()
{
	NextMsgArrow->SetActive(true);

	if (CurLine + 1 < Text->GetLineCount())
	{
		int PixelLineWidth = Text->GetPixelLineWidth(CurLine + 1);
		NextMsgArrow->SetRelativePosition(UPokemonUtil::PixelVector(12 + PixelLineWidth, 9 + Text->GetLineSpace()));
	}
	else
	{
		int PixelLineWidth = Text->GetPixelLineWidth(CurLine);
		NextMsgArrow->SetRelativePosition(UPokemonUtil::PixelVector(12 + PixelLineWidth, 9));
	}
}

void APokemonMsgBox::HideSkipArrow()
{
	NextMsgArrow->SetActive(false);
}

void APokemonMsgBox::Write()
{
	// ���� �����ϴ� ���
	if (State == EWriteState::None)
	{
		State = EWriteState::Writing;
		Text->SetAllGlyphsActive(false);
		Timer = WriteInterval;
	}
	// ��ũ���� �ϰ� ��� �ϴ� ���
	if (State == EWriteState::ScrollWaiting)
	{
		State = EWriteState::Scrolling;
		Timer = ScrollTime;

		IsCurLineErased = false;
		TextPrevPos = Text->GetRelativePosition();
		TextNextPos = TextPrevPos + FVector::Up * Global::FloatPixelSize * static_cast<float>(Text->GetLineSpace());
	}
}

void APokemonMsgBox::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftBot, 5, -3);
	Cover = CreateImageElement(Background, ERenderingOrder::UI3, EPivotType::LeftTop, 0, 0);
	NextMsgArrow = CreateImageElement(Background, ERenderingOrder::UI2, EPivotType::LeftTop, 12, 13);
	NextMsgArrow->SetImage(RN::NextMsgArrow);
	NextMsgArrow->CreateAnimation("Idle", 0, 3, 0.15f, true);
	NextMsgArrow->ChangeAnimation("Idle");
	NextMsgArrow->SetActive(false);
	Text = CreateText(Background, ERenderingOrder::UI2, EPivotType::LeftTop, 11, 17);
	TextInitPos = Text->GetRelativePosition();
}

void APokemonMsgBox::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	Timer -= WriteSpeed * _DeltaTime;

	switch (State)
	{
	case EWriteState::Writing:
		ProcessWriting();
		break;
	case EWriteState::Scrolling:
		ProcessScrolling();
		break;
	default:
		break;
	}

}

void APokemonMsgBox::ProcessWriting()
{
	if (Timer <= 0.0f)
	{
		Text->SetGlyphActive(GlyphIndex, true);
		++GlyphIndex;
		int LineOfGlyph = Text->FindLineOfGlyph(GlyphIndex);

		// ���ο� GlyphIndex�� ������ ���� ������ ����Ű�� ��� -> ��� �޽��� �������� ������
		if (LineOfGlyph == -1)
		{
			State = EWriteState::WriteEnd;
		}
		// ���ο� GlyphIndex�� �޽��� �ڽ��� ���̴� ������ ����Ű�� ��� -> �޽��� �ڽ� ���� ������ ���̴�
		else if (LineOfGlyph == CurLine || LineOfGlyph == CurLine + 1)
		{
			Timer = WriteInterval;
		}
		// ���ο� GlyphIndex�� �޽��� �ڽ��� ������ �ʴ� ������ ����Ű�� ��� -> �޽��� �ڽ� ���� �������� �������� ���� �޽����� �����ִ�
		else
		{
			State = EWriteState::ScrollWaiting;
		}
	}
}

void APokemonMsgBox::ProcessScrolling()
{
	FVector TextPos = UPokemonMath::Lerp(TextNextPos, TextPrevPos, Timer / ScrollTime);
	Text->SetRelativePosition(TextPos);

	if (false == IsCurLineErased && Timer <= 9/16.0f * ScrollTime)
	{
		Text->SetLineActive(CurLine, false);
		IsCurLineErased = true;
	}

	if (Timer <= 0.0f)
	{
		++CurLine;
		State = EWriteState::Writing;
	}
}
