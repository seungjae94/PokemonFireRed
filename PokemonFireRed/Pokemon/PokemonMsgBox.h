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

	// 초기 설정
	void SetBackgroundImage(std::string_view _ImageName);
	void SetTextColor(EFontColor _Color);
	void SetWriteSpeed(int _WriteSpeed);
	void SetLineSpace(int _LineSpace);

	// 렌더링
	void SetMessage(std::wstring _Message);
	void SetSkipArrowActive(bool _Value);

	// 로직
	bool IsWriteVisibleAreaEnd() const;
	bool IsWriteMessageEnd() const;
	void ScrollUp();
	bool IsScrollEnd();

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 하위 요소
	AImageElement* Background = nullptr;
	AImageElement* NextMsgArrow = nullptr;
	AText* Message = nullptr;

	// 데이터
	int LineSpace = 0;
	int LineCount = 0;
	int CurLine = 0;
	std::wstring Message;

	// 로직
	void SequentialRender();
};

