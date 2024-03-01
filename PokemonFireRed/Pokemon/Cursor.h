#pragma once
#include "UIElement.h"
#include "PokemonMath.h"

class ACursor : public AUIElement
{
public:
	// constructor destructor
	ACursor();
	~ACursor();

	// delete Function
	ACursor(const ACursor& _Other) = delete;
	ACursor(ACursor&& _Other) noexcept = delete;
	ACursor& operator=(const ACursor& _Other) = delete;
	ACursor& operator=(ACursor&& _Other) noexcept = delete;

	int GetCursor() const
	{
		return Cursor;
	}

	void SetCursor(int _Cursor);

	void SetImage(std::string_view _ImageName)
	{
		ImageName = _ImageName;
	}

	void SetPixelGap(int _PixelGap)
	{
		PixelGap = _PixelGap;
	}

	void SetOptionCount(int _OptionCount)
	{
		OptionCount = _OptionCount;
		SetCursor(Cursor);
	}

	void SetCursorPositions(const std::vector<FVector>& _CursorPositions)
	{
		CursorPositions = _CursorPositions;
		SetCursor(Cursor);
	}

	void IncCursor()
	{
		int NewCursor = UPokemonMath::Mod(Cursor + 1, OptionCount);
		SetCursor(NewCursor);
	}

	void DecCursor()
	{
		int NewCursor = UPokemonMath::Mod(Cursor - 1, OptionCount);
		SetCursor(NewCursor);
	}

	bool IsLast() const;

	void FollowParentPosition() override;

protected:

private:
	std::string ImageName = RN::BlackCursor;
	UImageRenderer* Renderer = nullptr;

	std::vector<FVector> CursorPositions;

	int Cursor = 0;
	int OptionCount = 0;
	int PixelGap = 16;

	bool HasCancel = false;
	int CancelPixelX = 0;
	int CancelPixelY = 0;
};

