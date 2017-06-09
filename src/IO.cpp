#include "IO.h"
#include <ncurses.h>
#include <cstring>

namespace uc
{

void IO::SetWindowHandle(WINDOW* Handle)
{
	_WindowHandle = Handle;
}

IO::IO()
{
	_WindowHandle = NULL;
}

IO::~IO()
{

}

void IO::WriteChar(const char Out, bool Refresh)
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
}

void IO::WriteString(const std::string Out, bool Refresh)
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
}

IO& IO::operator<<(const char Out)
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

	return *this;
}

IO& IO::operator<<(const std::string& Out)
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

	return *this;
}

IO& IO::operator<<(const char* Out)
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

	return *this;
}

IO& IO::operator<<(const int Out)
{
	if (_WindowHandle)
		wprintw(_WindowHandle, "%d", Out);
		else
			printw("%d", Out);

	return *this;
}

IO& IO::operator<<(const unsigned int Out)
{
	if (_WindowHandle)
		wprintw(_WindowHandle, "%d", Out);
		else
			printw("%d", Out);

	return *this;
}

IO& IO::operator<<(const long unsigned int Out)
{
	if (_WindowHandle)
		wprintw(_WindowHandle, "%d", Out);
		else
			printw("%d", Out);

	return *this;
}


void IO::Flush()
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
}

int IO::ReadChar()
{
	int Value = 0;

	if (_WindowHandle)
		Value = wgetch(_WindowHandle);
	else
		Value = getch();

	return Value;
}

bool IO::ReadCharNoDelay(int* Value)
{
	bool Success = false;

	if (_WindowHandle)
	{
		nodelay(_WindowHandle, true);
		Success = !((*Value = wgetch(_WindowHandle)) == ERR);
		nodelay(_WindowHandle, false);
	}

	return Success;
}

int IO::ReadCharOrKey(int* Value)
{
	int KeyValue = Key::None;

	if (_WindowHandle)
	{
		*Value = wgetch(_WindowHandle);

		switch(*Value)
		{
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

	return KeyValue;
}

int IO::ReadCharOrKeyNoDelay(int* Value)
{
	int Success = Error;

	if (_WindowHandle)
	{
		nodelay(_WindowHandle, true);
		*Value = wgetch(_WindowHandle);

		switch(*Value)
		{
		case ERR:
			Success = Error;
			break;
		case KEY_UP:
			Success = Key::Up;
			break;
		case KEY_DOWN:
			Success = Key::Down;
			break;
		case KEY_RIGHT:
			Success = Key::Right;
			break;
		case KEY_LEFT:
			Success = Key::Left;
			break;
		case 9:			// KEY_TAB
			Success = Key::Tab;
			break;
		case KEY_BACKSPACE:
			Success = Key::Backspace;
			break;
		default:
			Success = Key::None;
			break;
		}

		nodelay(_WindowHandle, false);
	}

	return Success;
}

std::string IO::ReadString()
{
	static char Cin[1024];
	*Cin = '\0';

	if (_WindowHandle)
		wgetstr(_WindowHandle, Cin);

	return std::string(Cin);
}

std::string IO::ReadPassword()
{
	std::string Pass = "";
	int Char;
	int x, y;

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

	return Pass;
}

IO& IO::operator>>(int& In)
{
	if (_WindowHandle)
		In = wgetch(_WindowHandle);

	return *this;
}

IO& IO::operator>>(std::string& In)
{
	static char Cin[1024];
	Cin[0] = '\0';

	if (_WindowHandle)
		wgetstr(_WindowHandle, Cin);

	In = std::string(Cin);

	return *this;
}


}
