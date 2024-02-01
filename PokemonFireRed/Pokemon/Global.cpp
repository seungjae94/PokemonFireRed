#include "Global.h"

const int Global::PIXEL_SIZE = 3;
const int Global::SCREEN_X = (240 * PIXEL_SIZE);
const int Global::SCREEN_Y = (160 * PIXEL_SIZE);
const int Global::IMAGE_TILE_SIZE = 16;
const int Global::TILE_SIZE = (IMAGE_TILE_SIZE * PIXEL_SIZE);
const int Global::HALF_SCREEN_X = (SCREEN_X / 2);
const int Global::HALF_SCREEN_Y = (SCREEN_Y / 2);

const float Global::F_PIXEL_SIZE = static_cast<float>(3);
const float Global::F_SCREEN_X = static_cast<float>(240 * PIXEL_SIZE);
const float Global::F_SCREEN_Y = static_cast<float>(160 * PIXEL_SIZE);
const float Global::F_IMAGE_TILE_SIZE = static_cast<float>(16);
const float Global::F_TILE_SIZE = static_cast<float>(IMAGE_TILE_SIZE * PIXEL_SIZE);
const float Global::F_HALF_SCREEN_X = static_cast<float>(SCREEN_X / 2);
const float Global::F_HALF_SCREEN_Y = static_cast<float>(SCREEN_Y / 2);