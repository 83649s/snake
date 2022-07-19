#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DELAY 75000
#define APPLECOUNT 10

struct applePositions {
    int xCoordinates[APPLECOUNT];
    int yCoordinates[APPLECOUNT];
};


void preventAppleOverlap(int* appleX, int* appleY, int xBound, int yBound) {
    int cycleCount = 1;
    int noOverlap = 0;
    while (!noOverlap) {
        noOverlap = 1;
        for (int i = 0; i < APPLECOUNT; ++i) {
            for (int j = 0; j < APPLECOUNT; ++j) {
                if (appleX[i] == appleY[j] && i != j) {
                    noOverlap = 0;
                    appleX[i] = rand() % xBound;
                    appleY[i] = rand() % yBound;
                }
            }
        }
    }
}


int main(void) {
    int ch;
    int x = 1, y = 1;
    int maxY = 0, maxX = 0;
    char controls[4] = {'d', 'a', 's', 'w'};
    int heading = 0;

    struct applePositions apples;

    int snakeTailSize = 1;
    int snakeTailXPos[1000];
    int snakeTailYPos[1000];

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr, maxY, maxX);

    for (int i = 0; i < APPLECOUNT; i++) {
        apples.xCoordinates[i] = rand() % maxX;
        apples.yCoordinates[i] = rand() % maxY;
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

        for (int i = 0; i < APPLECOUNT; ++i) {
            if (x == apples.xCoordinates[i] & y == apples.yCoordinates[i]) {
                apples.xCoordinates[i] = -1;
                apples.yCoordinates[i] = -1;
                snakeTailSize++;
            }
            mvprintw(apples.yCoordinates[i], apples.xCoordinates[i], "@");
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

        for (int i = 0; i < APPLECOUNT; ++i) {
            if (apples.xCoordinates[i] == -1) {
                apples.xCoordinates[i] = rand() % maxX;
                apples.yCoordinates[i] = rand() % maxY;
            }
        }

        preventAppleOverlap(apples.xCoordinates, apples.yCoordinates, maxX, maxY);

        if (x >= maxX || x < 0 || y >= maxY || y < 0) {
            break;
        }
        free(score);
        
    }
    endwin();
    return 0;
}