#pragma once
#include "Canvas.h"

class AShopCanvas : public ACanvas
{
public:
	// constructor destructor
	AShopCanvas();
	~AShopCanvas();

	// delete Function
	AShopCanvas(const AShopCanvas& _Other) = delete;
	AShopCanvas(AShopCanvas&& _Other) noexcept = delete;
	AShopCanvas& operator=(const AShopCanvas& _Other) = delete;
	AShopCanvas& operator=(AShopCanvas&& _Other) noexcept = delete;

protected:

private:

};

