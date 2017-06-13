#pragma once

#include "BasicWindow.h"
#include "Defines.h"
#include "Terminal.h"

namespace uc
{

static Terminal& (*std)() = Terminal::GetStdTerminal;
static void (*end)() = Terminal::EndStdTerminal;

}
