#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "conio.h"
#include "Quaternion.h"

const unsigned n = 10;

const unsigned MSEC = CLOCKS_PER_SEC / 1000;

const unsigned delay_msec = 1250;

bool gameOver = false;

using Vec4 = Quaternion<int>;

const Vec4 u(1, 0, 0, 0);
const Vec4 i(0, 1, 0, 0);
const Vec4 j(0, 0, 1, 0);
const Vec4 k(0, 0, 0, 1);

Vec4 current = u;

Vec4 randTile()
{
	srand(time(NULL));
	unsigned x1, x2, x3, x4;
	x1 = 1 + rand() % n;
	x2 = 1 + rand() % n;
	x3 = 1 + rand() % n;
	x4 = 1 + rand() % n;
	return Vec4(x1, x2, x3, x4);
}

Vec4 apple(randTile());

std::vector<Vec4 > snake(2);

void update()
{
	snake.insert(snake.begin(), snake[0] + current);
	if (abs(snake[0].re - (n + 1) / 2.0) > (n - 1) / 2.0 || abs(snake[0].i - (n + 1) / 2.0) > (n - 1) / 2.0 || abs(snake[0].j - (n + 1) / 2.0) > (n - 1) / 2.0 || abs(snake[0].k - (n + 1) / 2.0) > (n - 1) / 2.0)
		gameOver = true;
	for (unsigned i = 1; i < snake.size(); ++i)
	{
		if (snake[i].re == snake[0].re && snake[i].i == snake[0].i && snake[i].j == snake[0].j && snake[i].k == snake[0].k)
			gameOver = true;
	}
	if (snake[0].re != apple.re || snake[0].i != apple.i || snake[0].j != apple.j || snake[0].k != apple.k)
		snake.pop_back();
	else
		apple = randTile();
}

int main()
{
	srand(time(NULL));
	unsigned x1, x2, x3, x4;
	x1 = 2 + rand() % (n - 2);
	x2 = 2 + rand() % (n - 2);
	x3 = 2 + rand() % (n - 2);
	x4 = 2 + rand() % (n - 2);
	snake[0] = Vec4(x1, x2, x3, x4);
	snake[1] = snake[0] - current;
	char key;
	unsigned current_time = clock();
	while (!gameOver)
	{
		for (auto tile : snake)
			std::cout << tile.re << " " << tile.i << " " << tile.j << " " << tile.k << std::endl;
		std::cout << apple.re << " " << apple.i << " " << apple.j << " " << apple.k << std::endl;
		std::cout << std::endl;
		while (clock() < current_time + delay_msec * MSEC)
		{
			if (_kbhit())
			{
				key = _getch();
				switch (key)
				{
				case 'w':
					current = u;
					break;
				case 's':
					current = -u;
					break;
				case 'a':
					current = -i;
					break;
				case 'd':
					current = i;
					break;
				case 'q':
					current = j;
					break;
				case 'e':
					current = -j;
					break;
				case 'z':
					current = k;
					break;
				case 'x':
					current = -k;
					break;
				}
			}
		}
		update();
		current_time = clock();
	}
	return 0;
}