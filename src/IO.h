#pragma once

#include "Defines.h"
#include <string>

// Forward-declarations of ncurses.h
typedef struct _win_st WINDOW;

namespace uc
{

class IO
{
private:
	WINDOW* _WindowHandle;

protected:
	void SetWindowHandle(WINDOW* Handle);

public:
	IO();
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
	bool ReadCharNoDelay(int* Value);
	int ReadCharOrKey(int* Value);
	int ReadCharOrKeyNoDelay(int* Value);
	std::string ReadString();
	std::string ReadPassword();
	IO& operator>>(int& In);
	IO& operator>>(std::string& In);
};

}
