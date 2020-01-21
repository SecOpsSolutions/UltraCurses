#include <deque>
#include <unistd.h>
#include <random>

#include "uCurses.h"

#define WORLD_WIDTH 27
#define WORLD_HEIGHT 12

class xPoint
{
private:
	uc::Point _Pos;
public:
	int& x = _Pos.x;
	int& y = _Pos.y;
	xPoint(const xPoint& copy) {_Pos.x = copy.x; _Pos.y = copy.y;}
	xPoint(const uc::Point& point) {_Pos.x = point.x; _Pos.y = point.y;}
	xPoint(int x, int y) {_Pos.x = x; _Pos.y = y;}
	bool operator==(xPoint& bit) {return (bit.x == _Pos.x && bit.y == _Pos.y);}
	operator uc::Point() {return _Pos;}
};
typedef std::deque<xPoint> SnakeBody;

unsigned long RandomNumber(unsigned long Min, unsigned long Max)
{
	static std::mt19937 Rng;
	bool Loaded = false;

	if (!Loaded)
	{
		Rng.seed(std::random_device()());
		Loaded = true;
	}

	std::uniform_int_distribution<unsigned long> dist(Min, Max);
	return dist(Rng);
}

bool CheckSnakeHit(SnakeBody& body, xPoint& bit)
{
	for (SnakeBody::iterator it = body.begin(); it != body.end(); ++it)
	{
		if (*it == bit)
			return true;
	}

	return false;
}

int main()
{
	uc::Terminal* Terminal = &(uc::Terminal::GetStdTerminal());
	uc::BasicWindow* Main;
	uc::BasicWindow* Border;
	uc::BasicWindow* World;
	uc::BasicWindow* Status;
	uc::Point WorldPosition, BorderPosition, StatusPosition;
	long Timeout = 200000;

	BorderPosition.x = (Terminal->Width() - WORLD_WIDTH) / 2;
	BorderPosition.y = (Terminal->Height() - WORLD_HEIGHT) / 2;
	WorldPosition.x = BorderPosition.x + 1;
	WorldPosition.y = BorderPosition.y + 1;
	StatusPosition.x = BorderPosition.x;
	StatusPosition.y = BorderPosition.y - 4;

	Main = new uc::BasicWindow(Terminal, Terminal->Height(), Terminal->Width(), 0, 0);
	Main->Refresh();

	Status = new uc::BasicWindow(Terminal, 3, WORLD_WIDTH, StatusPosition.x, StatusPosition.y);
	//Status->SetBackground(uc::Color::Blue);
	Status->EnableColors(uc::Color::White, uc::Color::Blue);
	Status->DrawBorder();
	Status->Refresh();
	Status->CursorSet(1, 1);
	uc::Point StatusPos = Status->CursorPosition();

	Border = new uc::BasicWindow(Terminal, WORLD_HEIGHT, WORLD_WIDTH, BorderPosition.x, BorderPosition.y);
	Border->EnableColors(uc::Color::White, uc::Color::Blue);
	Border->DrawBorder();
	Border->Refresh();

	World = new uc::BasicWindow(Terminal, WORLD_HEIGHT - 2, WORLD_WIDTH - 2, WorldPosition.x, WorldPosition.y);
	World->SetBackground(uc::Color::Blue);
	World->EnableColors(uc::Color::White, uc::Color::Blue);
	World->Refresh();

	xPoint Fruit(RandomNumber(0, World->WindowWidth() - 1), RandomNumber(0, World->WindowHeight() - 1));

	int Input;
	Terminal->Echo(false);
	Terminal->CursorVisibility(false);

	SnakeBody snakebody;
	snakebody.push_front(xPoint(World->CursorPosition()));
	World->WriteChar('#');
	snakebody.push_front(xPoint(World->CursorPosition()));
	World->WriteChar('#');
	snakebody.push_front(xPoint(World->CursorPosition()));
	World->WriteChar('#');

	bool Hit = false;
	int CurrentKeyValue = uc::Key::Right;
	unsigned long Score = 0;

	while (Input != 'q' && !Hit)
	{
		World->CursorSet(Fruit);
		World->WriteChar('*');

		usleep(Timeout);
		int KeyValue, NewInput;
		while ((KeyValue = World->ReadChar(&NewInput, false)) != uc::Key::Error)
		{
				Input = NewInput;
				CurrentKeyValue = KeyValue;
		}

		xPoint np(snakebody.front());

		switch (CurrentKeyValue)
		{
		case uc::Key::Up:
			if (np.y > 0)
				np.y -= 1;
			else
				Hit = true;
			break;
		case uc::Key::Down:
			if (np.y < World->WindowHeight() - 1)
				np.y += 1;
			else
				Hit = true;
			break;
		case uc::Key::Right:
			if (np.x < World->WindowWidth() - 1)
				np.x += 1;
			else
				Hit = true;
			break;
		case uc::Key::Left:
			if (np.x > 0)
				np.x -= 1;
			else
				Hit = true;
			break;
		default:
			break;
		}

		if (CheckSnakeHit(snakebody, np))
				Hit = true;

		if (Hit)
		{
			World->SetBackground(uc::Color::Black);
			std::string Text = "!!! GAME OVER !!!";
			xPoint GoPos((World->WindowWidth() / 2) - (Text.length() / 2), World->WindowHeight() / 2);
			World->CursorSet(GoPos);
			World->EnableColors(uc::Color::White, uc::Color::Black);
			World->SetTextBold(true);
			World->SetTextUnderlined(true);
			World->WriteString(Text);
			World->ReadChar();
		}
		else
		{
			if (np == Fruit)
			{
				Score += 1000;
				Fruit.x = RandomNumber(0, World->WindowWidth() - 1);
				Fruit.y = RandomNumber(0, World->WindowHeight() - 1);
			}
			else
			{
				World->CursorSet(snakebody.back());
				World->WriteChar(' ');
				snakebody.pop_back();
			}

			World->CursorSet(np);
			World->WriteChar('#');
			snakebody.push_front(np);

			Status->CursorSet(StatusPos);
			Status->WriteString("                         ");
			Status->CursorSet(StatusPos);
			(*Status) << "Score: " << Score;
			Status->Refresh();
		}
	}

	World->DisableColors();
	Terminal->Echo(true);
	Terminal->CursorVisibility(true);

	delete Status;
	delete World;
	delete Border;
	delete Main;

	uc::Terminal::EndStdTerminal();
}