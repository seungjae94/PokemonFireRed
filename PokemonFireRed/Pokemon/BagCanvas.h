#pragma once
#include "Canvas.h"
#include <list>

struct FInventoryRecord;

class ABagCanvas : public ACanvas
{
public:
	// constructor destructor
	ABagCanvas();
	~ABagCanvas();

	// delete Function
	ABagCanvas(const ABagCanvas& _Other) = delete;
	ABagCanvas(ABagCanvas&& _Other) noexcept = delete;
	ABagCanvas& operator=(const ABagCanvas& _Other) = delete;
	ABagCanvas& operator=(ABagCanvas&& _Other) noexcept = delete;

	void SetBackground(std::string_view _ImageName);
	void SetItemImage(std::string_view _ImageName);
	void SetItemExplain(std::wstring_view _Explain);
	void SetTargetCursor(int _Cursor);
	void RefreshList(const std::list<FInventoryRecord>& _Records);

	void SetActionItemBoxActive(bool _Value);
	void SetActionItemText(std::wstring_view _Text);
	int GetActionCursor();
	void SetActionCursor(int _Cursor);
	void IncActionCursor();
	void DecActionCursor();
protected:

private:
	void BeginPlay() override;

	AImageElement* Background = nullptr;
	AImageElement* LeftArrow = nullptr;
	AImageElement* RightArrow = nullptr;
	AImageElement* ItemImage = nullptr;
	AText* ItemExplain = nullptr;
	ACursor* TargetCursor = nullptr;
	std::vector<AText*> ItemNames;
	std::vector<AText*> ItemCrosses;
	std::vector<AText*> ItemCounts;

	AImageElement* ActionItemBox = nullptr;
	AImageElement* ActionBox = nullptr;
	AText* ActionItemText = nullptr;
	ACursor* ActionCursor = nullptr;
};

