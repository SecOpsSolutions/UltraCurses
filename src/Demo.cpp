#include "uCurses.h"
#include <unistd.h>
#include <sstream>

int main()
{
	uc::Terminal T1("/dev/pts/1");
	uc::Terminal T2("/dev/pts/2");

	T1.SetActive();
	T1.WriteString("Hello Terminal One\n", true);
//	T1.ReadChar();

	T2.SetActive();
	T2.WriteString("Hello Terminal Two\n", true);

	uc::std().SetActive();
	uc::std().WriteString("Press key to quit...", true);
	uc::std().ReadChar();
	uc::end();
}

