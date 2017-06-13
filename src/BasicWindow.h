#pragma once

#include "Defines.h"
#include "Terminal.h"
#include <string>

namespace uc
{

class BasicWindow
{
private:
	WINDOW* _WindowHandle;
	Terminal* _Terminal;
	bool _IsStdScr;

	Color _BgColor, _FgColor;
	int _Height, _Width, _PosX, _PosY;

public:
	BasicWindow(Terminal* Terminal);
	BasicWindow(Terminal* Terminal, int Height, int Width, int PosX, int PosY);
	~BasicWindow();

	int WindowHeight() {return _Height;}
	int WindowWidth() {return _Width;}
	int WindowPositionX() {return _PosX;}
	int WindowPositionY() {return _PosY;}
	Point WindowPosition() {return (Point){_PosX, _PosY};}

	void Clear(); // not yet implemented
	void Scroll(bool On); // not yet implemented
	void Refresh(); // not yet implemented

	Point CursorPosition(); // not yet implemented
	void CursorSet(Point NewPosition); // not yet implemented
	void CursorSet(int NewX, int NewY); // not yet implemented
	void CursorMove(Point Diff); // not yet implemented
	void CursorMove(int DiffX, int DiffY); // not yet implemented

	void WriteChar(const char Out, bool Refresh = true);
	void WriteString(const std::string Out, bool Refresh = true);
	BasicWindow& operator<<(const char Out);
	BasicWindow& operator<<(const std::string& Out);
	BasicWindow& operator<<(const char* Out);
	BasicWindow& operator<<(const int Out);
	BasicWindow& operator<<(const unsigned int Out);
	BasicWindow& operator<<(const long unsigned int Out);

	void Flush();
	int ReadChar();
	Key ReadChar(int* Value, bool Blocking = true);
	Key ReadLineOrKey(std::string& Input, std::string::iterator& InputCursorPos, Point StartOfLine, EchoStatus Echo = Enabled, bool Blocking = true);
	std::string ReadString(bool Blocking = true);
	std::string ReadPassword(bool Blocking = true);

	void DrawBorder(); // not yet implemented
	void DrawBorder(char Corner, char Vertical, char Horizontal); // not yet implemented
	void SetBackground(Color Background);
	void EnableColors(Color Foreground, Color Background);
	void DisableColors();

	void SetTextBold(bool On); // not yet implemented
	void SetTextUnderlined(bool On); // not yet implemented
};

}
