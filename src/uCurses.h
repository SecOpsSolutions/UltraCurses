#pragma once

#include "BasicWindow.h"
#include "Defines.h"
#include "Terminal.h"

namespace uc
{
	/**
	 * @brief helper function pointer for easy retrieval
	 * of std::cout based UltraCurses Window
	 * @return BasicWindow& singleton instance of StdWindow
	 * (which is the calling terminal)
	 * 
	 * instead of calling uc::BasicWindow::GetStdWindow()
	 * this pointer allows for calling uc::out() instead
	 */
	static BasicWindow& (*out)() = BasicWindow::GetStdWindow;

	/**
	 * @brief helper function pointer for easy retrieval
	 * of std::cout based UltraCurses Terminal
	 * @return Terminal& singleton instance of StdTerminal
	 * (which is the calling terminal)
	 * 
	 * instead of calling uc::Terminal::GetStdTerminal()
	 * this pointer allows for calling uc::std() instead
	 */
	static Terminal& (*std)() = Terminal::GetStdTerminal;

	/**
	 * @brief helper function pointer for easier ending
	 * of this UltraCurses session
	 * 
	 * instead of calling uc::Terminal::EndStdTerminal()
	 * this pointer allows for calling uc::end() istead
	 */
	static void (*end)() = Terminal::EndStdTerminal;
}
