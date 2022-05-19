#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

int main(int argc, char** argv[])
{
	srand(time(0));

	char map[10][20], player = 254, fruit = 3;
	int i, y, cordX = 4, cordY = 9, fruitX, fruitY, key = 0x0, score = 0;

	for (i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
	{
		for (y = 0; y < (sizeof(map[0]) / sizeof(char)); y++)
		{
			map[i][y] = ' ';
		}
	}

	fruitX = rand() % (sizeof(map) / sizeof(map[0]));
	fruitY = rand() % (sizeof(map[0]) / sizeof(char));
	map[fruitX][fruitY] = fruit;

	while (key != 0x71)
	{
		if (key == 0x50)
		{
			map[cordX][cordY] = ' ';

			if (cordX != 9) cordX++;
			else
			{
				printf("\nYou lost%c\n", 7);
				break;
			}
		}
		else if (key == 0x48)
		{
			map[cordX][cordY] = ' ';

			if (cordX != 0) cordX--;
			else
			{
				printf("\nYou lost%c\n", 7);
				break;
			}
		}
		else if (key == 0x4D)
		{
			map[cordX][cordY] = ' ';

			if (cordY != 19) cordY++;
			else
			{
				printf("\nYou lost%c\n", 7);
				break;
			}
		}
		else if (key == 0x4B)
		{
			map[cordX][cordY] = ' ';

			if (cordY != 0) cordY--;
			else
			{
				printf("\nYou lost%c\n", 7);
				break;
			}
		}

		if (cordX == fruitX && cordY == fruitY)
		{
			score++;

			while (1)
			{
				fruitX = rand() % (sizeof(map) / sizeof(map[0]));
				fruitY = rand() % (sizeof(map[0]) / sizeof(char));

				if (cordX != fruitX && cordY != fruitY)
					break;
			}

			map[fruitX][fruitY] = fruit;
		}

		// assign player to screen
		map[cordX][cordY] = player;

		// clear the console
		system("cls");

		// print out the map
		printf(" ____________________");
		for (i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
		{
			printf("\n|");
			for (y = 0; y < (sizeof(map[0]) / sizeof(char)); y++)
			{
				printf("%c", map[i][y]);
			}
			printf("|");
		}

		printf("\n --------------------");
		printf("\n\nScore: %d\n", score);

		// get keyboard input
		key = _getch();
	}
}