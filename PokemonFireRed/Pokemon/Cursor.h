#pragma once
#include "UIElement.h"
#include "PokemonMath.h"

class ACursor : public AUIElement
{
	friend class ACanvas;
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


	void SetOptionCount(int _OptionCount)
	{
		OptionCount = _OptionCount;
	}

	void IncCursor()
	{
		Cursor = UPokemonMath::Mod(Cursor + 1, OptionCount);
	}

	void DecCursor()
	{
		Cursor = UPokemonMath::Mod(Cursor - 1, OptionCount);
	}

	bool IsLast() const;

	void FollowContainer() override;

protected:

private:
	std::string ImageName = RN::BlackCursor;
	UImageRenderer* Renderer = nullptr;

	int Cursor = 0;
	int OptionCount = 0;
	int PixelGap = 16;

	bool HasCancel = false;
	int CancelPixelX = 0;
	int CancelPixelY = 0;
};

