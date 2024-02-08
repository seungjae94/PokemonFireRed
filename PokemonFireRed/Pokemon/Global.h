#pragma once
#include <EngineBase/EngineMath.h>

// 전역으로 사용할 상수를 정의한다.
class Global
{
public:
	static const int PIXEL_SIZE;
	static const int SCREEN_X;
	static const int SCREEN_Y;
	static const int IMAGE_TILE_SIZE;
	static const int TILE_SIZE;
	static const int HALF_SCREEN_X;
	static const int HALF_SCREEN_Y;

	static const float F_PIXEL_SIZE;
	static const float F_SCREEN_X;
	static const float F_SCREEN_Y;
	static const float F_IMAGE_TILE_SIZE;
	static const float F_TILE_SIZE;
	static const float F_HALF_SCREEN_X;
	static const float F_HALF_SCREEN_Y;

	static const int MONITOR_X;
	static const int MONITOR_Y;
	static const int HALF_MONITOR_X;
	static const int HALF_MONITOR_Y;

	static const FVector HALF_SCREEN;

	// delete Function
	Global(const Global& _Other) = delete;
	Global(Global&& _Other) noexcept = delete;
	Global& operator=(const Global& _Other) = delete;
	Global& operator=(Global&& _Other) noexcept = delete;

protected:

private:
	Global() {}
	~Global() {}
};

enum ERenderingOrder
{
	None,
	Background,
	Lower,
	Upper,
	Foreground,
	UI,
	Super
};