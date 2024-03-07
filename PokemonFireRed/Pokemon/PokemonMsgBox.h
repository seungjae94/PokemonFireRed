#pragma once
#include "Canvas.h"
#include "ImageElement.h"
#include "Text.h"

enum class EWriteState
{
	None,
	Writing,
	ScrollWaiting,
	Scrolling,
	WriteEnd
};

class PokemonMsgBox : public ACanvas
{
private:
public:
	// constructor destructor
	PokemonMsgBox();
	~PokemonMsgBox();

	// delete Function
	PokemonMsgBox(const PokemonMsgBox& _Other) = delete;
	PokemonMsgBox(PokemonMsgBox&& _Other) noexcept = delete;
	PokemonMsgBox& operator=(const PokemonMsgBox& _Other) = delete;
	PokemonMsgBox& operator=(PokemonMsgBox&& _Other) noexcept = delete;

	// �ʱ� ����
	void SetBaseRenderingOrder(ERenderingOrder _Order);
	void SetBackgroundImage(std::string_view _ImageName);
	void SetTextColor(EFontColor _Color);
	void SetWriteSpeed(float _WriteSpeed);
	void SetLineSpace(int _LineSpace);

	// ������
	void SetMessage(std::wstring _Message);
	void ShowSkipArrow();
	void HideSkipArrow();

	// ����
	void Write();
	EWriteState GetWriteState() const
	{
		return State;
	}

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// ����
	EWriteState State = EWriteState::None;

	// ���� ���
	AImageElement* Background = nullptr;
	AImageElement* NextMsgArrow = nullptr;		// TODO: �ִϸ��̼����� ����
	AText* Text = nullptr;

	// ���
	const float ScrollTime = 1.0f;
	const float WriteInterval = 1 / 60.0f;
	
	// ������
	float Timer = 0.0f;
	float WriteSpeed = 1.0f;
	int CurLine = 0;
	int GlyphIndex = 0;
	std::wstring Message;
	FVector TextInitPos;
	FVector TextPrevPos;
	FVector TextNextPos;

	// ���� ƽ �Լ�
	void ProcessWriting();
	void ProcessScrolling();
};

