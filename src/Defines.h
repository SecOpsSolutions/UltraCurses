#pragma once

#include <string>

// Forward-declarations of ncurses.h
typedef struct _win_st WINDOW;
typedef struct screen SCREEN;

namespace uc
{

static const std::string endl = "\n";

enum EchoStatus
{
	Disabled = false,
	Enabled = true,
	Password
};

enum Key
{
	Error = -1,
	None = 0,
	Up,
	Down,
	Left,
	Right,
	Tab,
	Backspace,
	Delete
};

enum Color
{
	Default = -1,
	Black = 0, //COLOR_BLACK,
	White = 7, //COLOR_WHITE,
	Red = 1, //COLOR_RED,
	Yellow = 3, //COLOR_YELLOW,
	Magenta = 5, //COLOR_MAGENTA,
	Blue = 4, //COLOR_BLUE,
	Cyan = 6, //COLOR_CYAN,
	Green = 2 //COLOR_GREEN
};

struct Point
{
	int x;
	int y;
};

};
