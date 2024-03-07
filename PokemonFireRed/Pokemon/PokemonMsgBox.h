#pragma once
#include "UIElement.h"
#include "ImageElement.h";
#include "Text.h";

class PokemonMsgBox : public ACanvas
{
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
	void SetBackgroundImage(std::string_view _ImageName);
	void SetTextColor(EFontColor _Color);
	void SetWriteSpeed(int _WriteSpeed);
	void SetLineSpace(int _LineSpace);

	// ������
	void SetMessage(std::wstring _Message);
	void SetSkipArrowActive(bool _Value);

	// ����
	bool IsWriteVisibleAreaEnd() const;
	bool IsWriteMessageEnd() const;
	void ScrollUp();
	bool IsScrollEnd();

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// ���� ���
	AImageElement* Background = nullptr;
	AImageElement* NextMsgArrow = nullptr;
	AText* Message = nullptr;

	// ������
	int LineSpace = 0;
	int LineCount = 0;
	int CurLine = 0;
	std::wstring Message;

	// ����
	void SequentialRender();
};

