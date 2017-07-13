# pragma once

#include "Defines.h"
#include "Mutex.h"
#include <string>

namespace uc
{

class Terminal
{
private:
	static Terminal* _StdTerminal;
	static int _Instances;

	static mt::Mutex _Lock;
	static Terminal* _Locker;

	SCREEN* _TerminalHandle;
	FILE* _io;
	std::string _TTY;

	void InitializeCurses();

protected:
	Terminal();

public:
	static std::string CreateNewTerminalWindow();
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
