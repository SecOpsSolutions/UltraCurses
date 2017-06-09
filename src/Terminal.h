# pragma once

#include "IO.h"
#include <string>

// Forward-declarations of ncurses.h
typedef struct screen SCREEN;

namespace uc
{

class Terminal: public IO
{
private:
	static Terminal* _ActiveTerminal, * _StdTerminal;
	static int _Instances;

	SCREEN* _TerminalHandle;
	FILE* _io;
	std::string _TTY;

	Terminal();

public:
	static Terminal& GetStdTerminal();
	static void EndStdTerminal();
	Terminal(std::string TTY);
	~Terminal();

	void SetActive();
	void Refresh();

	int Height();
	int Width();

	void SaveScreen(std::string SaveName = "");
	void RestoreScreen(std::string SaveName = "");
	void ClearScreen();
	void Echo(bool On = true);
	void CursorVisibility(bool On = true);
};

}
