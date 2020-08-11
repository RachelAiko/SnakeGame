/******************************************************************************
* File:    snakedefs.h
* Class:   COP 2001 202005
* Author:  Rachel Matthews
* Purpose: Header file for global definitions of the Snake Game 
******************************************************************************/
#ifndef SNAKE_DEFS_H
#define SNAKE_DEFS_H

#include <string>

//-----------------------------------------------------------------------------
// Global space - constants, functions, data structures, etc.
//-----------------------------------------------------------------------------
// game frames-per-second rate (FPS) in miliseconds
const float FPS_RATE = 1000 / 60.0f;

//game area boundraies for game objects
const int GAME_WIDTH{ 800 };
const int GAME_HEIGHT{ 400 };

//game boundray and object sizes
const int MARGIN{ 10 };
const int BORDER_SIZE{ 5 };
const int BLOCK_SIZE{ 10 };

// block array index for head of the snake in the body array
const int SNAKE_HEAD{ 0 };

// how many pixels the snake should move in one full frame
const float SNAKE_VELOCITY{ BLOCK_SIZE * 5 };

const int WINDOW_WIDTH{ GAME_WIDTH + 2 * MARGIN + 2 * BORDER_SIZE };
const int WINDOW_HEIGHT{ GAME_HEIGHT + 2 * MARGIN + 2 * BORDER_SIZE };
const std::string WINDOW_TITLE{ "Snake Game\0" };

enum Color
{
	White = 0xFFFFFF00,
	Silver = 0xC0C0C000,
	Gray = 0x80808000,
	Black = 0x00000000,
	Red = 0xFF000000,
	Maroon = 0x80000000,
	Yellow = 0xFFFF0000,
	Olive = 0x80800000,
	Lime = 0x00FF0000,
	Green = 0x00800000,
	Aqua = 0x00FFFF00,
	Teal = 0x00808000,
	Blue = 0x0000FF00,
	Navy = 0x00008000,
	Pink = 0xEB94E500,
	Purple = 0x80008000
};

const Color BORDER_COLOR{ Aqua };
const Color SNAKE_COLOR{ Pink };
const Color FOOD_COLOR{ Lime };

enum Direction
{
	Exit = -1,
	None = 0,
	Up = 1,
	Down = 2,
	Left = 3,
	Right = 4,
};

enum Border
{
	Top,
	Bottom,
	LeftBorder,
	RightBorder,
};

#endif // SNAKE_DEFS_H

