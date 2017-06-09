#include "Terminal.h"
#include <ncurses.h>
#include <iostream>

namespace uc
{

int Terminal::_Instances = 0;
Terminal* Terminal::_StdTerminal = 0;
Terminal* Terminal::_ActiveTerminal = 0;

Terminal::Terminal()
{
	_io = NULL;
	_TerminalHandle = newterm(0, stdout, stdin);
	_StdTerminal = this;
	_TTY = "";

	set_term(_TerminalHandle);
	SetWindowHandle(stdscr);
}

Terminal::Terminal(std::string TTY)
{
	_io = fopen(TTY.c_str(), "w+");
	_TerminalHandle = newterm(NULL, _io, _io);
	_TTY = TTY;

	set_term(_TerminalHandle);
	SetWindowHandle(stdscr);
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
		SetActive();
		endwin();
		delscreen(_TerminalHandle);
	}

	if (_io)
		fclose(_io);
}

void Terminal::SetActive()
{
	if (_ActiveTerminal != this && _TerminalHandle)
	{
		set_term(_TerminalHandle);
		_ActiveTerminal = this;
	}
}

//void Terminal::WriteChar(const char Out)
//{
//	SetActive();
//	addch(Out);
//	Refresh();
//}
//
//void Terminal::WriteString(std::string Out)
//{
//	SetActive();
//	addstr(Out.c_str());
//	Refresh();
//}
//
//std::string Terminal::ReadString()
//{
//	static char In[1024];
//	getstr(In);
//	return std::string(In);
//}
//
//int Terminal::ReadChar()
//{
//	SetActive();
//	return getch();
//}

void Terminal::Refresh()
{
	SetActive();
	refresh();
}

int Terminal::Height()
{
	SetActive();
	return LINES;
}

int Terminal::Width()
{
	SetActive();
	return COLS;
}

void Terminal::SaveScreen(std::string SaveName)
{

}

void Terminal::RestoreScreen(std::string SaveName)
{

}

void Terminal::ClearScreen()
{
	SetActive();
	clear();
	Refresh();
}

void Terminal::Echo(bool On)
{
	SetActive();
	if (On) echo(); else noecho();
}

void Terminal::CursorVisibility(bool On)
{
	SetActive();
	if (On) curs_set(1); else curs_set(0);
}


}
