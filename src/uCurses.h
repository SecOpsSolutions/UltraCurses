#pragma once

#include "Defines.h"
#include "IO.h"
#include "Terminal.h"

namespace uc
{

static Terminal& (*std)() = Terminal::GetStdTerminal;
static void (*end)() = Terminal::EndStdTerminal;

}
