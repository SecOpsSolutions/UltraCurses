#include "uCurses.h"
#include "Thread.h"
#include <unistd.h>
#include <sstream>

class Worker: public mt::Thread
{
private:
	std::string _tty;
	long _us;

	void Run()
	{
		uc::Terminal T(_tty);
		uc::BasicWindow io(&T);

		io.Scroll(true);
		//io.SetBackground(uc::Color::Magenta);
		io.EnableColors(uc::Color::Blue, uc::Color::Yellow);

		unsigned long count = 0;

		while (!_RequestStop)
		{
			std::stringstream out;
			out << "[" << count++ << "] " << _tty << "\n";
			io.WriteString(out.str());
			TimedWait(_us);
		}
		io.WriteString("Thread stop\n");
	}

public:
	Worker(std::string tty, long us): Thread()
{
		_tty = tty;
		_us = us;
}
};

int main()
{
	Worker W1("/dev/pts/1", 750000);
	Worker W2("/dev/pts/2", 500000);

	uc::BasicWindow io(&(uc::Terminal::GetStdTerminal()));

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

	W1.RequestStop();
	W2.RequestStop();

	W1.WaitForStop();
	W2.WaitForStop();

	uc::end();
}

