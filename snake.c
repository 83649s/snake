#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DELAY 75000

int main(void) {
    int ch;
    int x = 1, y = 1;
    int maxY = 0, maxX = 0;
    char controls[4] = {'d', 'a', 's', 'w'};
    int heading = 0;

    int appleCount = 10;
    int appleXPos[appleCount];
    int appleYPos[appleCount];

    int snakeTailSize = 1;
    int snakeTailXPos[1000];
    int snakeTailYPos[1000];

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr, maxY, maxX);

    for (int i = 0; i < appleCount; i++) {
        appleXPos[i] = rand() % maxX;
        appleYPos[i] = rand() % maxY;
    }

    while (TRUE) {
        ch = getch();

        clear();
        char* score = malloc(sizeof(char) * 100);
        sprintf(score, "%i", snakeTailSize - 1);
        mvprintw(0, 0, score);
        mvprintw(y, x, "#");
        for (int i = 1; i < snakeTailSize; ++i) {
            mvprintw(snakeTailYPos[i], snakeTailXPos[i], "#");
        }

        for (int i = 0; i < appleCount; ++i) {
            if (x == appleXPos[i] & y == appleYPos[i]) {
                appleXPos[i] = -1;
                appleYPos[i] = -1;
                snakeTailSize++;
            }
            mvprintw(appleYPos[i], appleXPos[i], "@");
        }
        refresh();
        usleep(DELAY);
        for (int i = 0; i < 4; ++i) {
            if (controls[i] == ch) {
                heading = i;
            }
        }

        switch (heading) {
            case 0:
                x += 1;
                break;
            case 1:
                x -= 1;
                break;
            case 2:
                y += 1;
                break;
            case 3:
                y -= 1;
                break;
            default:
                break;
        }

        int snakeTailXPosTemp[sizeof(snakeTailXPos)];
        int snakeTailYPosTemp[sizeof(snakeTailYPos)];
        memcpy(snakeTailXPosTemp, snakeTailXPos, sizeof(snakeTailXPos));
        memcpy(snakeTailYPosTemp, snakeTailYPos, sizeof(snakeTailYPos));
        snakeTailXPos[0] = x;
        snakeTailYPos[0] = y;

        for (int i = 0; i < snakeTailSize - 1; ++i) {
            snakeTailXPos[i + 1] = snakeTailXPosTemp[i];
            snakeTailYPos[i + 1] = snakeTailYPosTemp[i];
        }

        for (int i = 0; i < appleCount; i++) {
            if (appleXPos[i] == -1) {
                appleXPos[i] = rand() % maxX;
                appleYPos[i] = rand() % maxY;
            }
        }

        if (x >= maxX || x < 0 || y >= maxY || y < 0) {
            break;
        }
        
    }
    endwin();
    return 0;
}