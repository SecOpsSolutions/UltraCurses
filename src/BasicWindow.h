#pragma once

#include "Defines.h"
#include "Terminal.h"
#include <string>
#include <map>

namespace uc
{

class BasicWindow
{
private:
	static std::map<Terminal*, BasicWindow*> _StdWindows;
	WINDOW* _WindowHandle;
	Terminal* _Terminal;
	bool _IsStdScr;
	std::string _ScreenSaveName;

	Color _BgColor, _FgColor;
	int _Height, _Width, _PosX, _PosY;

public:
	BasicWindow(Terminal* Terminal);
	BasicWindow(Terminal* Terminal, int Height, int Width, int PosX, int PosY);
	~BasicWindow();

	static BasicWindow& GetStdWindow();

	int WindowHeight() {return _Height;}
	int WindowWidth() {return _Width;}
	int WindowPositionX() {return _PosX;}
	int WindowPositionY() {return _PosY;}
	Point WindowPosition() {return (Point){_PosX, _PosY};}

	void Clear();
	void Scroll(bool On);
	void Refresh();

	Point CursorPosition();
	void CursorSet(Point NewPosition);
	void CursorSet(int NewX, int NewY);
	void CursorMove(Point Diff);
	void CursorMove(int DiffX, int DiffY);

	void WriteChar(const char Out, bool Refresh = true);
	void WriteString(const std::string Out, bool Refresh = true);
	BasicWindow& operator<<(const char Out);
	BasicWindow& operator<<(const std::string& Out);
	BasicWindow& operator<<(const char* Out);
	BasicWindow& operator<<(const int Out);
	BasicWindow& operator<<(const unsigned int Out);
	BasicWindow& operator<<(const long unsigned int Out);
	BasicWindow& operator<<(double Out);

	void Flush();
	unsigned long ReadUnsignedLong();
	long ReadLong();
	unsigned int ReadUnsignedInt();
	int ReadInt();
	int ReadChar();
	Key ReadChar(int* Value, bool Blocking = true);
	Key ReadLineOrKey(std::string& Input, std::string::iterator& InputCursorPos, Point StartOfLine, EchoStatus Echo = Enabled, bool Blocking = true);
	std::string ReadHiddenString(bool Blocking = true);
	std::string ReadString(bool Blocking = true);
	std::string ReadPassword(bool Blocking = true);

	void DrawBorder();
	void DrawBorder(char Corner, char Vertical, char Horizontal);
	void SetBackground(Color Background);
	void EnableColors(Color Foreground, Color Background);
	void DisableColors();

	void SetTextBold(bool On);
	void SetTextUnderlined(bool On);
};

}
