#include "IO.h"
#include <ncurses.h>
#include <cstring>
#include <unistd.h>

namespace uc
{

IO::IO(Terminal* Terminal, WINDOW* WindowHandle)
{
	_WindowHandle = WindowHandle;
	_Terminal = Terminal;
}

IO::IO(Terminal* Terminal)
{
	_WindowHandle = stdscr;
	_Terminal = Terminal;
}

IO::~IO()
{

}

void IO::WriteChar(const char Out, bool Refresh)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			waddch(_WindowHandle, Out);
			if (Refresh) wrefresh(_WindowHandle);
		}
		else
		{
			addch(Out);
			if (Refresh) refresh();
		}

		_Terminal->Unlock();
	}
}

void IO::WriteString(const std::string Out, bool Refresh)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			waddstr(_WindowHandle, Out.c_str());
			if (Refresh) wrefresh(_WindowHandle);
		}
		else
		{
			addstr(Out.c_str());
			if (Refresh) refresh();
		}

		_Terminal->Unlock();
	}
}

IO& IO::operator<<(const char Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			waddch(_WindowHandle, Out);
			if (Out == '\n') wrefresh(_WindowHandle);
		}
		else
		{
			addch(Out);
			if (Out == '\n') refresh();
		}

		_Terminal->Unlock();
	}

	return *this;
}

IO& IO::operator<<(const std::string& Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			waddstr(_WindowHandle, Out.c_str());
			if (Out == uc::endl) wrefresh(_WindowHandle);
		}
		else
		{
			addstr(Out.c_str());
			if (Out == uc::endl) refresh();
		}

		_Terminal->Unlock();
	}

	return *this;
}

IO& IO::operator<<(const char* Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			waddstr(_WindowHandle, Out);
			if (strcmp(Out, uc::endl.c_str()) == 0) wrefresh(_WindowHandle);
		}
		else
		{
			addstr(Out);
			if (strcmp(Out, uc::endl.c_str()) == 0) refresh();
		}

		_Terminal->Unlock();
	}

	return *this;
}

IO& IO::operator<<(const int Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{

		if (_WindowHandle)
			wprintw(_WindowHandle, "%d", Out);
		else
			printw("%d", Out);

		_Terminal->Unlock();
	}

	return *this;
}

IO& IO::operator<<(const unsigned int Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
			wprintw(_WindowHandle, "%d", Out);
		else
			printw("%d", Out);

		_Terminal->Unlock();
	}

	return *this;
}

IO& IO::operator<<(const long unsigned int Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
			wprintw(_WindowHandle, "%d", Out);
		else
			printw("%d", Out);

		_Terminal->Unlock();
	}

	return *this;
}


void IO::Flush()
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			nodelay(_WindowHandle, true);
			while (wgetch(_WindowHandle) != ERR) ;
			nodelay(_WindowHandle, false);
		}
		else
		{
			nodelay(stdscr, true);
			while (getch() != ERR) ;
			nodelay(stdscr, false);
		}

		_Terminal->Unlock();
	}
}

int IO::ReadChar()
{
	int Value = 0;
	Key Success = ReadChar(&Value);

	if (Success != None)
		Value = 0;

	return Value;
}

Key IO::ReadChar(int* Value, bool NonBlocking)
{
	Key KeyValue = Key::Error;

	if (_Terminal)
	{
		if (_WindowHandle)
		{
			do
			{
				usleep(25000);
				_Terminal->WaitForInput(50000);
				if (true)
				{
					if (_Terminal->FocusAndLock())
					{
						nodelay(_WindowHandle, true);
						*Value = wgetch(_WindowHandle);
						nodelay(_WindowHandle, false);
						_Terminal->Unlock();
					}

					switch(*Value)
					{
					case ERR:
						KeyValue = Key::Error;
						break;
					case KEY_UP:
						KeyValue = Key::Up;
						break;
					case KEY_DOWN:
						KeyValue = Key::Down;
						break;
					case KEY_RIGHT:
						KeyValue = Key::Right;
						break;
					case KEY_LEFT:
						KeyValue = Key::Left;
						break;
					case 9:			// KEY_TAB
						KeyValue = Key::Tab;
						break;
					case KEY_BACKSPACE:
						KeyValue = Key::Backspace;
						break;
					default:
						KeyValue = Key::None;
						break;
					}
				}
			}
			while(*Value == ERR && !NonBlocking);
		}
	}

	return KeyValue;
}

std::string IO::ReadString(bool NonBlocking)
{
	static char Cin[1024];
	*Cin = '\0';

	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
			wgetstr(_WindowHandle, Cin);

		_Terminal->Unlock();
	}

	return std::string(Cin);
}

std::string IO::ReadPassword(bool NonBlocking)
{
	std::string Pass = "";
	int Char;
	int x, y;

	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
		{
			noecho();
			while ((Char = wgetch(_WindowHandle)) != '\n')
			{
				switch (Char)
				{
				case KEY_BACKSPACE:
					if (Pass.length() > 0)
					{
						getyx(_WindowHandle, y, x);
						Pass.pop_back();
						wmove(_WindowHandle, y, --x);
						waddch(_WindowHandle, ' ');
						wmove(_WindowHandle, y, --x);
					}
					break;
				default:
					Pass += Char;
					WriteChar('*');
				}
				wrefresh(_WindowHandle);
			}
			waddch(_WindowHandle, '\n');
			echo();
		}

		_Terminal->Unlock();
	}

	return Pass;
}


}
