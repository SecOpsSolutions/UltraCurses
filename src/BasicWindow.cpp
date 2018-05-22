#include <cstring>
#include <unistd.h>
#include <sstream>
#include <ncurses.h>

#include "BasicWindow.h"

namespace uc
{

BasicWindow& BasicWindow::GetStdWindow()
{
	static BasicWindow* stdout = 0;

	if (stdout == 0)
		stdout = new BasicWindow(&(Terminal::GetStdTerminal()));

	return *stdout;
}

BasicWindow::BasicWindow(Terminal* Terminal)
{
	_Terminal = Terminal;

	if (_Terminal->FocusAndLock())
	{
		_Height = LINES;
		_Width = COLS;

		_WindowHandle = stdscr;
		keypad(_WindowHandle, TRUE);
		_Terminal->Unlock();
	}

	_PosX = 0;
	_PosY = 0;
	_IsStdScr = true;
	_FgColor = _BgColor = Color::Default;
}


BasicWindow::BasicWindow(Terminal* Terminal, int Height, int Width, int PosX, int PosY)
{
	_Terminal = Terminal;

	_Height = Height;
	_Width = Width;
	_PosX = PosX;
	_PosY = PosY;

	if (Terminal->FocusAndLock())
	{
		if (_Height > LINES)
			_Height = LINES;
		if (_Width > COLS)
			_Width = COLS;
		if (_PosX > COLS)
			_PosX = COLS;
		if (_PosY > LINES)
			_PosY = LINES;

		_WindowHandle = newwin(Height, Width, PosY, PosX);
		keypad(_WindowHandle, TRUE);
		Terminal->Unlock();
	}

	_IsStdScr = false;
	_FgColor = _BgColor = Color::Default;
}

BasicWindow::~BasicWindow()
{
	if (_WindowHandle)
	{
		if (!_IsStdScr)
			delwin(_WindowHandle);
		_WindowHandle = 0;
	}
}

void BasicWindow::Clear()
{
	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			//clearok(_WindowHandle, true);
			//wrefresh(_WindowHandle);
			werase(_WindowHandle);
			_Terminal->Unlock();
		}
	}
}

void BasicWindow::Scroll(bool On)
{
	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			scrollok(_WindowHandle, On);
			_Terminal->Unlock();
		}
	}
}

void BasicWindow::Refresh()
{
	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			wrefresh(_WindowHandle);
			_Terminal->Unlock();
		}
	}
}


Point BasicWindow::CursorPosition()
{
	Point CurPos;
	CurPos.x = 0;
	CurPos.y = 0;

	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			getyx(_WindowHandle, CurPos.y, CurPos.x);
			_Terminal->Unlock();
		}
	}

	return CurPos;
}

void BasicWindow::CursorSet(Point NewPosition)
{
	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			wmove(_WindowHandle, NewPosition.y, NewPosition.x);
			_Terminal->Unlock();
		}
	}
}

void BasicWindow::CursorSet(int NewX, int NewY)
{
	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			wmove(_WindowHandle, NewY, NewX);
			_Terminal->Unlock();
		}
	}
}

void BasicWindow::CursorMove(Point Diff)
{
	Point CurPos = CursorPosition();
	CurPos.x += Diff.x;
	CurPos.y += Diff.y;

	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock())
		{
			wmove(_WindowHandle, CurPos.y, CurPos.x);
			_Terminal->Unlock();
		}
	}
}

void BasicWindow::CursorMove(int DiffX, int DiffY)
{
	Point Diff;
	Diff.x = DiffX;
	Diff.y = DiffY;
	CursorMove(Diff);
}


void BasicWindow::WriteChar(const char Out, bool Refresh)
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

void BasicWindow::WriteString(const std::string Out, bool Refresh)
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

BasicWindow& BasicWindow::operator<<(const char Out)
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

BasicWindow& BasicWindow::operator<<(const std::string& Out)
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

BasicWindow& BasicWindow::operator<<(const char* Out)
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

BasicWindow& BasicWindow::operator<<(const int Out)
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

BasicWindow& BasicWindow::operator<<(const unsigned int Out)
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

BasicWindow& BasicWindow::operator<<(const long unsigned int Out)
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

BasicWindow& BasicWindow::operator<<(double Out)
{
	if (_Terminal && _Terminal->FocusAndLock())
	{
		if (_WindowHandle)
			wprintw(_WindowHandle, "%lf", Out);
		else
			printw("%lf", Out);
	}

	return *this;
}

void BasicWindow::Flush()
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

unsigned long BasicWindow::ReadUnsignedLong()
{
	std::string Number = ReadString();
	return std::stoul(Number);
}

long BasicWindow::ReadLong()
{
	std::string Number = ReadString();
	return std::stol(Number);
}

