#pragma once
#include "Canvas.h"

class AFadeCanvas : public ACanvas
{
public:
	// constructor destructor
	AFadeCanvas();
	~AFadeCanvas();

	// delete Function
	AFadeCanvas(const AFadeCanvas& _Other) = delete;
	AFadeCanvas(AFadeCanvas&& _Other) noexcept = delete;
	AFadeCanvas& operator=(const AFadeCanvas& _Other) = delete;
	AFadeCanvas& operator=(AFadeCanvas&& _Other) noexcept = delete;

protected:

private:

};

