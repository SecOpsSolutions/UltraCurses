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
		uc::IO io(&T);
		int i = 5;
		while (!_RequestStop && i-- > 0)
		{
			io.WriteString(_tty + "\n");
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
	Worker W1("/dev/pts/1", 1000000);
	Worker W2("/dev/pts/2", 1500000);

	uc::IO io(&(uc::Terminal::GetStdTerminal()));
	io.WriteString("Press key to quit... ");
	int Val;
	io.ReadChar(&Val);

	W1.RequestStop();
	W2.RequestStop();

	W1.WaitForStop();
	W2.WaitForStop();

	uc::end();
}

