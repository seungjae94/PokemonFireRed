#include "PokemonMsgBox.h"

PokemonMsgBox::PokemonMsgBox() 
{
}

PokemonMsgBox::~PokemonMsgBox() 
{
}

void PokemonMsgBox::SetBaseRenderingOrder(ERenderingOrder _Order)
{
	ERenderingOrder _PlusOrder = static_cast<ERenderingOrder>(static_cast<int>(_Order) + 1);

	Background->SetRenderingOrder(_Order);
	NextMsgArrow->SetRenderingOrder(_PlusOrder);
	Text->SetRenderingOrder(_PlusOrder);
}

void PokemonMsgBox::SetBackgroundImage(std::string_view _ImageName)
{
	Background->SetImage(_ImageName);
}

void PokemonMsgBox::SetTextColor(EFontColor _Color)
{
	Text->SetColor(_Color);
}

void PokemonMsgBox::SetWriteSpeed(float _WriteSpeed)
{
	WriteSpeed = _WriteSpeed;
}

void PokemonMsgBox::SetLineSpace(int _LineSpace)
{
	Text->SetLineSpace(_LineSpace);
}

void PokemonMsgBox::SetMessage(std::wstring _Message)
{
	Text->SetText(_Message);
	Text->SetRelativePosition(TextInitPos);
	State = EWriteState::None;
	CurLine = 0;
	GlyphIndex = 0;
}

void PokemonMsgBox::ShowSkipArrow()
{
	// TODO: �ִϸ��̼�����...
	NextMsgArrow->SetActive(true);
}

void PokemonMsgBox::HideSkipArrow()
{
	NextMsgArrow->SetActive(false);
}

void PokemonMsgBox::Write()
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
		TextNextPos = TextPrevPos + FVector::Up * Global::FloatPixelSize * Text->GetLineSpace() ;
	}
}

void PokemonMsgBox::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI1, EPivotType::LeftBot, 5, 0);
	NextMsgArrow = CreateImageElement(this, ERenderingOrder::UI2, EPivotType::RightBot, -5, -5);
	Text = CreateText(this, ERenderingOrder::UI2, EPivotType::LeftTop, 11, 17);
	
	Text->SetCuttingRect(UPokemonUtil::PixelVector(0, 143), UPokemonUtil::PixelVector(240, 150));
	TextInitPos = Text->GetRelativePosition();
}

void PokemonMsgBox::Tick(float _DeltaTime)
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

void PokemonMsgBox::ProcessWriting()
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
		if (LineOfGlyph == CurLine || LineOfGlyph == CurLine + 1)
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

void PokemonMsgBox::ProcessScrolling()
{
	FVector TextPos = UPokemonMath::Lerp(TextNextPos, TextPrevPos, Timer / ScrollTime);
	Text->SetRelativePosition(TextPos);

	if (Timer <= 0.0f)
	{
		++CurLine;
		State = EWriteState::Writing;
	}
}
