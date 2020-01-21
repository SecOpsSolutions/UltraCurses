#pragma once

#include <string>

/**
 * @brief forward declarations of ncurses
 */
typedef struct _win_st WINDOW;
typedef struct screen SCREEN;

namespace uc
{
	/**
	 * @brief pre-define of a newline. since we love linux \n is
	 * enough most times. Should be changed for special use-cases
	 */
	static const std::string endl = "\n";

	/**
	 * @brief defining different stati for the terminal-echo
	 */
	enum EchoStatus
	{
		Disabled = false,
		Enabled = true,
		Password
	};

	/**
	 * @brief defining some but not all keystrokes
	 */
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

	/**
	 * @brief defining some pretty straightforward colors...
	 */
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

	/**
	 * @brief a point. having a x- and a y-value on the terminal
	 */
	struct Point
	{
		int x;
		int y;
	};

};
