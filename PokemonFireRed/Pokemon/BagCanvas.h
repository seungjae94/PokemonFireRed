#pragma once
#include "Canvas.h"

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

	void Init() {}

protected:

private:
	void BeginPlay() override;

	AImageElement* Background = nullptr;
	AImageElement* LeftArrow = nullptr;
	AImageElement* RightArrow = nullptr;
	AImageElement* ItemImage = nullptr;
	AText* ItemExplain = nullptr;
	std::vector<AText*> ItemNames;
	std::vector<AText*> ItemCrosses;
	std::vector<AText*> ItemCounts;
	std::vector<ACursor*> TargetCursors;
};

