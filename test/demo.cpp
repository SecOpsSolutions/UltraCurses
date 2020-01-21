#include <unistd.h>
#include <sstream>
#include <dirent.h>
#include <set>

#include "uCurses.h"

int main()
{	
	uc::BasicWindow& io = uc::out();

	// Read standard string from line input
	io.SetTextBold(true);
	io.EnableColors(uc::Color::Red, uc::Color::Default);
	io.WriteString("Input String: ");
	io.DisableColors();
	io.SetTextBold(false);

	std::string Input = io.ReadString();
	io.WriteString("You wrote: '" + Input + "'\n");

	// Read password string from line input
	io.WriteString("Input Password: ");
	io.DisableColors();

	io.EnableColors(uc::Color::White, uc::Color::Black);
	Input.clear();
	Input = io.ReadPassword();
	io.DisableColors();
	io.WriteString("You wrote: '" + Input + "'\n");

	// Read string from input line while echo is disabled (does anyone really need this?)
	io.EnableColors(uc::Color::Red, uc::Color::Blue);
	io.WriteString("Input hidden string: ");

	std::string::iterator InputCursorPos = Input.begin();
	uc::Point StartCursorPos = io.CursorPosition();
	Input.clear();
	while(io.ReadLineOrKey(Input, InputCursorPos, StartCursorPos, uc::EchoStatus::Disabled, true) != uc::Key::None) ;
	io.DisableColors();
	io.WriteString("You wrote: '" + Input + "'\n");

	// Quit program not before asking for permission like the obidient program we are
	io.WriteString("Press key to quit... ");
	int Val;
	io.ReadChar(&Val);

	uc::end();
}