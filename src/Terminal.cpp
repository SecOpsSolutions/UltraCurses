#include "Terminal.h"
#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace uc
{
int Terminal::_Instances = 0;
Terminal* Terminal::_StdTerminal = 0;
mt::Mutex Terminal::_Lock;
Terminal* Terminal::_Locker = 0;

void Terminal::InitializeCurses()
{
	start_color();
	use_default_colors();
	raw();

	for (int fgc = 0; fgc < 8; fgc++)
	{
		for (int bgc = 0; bgc < 8; bgc++)
			init_pair(((fgc * 8) + bgc), fgc, bgc);
	}

	for (int fgc = 0; fgc < 8; fgc++)
		init_pair(65 + fgc, fgc, -1);

	for (int bgc = 0; bgc < 8; bgc++)
		init_pair(73 + bgc, -1, bgc);

	init_pair(81, -1, -1);
}

Terminal::Terminal()
{
	_io = NULL;
	_StdTerminal = this;
	_TTY = "";
	_Instances++;

	_Lock.Lock();
	_TerminalHandle = newterm(0, stdout, stdin);
	InitializeCurses();
	_Lock.UnLock();
}

Terminal::Terminal(std::string TTY)
{
	_io = fopen(TTY.c_str(), "w+");
	_TTY = TTY;
	_Instances++;

	_Lock.Lock();
	_TerminalHandle = newterm(NULL, _io, _io);
	InitializeCurses();
	_Lock.UnLock();
}

Terminal& Terminal::GetStdTerminal()
{
	if (!_StdTerminal)
		_StdTerminal = new Terminal();

	return *_StdTerminal;
}

void Terminal::EndStdTerminal()
{
	if (!_StdTerminal)
		return;

	delete _StdTerminal;
	_StdTerminal = NULL;
}

Terminal::~Terminal()
{
	if (_TerminalHandle)
	{
		if (FocusAndLock())
		{
			endwin();
			delscreen(_TerminalHandle);
			Unlock();
		}

		_TerminalHandle = 0;
		--_Instances;
	}

	if (_io)
		fclose(_io);
}

bool Terminal::WaitForInput(long us)
{
	fd_set fds;
	FD_ZERO(&fds);

	if (_io)
		FD_SET(_io->_fileno, &fds);
	else
		FD_SET(stdin->_fileno, &fds);
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = us;

	int status = select(1, &fds, NULL, NULL, &tv);

	if (status == 1)
		return true;
	else
		return false;
}

bool Terminal::FocusAndLock()
{
	bool Success = false;

	if (_Locker == this)
		Success = true;
	else if (_TerminalHandle)
	{
		Success = _Lock.Lock();

		if (Success)
		{
			set_term(_TerminalHandle);
			_Locker = this;
		}
	}

	return Success;
}

void Terminal::Unlock()
{
	if (_Locker != NULL)
	{
		_Lock.UnLock();
		_Locker = NULL;
	}
}

void Terminal::Refresh()
{
	if (FocusAndLock())
	{
		refresh();
		Unlock();
	}
}

int Terminal::Height()
{
	int Value = 0;

	if (FocusAndLock())
	{
		Value = LINES;
		Unlock();
	}

	return Value;
}

int Terminal::Width()
{
	int Value = 0;

	if (FocusAndLock())
	{
		Value = COLS;
		Unlock();
	}

	return Value;
}

void Terminal::SaveScreen(std::string SaveName)
{
	if (FocusAndLock())
	{
		scr_dump(SaveName.c_str());
		Unlock();
	}
}

void Terminal::RestoreScreen(std::string SaveName)
{
	// TODO: It would be nice to have this function remove the dump-file after restoring.

	if (FocusAndLock())
	{
		scr_restore(SaveName.c_str());
		refresh();
		Unlock();
	}
}

void Terminal::ClearScreen()
{
	if (FocusAndLock())
	{
		clear();
		refresh();
		Unlock();
	}
}

void Terminal::Echo(bool On)
{
	if (FocusAndLock())
	{
		if (On) echo(); else noecho();
		Unlock();
	}
}

void Terminal::CursorVisibility(bool On)
{
	if (FocusAndLock())
	{
		if (On) curs_set(1); else curs_set(0);
		Unlock();
	}
}


}
