#include "Terminal.h"
#include <ncurses.h>
#include <iostream>

#include <unistd.h>

namespace uc
{

int Terminal::_Instances = 0;
Terminal* Terminal::_StdTerminal = 0;
mt::Mutex Terminal::_Lock;

Terminal::Terminal()
{
	_io = NULL;
	_StdTerminal = this;
	_TTY = "";

	_Lock.Lock();
	_TerminalHandle = newterm(0, stdout, stdin);
	_Lock.UnLock();
}

Terminal::Terminal(std::string TTY)
{
	_io = fopen(TTY.c_str(), "w+");
	_TTY = TTY;

	_Lock.Lock();
	_TerminalHandle = newterm(NULL, _io, _io);
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

	if (_TerminalHandle)
	{
		Success = _Lock.Lock();

		if (Success)
			set_term(_TerminalHandle);
	}

	return Success;
}

void Terminal::Unlock()
{
	_Lock.UnLock();
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

}

void Terminal::RestoreScreen(std::string SaveName)
{

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