unsigned int BasicWindow::ReadUnsignedInt()
{
	std::string Number = ReadString();
	return std::stoul(Number);
}

int BasicWindow::ReadInt()
{
	std::string Number = ReadString();
	return std::stoi(Number);
}

int BasicWindow::ReadChar()
{
	int Value = 0;
	Key Success = ReadChar(&Value);

	if (Success != None)
		Value = 0;

	return Value;
}

Key BasicWindow::ReadChar(int* Value, bool Blocking)
{
	Key KeyValue = Key::Error;

	if (_Terminal)
	{
		if (_WindowHandle)
		{
			do
			{
				if (Blocking)
					_Terminal->WaitForInput(500000);

				if (_Terminal->FocusAndLock())
				{
					nodelay(_WindowHandle, true);
					noecho();
					unsigned long Val = wgetch(_WindowHandle);
					*Value = Val;

					switch(Val)
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
					case KEY_DC:
						KeyValue = Key::Delete;
						break;
					default:
						KeyValue = Key::None;
						break;
					}

					echo();
					nodelay(_WindowHandle, false);
					_Terminal->Unlock();
				}
			}
			while(*Value == ERR && Blocking);
		}
	}

	return KeyValue;
}

Key BasicWindow::ReadLineOrKey(std::string& Input, std::string::iterator& InputCursorPos, Point StartOfLine, EchoStatus Echo, bool Blocking)
{
	Key KeyVal;
	int CharVal;

	if (_WindowHandle)
	{
		if (_Terminal->FocusAndLock() )
		{
			noecho();

			CursorSet(StartOfLine);
			wclrtoeol(_WindowHandle);
			CursorSet(StartOfLine);
			//waddstr(_WindowHandle, Input.c_str());
			if (Echo == Enabled)
				waddstr(_WindowHandle, Input.c_str());
			else if (Echo == Password)
				for (int i = 0; i < Input.length(); ++i) waddch(_WindowHandle, '*');
			CursorSet(StartOfLine.x + std::distance(Input.begin(), InputCursorPos), StartOfLine.y);
			wrefresh(_WindowHandle);

			_Terminal->Unlock();
		}

		do
		{
			KeyVal = ReadChar(&CharVal, Blocking);

			switch (KeyVal)
			{
			case Up:
				return Key::Up;
				break;
			case Down:
				return Key::Down;
				break;
			case Tab:
				return Key::Tab;
				break;
			case None:
				if (CharVal != '\n')
				{
					if (Echo == Disabled)
						Input += (char) CharVal;
					else
						InputCursorPos = Input.insert(InputCursorPos, (char) CharVal) + 1;

					if (_Terminal->FocusAndLock())
					{
						CursorSet(StartOfLine);
						wclrtoeol(_WindowHandle);
						if (Echo == Enabled)
							waddstr(_WindowHandle, Input.c_str());
						else if (Echo == Password)
							for (int i = 0; i < Input.length(); ++i) waddch(_WindowHandle, '*');
						CursorSet(StartOfLine.x + std::distance(Input.begin(), InputCursorPos), StartOfLine.y);
						wrefresh(_WindowHandle);
						_Terminal->Unlock();
					}
				}
				else
				{
					if (_Terminal->FocusAndLock())
					{
						waddch(_WindowHandle, '\n');
						wrefresh(_WindowHandle);
						_Terminal->Unlock();
					}
				}
				break;
			case Backspace:
				if (InputCursorPos > Input.begin() && Echo != Disabled && _Terminal->FocusAndLock())
				{
					--InputCursorPos;

					Input.erase(InputCursorPos);

					CursorSet(StartOfLine);
					wclrtoeol(_WindowHandle);
					if (Echo == Enabled)
						waddstr(_WindowHandle, Input.c_str());
					else if (Echo == Password)
						for (int i = 0; i < Input.length(); ++i) waddch(_WindowHandle, '*');
					CursorSet(StartOfLine.x + std::distance(Input.begin(), InputCursorPos), StartOfLine.y);
					wrefresh(_WindowHandle);
					_Terminal->Unlock();
				}
				break;
			case Delete:
				if (InputCursorPos < Input.end() &&_Terminal->FocusAndLock())
				{
					InputCursorPos = Input.erase(InputCursorPos);


					CursorSet(StartOfLine);
					wclrtoeol(_WindowHandle);
					if (Echo == Enabled)
						waddstr(_WindowHandle, Input.c_str());
					else if (Echo == Password)
						for (int i = 0; i < Input.length(); ++i) waddch(_WindowHandle, '*');
					CursorSet(StartOfLine.x + std::distance(Input.begin(), InputCursorPos), StartOfLine.y);
					wrefresh(_WindowHandle);
					_Terminal->Unlock();
				}
				break;
			case Left:
				if (InputCursorPos > Input.begin() && Echo != Disabled)
				{
					--InputCursorPos;
					if (_Terminal->FocusAndLock())
					{
						CursorMove(-1, 0);
						wrefresh(_WindowHandle);
						_Terminal->Unlock();
					}
				}
				break;
			case Right:
				if (InputCursorPos < Input.end() && Echo != Disabled)
				{
					++InputCursorPos;
					if (_Terminal->FocusAndLock())
					{
						CursorMove(1, 0);
						wrefresh(_WindowHandle);
						_Terminal->Unlock();
					}
				}
				break;
			case Error:
			default:
				break;
			}
		}
		while (KeyVal != Error && CharVal != '\n');
	}

	if (_Terminal->FocusAndLock())
	{
		echo();
		_Terminal->Unlock();
	}

	return KeyVal;
}

