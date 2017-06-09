# pragma once

#include "Defines.h"
#include "Mutex.h"
#include <string>

// Forward-declarations of ncurses.h
typedef struct screen SCREEN;

namespace uc
{

class Terminal
{
private:
	static Terminal* _StdTerminal;
	static int _Instances;
	static mt::Mutex _Lock;

	SCREEN* _TerminalHandle;
	FILE* _io;
	std::string _TTY;

protected:
	Terminal();

public:
	static Terminal& GetStdTerminal();
	static void EndStdTerminal();
	Terminal(std::string TTY);
	~Terminal();

	bool WaitForInput(long us);
	bool FocusAndLock();
	void Unlock();
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
