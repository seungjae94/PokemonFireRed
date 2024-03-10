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

class APokemonMsgBox : public ACanvas
{
private:
public:
	// constructor destructor
	APokemonMsgBox();
	~APokemonMsgBox();

	// delete Function
	APokemonMsgBox(const APokemonMsgBox& _Other) = delete;
	APokemonMsgBox(APokemonMsgBox&& _Other) noexcept = delete;
	APokemonMsgBox& operator=(const APokemonMsgBox& _Other) = delete;
	APokemonMsgBox& operator=(APokemonMsgBox&& _Other) noexcept = delete;

	// �ʱ� ����
	void SetBaseRenderingOrder(ERenderingOrder _Order);
	void SetBackgroundImage(std::string_view _ImageName);
	void SetBackgroundLeftBotRelativePos(FVector _Pos);
	void SetCoverImage(std::string_view _ImageName);
	void SetTextLeftTopRelativePos(FVector _Pos);
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

	// �ʱ�ȭ
	void Init();

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// ����
	EWriteState State = EWriteState::None;

	// ���� ���
	AImageElement* Background = nullptr;
	AImageElement* Cover = nullptr;
	AImageElement* NextMsgArrow = nullptr;
	AText* Text = nullptr;

	// ���
	const float ScrollTime = 0.5f;
	const float WriteInterval = 1 / 30.0f;
	
	// ������
	float Timer = 0.0f;
	float WriteSpeed = 1.0f;
	int CurLine = 0;
	int GlyphIndex = 0;
	std::wstring Message;
	FVector TextInitPos;
	FVector TextPrevPos;
	FVector TextNextPos;
	bool IsCurLineErased = false;

	// ���� ƽ �Լ�
	void ProcessWriting();
	void ProcessScrolling();
};

