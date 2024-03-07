#include "PokemonMsgBox.h"

APokemonMsgBox::APokemonMsgBox() 
{
}

APokemonMsgBox::~APokemonMsgBox() 
{
}

void APokemonMsgBox::SetBaseRenderingOrder(ERenderingOrder _Order)
{
	ERenderingOrder _PlusOrder = static_cast<ERenderingOrder>(static_cast<int>(_Order) + 1);

	Background->SetRenderingOrder(_Order);
	NextMsgArrow->SetRenderingOrder(_PlusOrder);
	Text->SetRenderingOrder(_PlusOrder);
}

void APokemonMsgBox::SetBackgroundImage(std::string_view _ImageName)
{
	Background->SetImage(_ImageName);
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
	// TODO: �ִϸ��̼�����...
	NextMsgArrow->SetActive(true);
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

		TextPrevPos = Text->GetRelativePosition();
		TextNextPos = TextPrevPos + FVector::Up * Global::FloatPixelSize * static_cast<float>(Text->GetLineSpace());
	}
}

void APokemonMsgBox::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftBot, 5, -3);
	NextMsgArrow = CreateImageElement(Background, ERenderingOrder::UI2, EPivotType::RightBot, -5, -5);
	Text = CreateText(Background, ERenderingOrder::UI2, EPivotType::LeftTop, 11, 17);
	Text->SetCuttingRect(UPokemonUtil::PixelVector(16, 124), UPokemonUtil::PixelVector(208, 26));
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
	Text->Cut();

	if (Timer <= 0.0f)
	{
		++CurLine;
		State = EWriteState::Writing;
	}
}