std::string BasicWindow::ReadHiddenString(bool Blocking)
{
	std::string Input = "";
	std::string::iterator InputCursorPos = Input.begin();
	Point StartCursorPos = CursorPosition();

	while(ReadLineOrKey(Input, InputCursorPos, StartCursorPos, Disabled, true) != None) ;

	return Input;
}

std::string BasicWindow::ReadString(bool Blocking)
{
	std::string Input = "";
	std::string::iterator InputCursorPos = Input.begin();
	Point StartCursorPos = CursorPosition();

	while(ReadLineOrKey(Input, InputCursorPos, StartCursorPos, Enabled, true) != None) ;

	return Input;
}

std::string BasicWindow::ReadPassword(bool Blocking)
{
	std::string Input = "";
	std::string::iterator InputCursorPos = Input.begin();
	Point StartCursorPos = CursorPosition();

	while(ReadLineOrKey(Input, InputCursorPos, StartCursorPos, Password, true) != None) ;

	return Input;
}

void BasicWindow::DrawBorder()
{
	Point Save;

	if (!_WindowHandle)
		return;

	if (_Terminal->FocusAndLock())
	{
		Save = CursorPosition();
		CursorSet(0, 0);

		box(_WindowHandle, 0, 0);

		CursorSet(Save);
		wrefresh(_WindowHandle);

		_Terminal->Unlock();
	}
}

void BasicWindow::DrawBorder(char Corner, char Vertical, char Horizontal)
{
	Point Save;

	if (!_WindowHandle)
		return;

	if (_Terminal->FocusAndLock())
	{
		Save = CursorPosition();
		CursorSet(0, 0);

		wborder(_WindowHandle, Vertical, Vertical, Horizontal, Horizontal, Corner, Corner, Corner, Corner);

		CursorSet(Save);
		wrefresh(_WindowHandle);

		_Terminal->Unlock();
	}
}

void BasicWindow::SetBackground(Color Background)
{
	if (_WindowHandle && _Terminal->FocusAndLock())
	{
		wbkgd(_WindowHandle, COLOR_PAIR(Background));
		wrefresh(_WindowHandle);
		_Terminal->Unlock();
	}
}

void BasicWindow::EnableColors(Color Foreground, Color Background)
{
	if (_FgColor != Color::Default || _BgColor != Color::Default)
		DisableColors();

	_FgColor = Foreground;
	_BgColor = Background;

	if (!_WindowHandle)
		return;

	if (_Terminal->FocusAndLock())
	{
		if (_FgColor == Color::Default && _BgColor == Color::Default)
			wattron(_WindowHandle, COLOR_PAIR(81));
		else if (_BgColor == Color::Default)
			wattron(_WindowHandle, COLOR_PAIR(65 + _FgColor));
		else if (_FgColor == Color::Default)
			wattron(_WindowHandle, COLOR_PAIR(73 + _BgColor));
		else
			wattron(_WindowHandle, COLOR_PAIR(_FgColor * 8 + _BgColor));

		_Terminal->Unlock();
	}
}

void BasicWindow::DisableColors()
{
	if (_WindowHandle && _Terminal->FocusAndLock())
	{
		wattroff(_WindowHandle, COLOR_PAIR(_FgColor * 8 + _BgColor));
		_Terminal->Unlock();
	}

	_BgColor = _FgColor = Color::Default;
}


void BasicWindow::SetTextBold(bool On)
{
	if (!_WindowHandle)
		return;

	if (_Terminal->FocusAndLock())
	{
		if (On)
			wattron(_WindowHandle, A_BOLD);
		else
			wattroff(_WindowHandle, A_BOLD);

		_Terminal->Unlock();
	}
}

void BasicWindow::SetTextUnderlined(bool On)
{
	if (!_WindowHandle)
		return;

	if (_Terminal->FocusAndLock())
	{
		if (On)
			wattron(_WindowHandle, A_UNDERLINE);
		else
			wattroff(_WindowHandle, A_UNDERLINE);

		_Terminal->Unlock();
	}
}

}
