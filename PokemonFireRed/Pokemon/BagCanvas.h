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

protected:

private:
	void BeginPlay() override;

	AImageElement* Background = nullptr;
	AImageElement* LeftArrow = nullptr;
	AImageElement* RightArrow = nullptr;
	AImageElement* ItemImage = nullptr;
	


};

