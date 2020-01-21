#pragma once

#include <string>
#include <mutex>

#include "Defines.h"

namespace uc
{
	class Terminal
	{
	private:
		static Terminal* _StdTerminal;
		static int _Instances;

		static std::mutex _Lock;
		static Terminal* _Locker;

		SCREEN* _TerminalHandle;

		void InitializeCurses();
		Terminal();

	public:
		/**
		 * @brief Get the singleton StdTerminal instance.
		 * 
		 * You are not allowed to create new terminal instances.
		 * Instead you can retrieve the singleton UltraCurses Terminal
		 * instance for the calling terminal, called Stdterminal.
		 * Upon first calling this UltraCurses will initialize the 
		 * terminal resulting in other applications not being able to 
		 * interact with it anymore. All the fancy functions UltraCurses
		 * offers will now work.
		 * 
		 * @return Terminal& UltraCurses Standard Terminal instance
		 */
		static Terminal& GetStdTerminal();

		/**
		 * @brief ending the standard terminal
		 * 
		 * should be called at the end of the application before 
		 * quitting. Will return the terminal handle to the parent 
		 * application and deinit any funky stuff the initialization
		 * did to it.
		 */
		static void EndStdTerminal();

		/**
		 * @brief obligatory destructor
		 */
		~Terminal();

		/**
		 * @brief read from the terminal but only wait so long for
		 * the user to enter stuff and then resume.
		 * 
		 * @param us amount of microseconds to wait for interaction
		 * @return true the user entered stuff
		 * @return false the user did not enter stuff
		 */
		bool WaitForInput(long us);

		/**
		 * @brief in case of a multithreaded application this will 
		 * lock the Terminal for stuff like rendering which is 
		 * better done undisturbed. also sets focus to this terminal
		 * 
		 * @return true could be locked and also is now locked. Or 
		 * was already locked and is still focused
		 * @return false couldn't be locked
		 */
		bool FocusAndLock();

		/**
		 * @brief unlock a previous locked Terminal
		 */
		void Unlock();

		/**
		 * @brief actually renders all the changes since the last 
		 * rendering to the terminal
		 */
		void Refresh();

		/**
		 * @brief retrieve the current height of this terminal
		 * 
		 * @return int height of this terminal
		 */
		int Height();

		/**
		 * @brief retrieve the current width of this terminal 
		 * 
		 * @return int width of this terminal
		 */
		int Width();

		/**
		 * @brief Saves the currently rendered output of this 
		 * terminal into a dump-file to be restored later on.
		 * This comes handy if you render temporary windows over
		 * current windows and want to return to the current 
		 * status of output later on.
		 * 
		 * @param SaveName name of the dumpfile which should be 
		 * used for saving the terminal output
		 */
		void SaveScreen(std::string SaveName = "");

		/**
		 * @brief Restores a previously saved terminal output
		 * onto this terminal
		 * 
		 * @param SaveName name of the dumpfile which from which
		 * the terminal output should be loaded
		 */
		void RestoreScreen(std::string SaveName = "");

		/**
		 * @brief Clear the screen completely
		 */
		void ClearScreen();

		/**
		 * @brief enable/disable echoing of user inputs to this
		 * terminal
		 * 
		 * @param On true means enabling, false means disabling. 
		 * defaults to true
		 */
		void Echo(bool On = true);

		/**
		 * @brief enable/disable cursor visibility on this terminal
		 * 
		 * @param On true means enabling, false means disabling. 
		 * defaults to true
		 */
		void CursorVisibility(bool On = true);
	};

}
