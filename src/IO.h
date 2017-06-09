#pragma once

#include "Defines.h"
#include "Terminal.h"
#include <string>

// Forward-declarations of ncurses.h
typedef struct _win_st WINDOW;
typedef struct screen SCREEN;

namespace uc
{

class IO
{
private:
	WINDOW* _WindowHandle;
	Terminal* _Terminal;

public:
	IO(Terminal* Terminal);
	IO(Terminal* Terminal, WINDOW* WindowHandle);
	~IO();

	void WriteChar(const char Out, bool Refresh = true);
	void WriteString(const std::string Out, bool Refresh = true);
	IO& operator<<(const char Out);
	IO& operator<<(const std::string& Out);
	IO& operator<<(const char* Out);
	IO& operator<<(const int Out);
	IO& operator<<(const unsigned int Out);
	IO& operator<<(const long unsigned int Out);

	void Flush();
	int ReadChar();
	Key ReadChar(int* Value, bool NonBlocking = false);
	std::string ReadString(bool NonBlocking = false);
	std::string ReadPassword(bool NonBlocking = false);
};

}
