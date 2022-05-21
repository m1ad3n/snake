#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

char map[10][20], player = 254, fruit = 3, body_img = 178;
int i, y, cordX = 4, cordY = 9, fruitX, fruitY, score = 0, key = 0x0, smer = 0x0;

DWORD key_input()
{
    while (key != 0x71)
    {
        if (key == 0x50 && smer != 0x2)
            smer = 0x1;
        else if (key == 0x48 && smer != 0x1)
            smer = 0x2;
        else if (key == 0x4D && smer != 0x4)
            smer = 0x3;
        else if (key == 0x4B && smer != 0x3)
            smer = 0x4;

        key = _getch();
    }
    return 0;
}

DWORD screen_ptr()
{
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
        map[cordX][cordY] = ' ';

        switch (smer)
        {
        case 0x1:
            if (cordX != 9) cordX++;
            else { printf("\nYou lost%c\n", 7); exit(0); }

            break;
        case 0x2:
            if (cordX != 0) cordX--;
            else { printf("\nYou lost%c\n", 7); exit(0); }

            break;
        case 0x3:
            if (cordY != 19) cordY++;
            else { printf("\nYou lost%c\n", 7); exit(0); }

            break;
        case 0x4:
            if (cordY != 0) cordY--;
            else { printf("\nYou lost%c\n", 7); exit(0); }

            break;
        }

        if (cordX == fruitX && cordY == fruitY)
        {
            while (cordX == fruitX && cordY == fruitY)
            {
                fruitX = rand() % (sizeof(map) / sizeof(map[0]));
                fruitY = rand() % (sizeof(map[0]) / sizeof(char));
            }

            map[fruitX][fruitY] = fruit;
            score++;
        }

        map[cordX][cordY] = player;

        // clear the console
        system("cls");

        printf("%c", 201);
        for (i = 0; i < 20; i++)
            printf("%c", 205);
        printf("%c", 187);

        for (i = 0; i < (sizeof(map) / sizeof(map[0])); i++)
        {
            printf("\n%c", 186);
            for (y = 0; y < (sizeof(map[0]) / sizeof(char)); y++)
            {
                printf("%c", map[i][y]);
            }
            printf("%c", 186);
        }

        printf("\n%c", 200);
        for (i = 0; i < 20; i++)
            printf("%c", 205);

        printf("%c", 188);
        printf("\n\nScore: %d\n", score);

        Sleep(500);
    }

    return 0;
}

int main()
{
    srand(time(0));

    HANDLE thread1, thread2;

    // make threads
    thread1 = CreateThread(NULL, 0, key_input, NULL, 0, NULL);
    thread2 = CreateThread(NULL, 0, screen_ptr, NULL, 0, NULL);

    // wait for them to finish
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);
}
