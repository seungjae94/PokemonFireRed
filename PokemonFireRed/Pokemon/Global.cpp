#include "Global.h"
#include <Windows.h>

const float Global::CHARACTER_WALK_SPEED = 3.6f;
const float Global::CHARACTER_JUMP_SPEED = 1.4f;
const float Global::CHARACTER_WALK_ANIMATION_FRAME_INTERVAL = 1.0f / 4;
const float Global::CHARACTER_JUMP_ANIMATION_FRAME_INTERVAL = 1.0f / 48;

const int Global::PIXEL_SIZE = 3;
const int Global::MAP_RUNTIME_SCALE_FACTOR = 1;
const int Global::SCREEN_X = (240 * PIXEL_SIZE);
const int Global::SCREEN_Y = (160 * PIXEL_SIZE);
const int Global::IMAGE_TILE_SIZE = 16;
const int Global::TILE_SIZE = (IMAGE_TILE_SIZE * PIXEL_SIZE);
const int Global::HALF_SCREEN_X = (SCREEN_X / 2);
const int Global::HALF_SCREEN_Y = (SCREEN_Y / 2);

const float Global::F_PIXEL_SIZE = static_cast<float>(3);
const float Global::F_MAP_RUNTIME_SCALE_FACTOR = static_cast<float>(MAP_RUNTIME_SCALE_FACTOR);
const float Global::F_SCREEN_X = static_cast<float>(240 * PIXEL_SIZE);
const float Global::F_SCREEN_Y = static_cast<float>(160 * PIXEL_SIZE);
const float Global::F_IMAGE_TILE_SIZE = static_cast<float>(16);
const float Global::F_TILE_SIZE = static_cast<float>(IMAGE_TILE_SIZE * PIXEL_SIZE);
const float Global::F_HALF_SCREEN_X = static_cast<float>(SCREEN_X / 2);
const float Global::F_HALF_SCREEN_Y = static_cast<float>(SCREEN_Y / 2);

const int Global::MONITOR_X = GetSystemMetrics(SM_CXSCREEN);
const int Global::MONITOR_Y = GetSystemMetrics(SM_CYSCREEN);
const int Global::HALF_MONITOR_X = MONITOR_X / 2;
const int Global::HALF_MONITOR_Y = MONITOR_Y / 2;

const FVector Global::HALF_SCREEN = FVector(HALF_SCREEN_X, HALF_SCREEN_Y);