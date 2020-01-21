#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <set>

#include "Terminal.h"

namespace uc
{
	int Terminal::_Instances = 0;
	Terminal* Terminal::_StdTerminal = 0;
	std::mutex Terminal::_Lock;
	Terminal* Terminal::_Locker = 0;

	void Terminal::InitializeCurses()
	{
		// enable use of colors in this ncurses-session
		start_color();
		
		// assign terminal default foreground/background colors to color number -1
		use_default_colors();
		
		// characters (including interrupt, quit, suspend and flow control characters)
		// typed are immediately passed through to the user program
		raw();

		// color-management is done like the following:
		// there are 8 basic colors defined in uc::Color (see Defines.h)
		// ncurses handles colors in pairs (foreground and background)
		// before any color-pair can be used it has to be initialized with
		// init_pair(). All theoretical combinations of uc::Color (8*8=64)
		// in fore- and background will be initialized in advanced here
		for (int fgc = 0; fgc < 8; fgc++)
		{
			for (int bgc = 0; bgc < 8; bgc++)
				init_pair(((fgc * 8) + bgc), fgc, bgc);
		}

		// there is a 9th color: uc::Color::Default (value -1) which is 
		// whatever color it was before. E.g. if one want to set only the
		// foreground color without knowing about the current background
		// color he would use one of the pairs between 73 and 81

		// Initialize all combinations with default as background-color
		for (int fgc = 0; fgc < 8; fgc++)
			init_pair(65 + fgc, fgc, -1);

		// Initialize all combinations with default as foreground-color
		for (int bgc = 0; bgc < 8; bgc++)
			init_pair(73 + bgc, -1, bgc);

		// Initialize default color-pair with value 81
		init_pair(81, -1, -1);
	}

	Terminal::Terminal()
	{
		// at some point in the future there might be a constructor which
		// takes a tty for a different terminal-hosting process which
		// can be set here. For now the only instance allowed is the one
		// for the calling process. Hence this is a singleton constructor
		_StdTerminal = this;

		// to prevent some race-condition or else issues in a multithreaded
		// environment the first call will lock the mutex before initializing
		// ncurses
		_Lock.lock();
		_TerminalHandle = newterm(0, stdout, stdin);
		InitializeCurses();
		_Lock.unlock();
	}

	Terminal& Terminal::GetStdTerminal()
	{
		// already initialized? --> don't initialize twice, instead
		// use the already set up instance
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
		}
	}

	bool Terminal::WaitForInput(long us)
	{
		// this rather complex snippet for simply waiting some time
		// will return not only once the time (us) run out, but also
		// if something is happening on the input (stdin). This way
		// applications could wait for user-input, but only for a
		// given time before they do something else

		fd_set fds;
		FD_ZERO(&fds);

		FD_SET(stdin->_fileno, &fds);
		struct timeval tv;

		tv.tv_sec = us / 1000000;
		tv.tv_usec = us % 1000000;

		int status = select(1, &fds, NULL, NULL, &tv);

		return (status == 1);
	}

	bool Terminal::FocusAndLock()
	{
		bool Success = false;

		if (_Locker == this)
			Success = true;
		else if (_TerminalHandle)
		{
			Success = _Lock.try_lock();

			if (Success)
			{
				// for the current implementation setting focus is not
				// really necessary since there is only one instance of
				// a ncurses-terminal (the one which is bound to the 
				// calling terminal-process). One possible future enhance-
				// ment might be the support for multiple terminal-
				// instances (on different terminal-hosting processes).
				// to support this either in a multi- or single-threaded 
				// environment this set_term call together with locking
				// a mutex is key.
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
			_Lock.unlock();
			_Locker = NULL;
		}
	}

	void Terminal::Refresh()
	{
		if (FocusAndLock())
		{
			// this call will render all the modifications on the
			// terminal output done since the last refresh()
			refresh();
			Unlock();
		}
	}

	int Terminal::Height()
	{
		int Value = 0;

		if (FocusAndLock())
		{
			// the variable LINES is supported in ncurses and will
			// return the current amount of lines in the focussed 
			// terminal
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
			// the variable COLS is supported in ncurses and will
			// return the current amount of columns in the focussed 
			// terminal
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
