#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DELAY 100000
#define APPLECOUNT 10

void preventAppleOverlap(int* appleX, int* appleY, int xBound, int yBound);

struct applePositions {
    int xCoordinates[APPLECOUNT];
    int yCoordinates[APPLECOUNT];
};

int main(void) {
    int ch;
    int x = 1, y = 1;
    int maxY = 0, maxX = 0;
    char controls[4] = {'d', 'a', 's', 'w'};
    char controlsInverted[4] = {'a', 'd', 'w', 's'};
    int heading = 0;

    struct applePositions apples;

    // TODO add a dynamiclly allocated array for the tail positions
    int snakeTailSize = 1;
    int snakeTailXPositions[1000];
    int snakeTailYPositions[1000];

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr, maxY, maxX);

    // Generates the initial positions of the apples
    for (int i = 0; i < APPLECOUNT; i++) {
        apples.xCoordinates[i] = rand() % maxX;
        apples.yCoordinates[i] = rand() % maxY;
    }

    preventAppleOverlap(apples.xCoordinates, apples.yCoordinates, maxX, maxY);

    while (1) {
        ch = getch();

        clear();
        char* score = malloc(sizeof(char) * 100);
        sprintf(score, "%i", snakeTailSize - 1);
        // Renders the current score
        mvprintw(0, 0, score);
        // Renders thee head of the snake at the current position
        mvprintw(y, x, "#");
        // Renders the tail of the snale at the previous positions that are stored in the array
        for (int i = 1; i < snakeTailSize; ++i) {
            mvprintw(snakeTailYPositions[i], snakeTailXPositions[i], "#");
        }

        // Rendering the apples
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
        // Input handling
        int previousHeading = heading;

        for (int i = 0; i < 4; ++i) {
            if (controls[i] == ch) {
                heading = i;
            }
        }

        // Disables the ability to do instant 180 degree turns 
        if (controls[heading] == controlsInverted[previousHeading]) {
            heading = previousHeading;
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

        // Tail logic
        int* snakeTailXPosTemp = malloc(sizeof(snakeTailXPositions));
        int* snakeTailYPosTemp = malloc(sizeof(snakeTailYPositions));
		for (int i = 0; i < snakeTailSize; ++i) {
			snakeTailXPosTemp[i] = snakeTailXPositions[i];
			snakeTailYPosTemp[i] = snakeTailYPositions[i];
		}
        snakeTailXPositions[0] = x;
        snakeTailYPositions[0] = y;

        for (int i = 0; i < snakeTailSize - 1; ++i) {
            snakeTailXPositions[i + 1] = snakeTailXPosTemp[i];
            snakeTailYPositions[i + 1] = snakeTailYPosTemp[i];
        }

        // Respawning already eaten apples
        for (int i = 0; i < APPLECOUNT; ++i) {
            if (apples.xCoordinates[i] == -1) {
                apples.xCoordinates[i] = rand() % maxX;
                apples.yCoordinates[i] = rand() % maxY;
            }
        }

        preventAppleOverlap(apples.xCoordinates, apples.yCoordinates, maxX, maxY);

        // End if the snake collides with its own tail
        for (int i = 0; i < snakeTailSize; ++i) {
            for (int j = 0; j < snakeTailSize; ++j) {
                if (snakeTailXPositions[i] == snakeTailXPositions[j] && snakeTailYPositions[i] == snakeTailYPositions[j] && i != j) {
                    endwin();
                    return 0;   
                }
            }
        }
        // End the game if snake is offscreen
        if (x >= maxX || x < 0 || y >= maxY || y < 0) {
            endwin();
            return 0;
        }
        free(score);
		free(snakeTailXPosTemp);
		free(snakeTailYPosTemp);
        
    }
    endwin();
    return 0;
}


void preventAppleOverlap(int* appleX, int* appleY, int xBound, int yBound) {
    int cycleCount = 1;
    int noOverlap = 0;
    while (!noOverlap) {
        cycleCount++;
        noOverlap = 1;
        for (int i = 0; i < APPLECOUNT; ++i) {
            for (int j = 0; j < APPLECOUNT; ++j) {
                if (appleX[i] == appleX[j] &&  appleY[i] == appleY[j] && i != j) {
                    noOverlap = 0;
                    appleX[i] = rand() % xBound;
                    appleY[i] = rand() % yBound;
                }
            }
        }
        if (cycleCount == 2) {
            break;
        }
    }
}

