#pragma once

#include <string>

namespace uc
{

static const std::string endl = "\n";

enum Key
{
	Error = -1,
	None = 0,
	Up,
	Down,
	Left,
	Right,
	Tab,
	Backspace
};

};
