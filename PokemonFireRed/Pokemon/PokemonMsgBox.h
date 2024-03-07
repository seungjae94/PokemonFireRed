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

	// 초기 설정
	void SetBaseRenderingOrder(ERenderingOrder _Order);
	void SetBackgroundImage(std::string_view _ImageName);
	void SetTextColor(EFontColor _Color);
	void SetWriteSpeed(float _WriteSpeed);
	void SetLineSpace(int _LineSpace);

	// 렌더링
	void SetMessage(std::wstring _Message);
	void ShowSkipArrow();
	void HideSkipArrow();

	// 로직
	void Write();
	EWriteState GetWriteState() const
	{
		return State;
	}

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 상태
	EWriteState State = EWriteState::None;

	// 하위 요소
	AImageElement* Background = nullptr;
	AImageElement* NextMsgArrow = nullptr;		// TODO: 애니메이션으로 제작
	AText* Text = nullptr;

	// 상수
	const float ScrollTime = 1.0f;
	const float WriteInterval = 1 / 60.0f;
	
	// 데이터
	float Timer = 0.0f;
	float WriteSpeed = 1.0f;
	int CurLine = 0;
	int GlyphIndex = 0;
	std::wstring Message;
	FVector TextInitPos;
	FVector TextPrevPos;
	FVector TextNextPos;

	// 상태 틱 함수
	void ProcessWriting();
	void ProcessScrolling();
};

