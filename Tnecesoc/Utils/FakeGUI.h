//
// Created by Tnecesoc on 2017/3/10.
//

#ifndef SPORTMANAGER_SPLAY_GUI_H
#define SPORTMANAGER_SPLAY_GUI_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mem.h>
#include <stdbool.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 25

typedef char Raster[SCREEN_WIDTH];

Raster screenBuf[SCREEN_HEIGHT];

void setCursorTo(int row, int cell) {
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT) cell, (SHORT) row};
    SetConsoleCursorPosition(hOut, pos);
}

void reloadBuf() {
    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            screenBuf[i][j] = '\0';
        }
    }
}

void refreshScreen() {
    system("cls");
    for (int i = 0; i < SCREEN_HEIGHT - 2; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            putchar(screenBuf[i][j]);
        }
    }
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        putchar('_');
    }
    printf("运动会管理系统, 封测版本.");
}

void uploadCharBlock(Raster charBlock[], int height, int placeR, int placeC) {
    for (int i = 0; i < height; ++i) {
        sprintf(screenBuf[i + placeR] + placeC, charBlock[i]);
    }
}

#define DEFAULT_CURSOR_ROW 2
#define DEFAULT_CURSOR_MARGIN 3
#define DEFAULT_CURSOR_CELL_LEFT 3
#define DEFAULT_CURSOR_CELL_RIGHT (SCREEN_WIDTH / 2 + DEFAULT_CURSOR_CELL_LEFT)

#define BOTTOM_OF_ROW (SCREEN_HEIGHT - 3)

int uploadString(char *str, int _row) {

    int cell = DEFAULT_CURSOR_CELL_LEFT;

    int curRow = 0, curCell = 0, row = _row;

    for (int i = 0; *str; ++i, ++str) {

        curRow = row + (i / (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_CELL_LEFT));
        curCell = cell + (i % (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_CELL_LEFT));

        if (*str == '\n') {

            if (curRow + 1 >= BOTTOM_OF_ROW) {

                sprintf(screenBuf[curRow + 1] + cell, "...");

                break;
            }

            row++;

            i = -1;
            continue;
        }

        screenBuf[curRow][curCell] = *str;
    }

    row = DEFAULT_CURSOR_ROW;
    cell = DEFAULT_CURSOR_CELL_RIGHT;

    if (*str) {

        str++;

        for (int i = 0; *str; ++i, ++str) {

            curRow = row + (i / (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_CELL_LEFT));
            curCell = cell + (i % (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_CELL_LEFT));

            if (*str == '\n') {

                if (curRow + 1 >= BOTTOM_OF_ROW) {

                    sprintf(screenBuf[curRow + 1] + cell, "...更多信息请按 Ctrl + K");

                    break;
                }

                row++;

                i = -1;
                continue;
            }

            screenBuf[curRow][curCell] = *str;
        }

    }

    return curRow;
}

int uploadStringAtCenter(char *str, int row) {

    int cell;

    char buf[128];

    strcpy(buf, str);

    str = strtok(buf, "\n");

    while (str != NULL) {

        if (row >= BOTTOM_OF_ROW) {
            continue;
        }

        cell = (SCREEN_WIDTH - strlen(str)) >> 1;
        sprintf(screenBuf[row] + cell, str);

        str = strtok(NULL, "\n");
        row++;
    }

    return row;
}

int putString(char *str, int _row) {

    int curRow = uploadString(str, _row);

    refreshScreen();

    return curRow;
}

void putStringAtCenter(char str[], int row) {

    uploadStringAtCenter(str, row);

    refreshScreen();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
bool showTextBox(char *out, char msg[], int row) {

    int cursorR = putString(msg, row) + 1, key = 0, i = 0;

    char buf[128];

    do {

        setCursorTo(
                cursorR + (i / (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_MARGIN)),
                DEFAULT_CURSOR_CELL_LEFT + (i % (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_MARGIN))
        );

        key = getch();

        if (key == 13 || key == 27) { // Enter or Esc
            break;
        } else {
            if (key == 8) { // backspace
                if (i > 0) {
                    buf[--i] = ' ';
                }
            } else if (i < 128) {
                buf[i++] = key;
                buf[i] = '\0';
            }

            putString(buf, cursorR);
        }

    } while (true);

    sscanf(buf, "%s", out);

    return key == 13;
}
#pragma clang diagnostic pop

void showMessageBox(char *msg) {

    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        screenBuf[SCREEN_HEIGHT / 4][i] = '=';
        screenBuf[3 * SCREEN_HEIGHT / 4][i] = '=';
        for (int j = SCREEN_HEIGHT / 4 + 1; j < 3 * SCREEN_HEIGHT / 4; ++j) {
            screenBuf[j][i] = '\0';
        }
    }

    uploadStringAtCenter(msg, 2 * SCREEN_HEIGHT / 5);

    uploadStringAtCenter("按任意键以继续", 3 * SCREEN_HEIGHT / 5);

    refreshScreen();

    getch();
}

bool showAlertDialog(char *msg) {

    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        screenBuf[SCREEN_HEIGHT / 4][i] = '=';
        screenBuf[3 * SCREEN_HEIGHT / 4][i] = '=';
        for (int j = SCREEN_HEIGHT / 4 + 1; j < 3 * SCREEN_HEIGHT / 4; ++j) {
            screenBuf[j][i] = '\0';
        }
    }

    uploadStringAtCenter(msg, 2 * SCREEN_HEIGHT / 5);

    screenBuf[3 * SCREEN_HEIGHT / 5][SCREEN_WIDTH / 4] = 'Y';
    screenBuf[3 * SCREEN_HEIGHT / 5][3 * SCREEN_WIDTH / 4] = 'N';

    refreshScreen();

    int ch;

    do {
        ch = getch();
    } while (ch != 'Y' && ch != 'N');

    return ch == 'Y';
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
bool showPasswordTextBox(char *out, char *msg) {

    int cursorR = putString(msg, DEFAULT_CURSOR_ROW) + 1, key = 0, i = 0;

    char buf[128], view[128];

    do {

        setCursorTo(
                cursorR + (i / (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_MARGIN)),
                DEFAULT_CURSOR_CELL_LEFT + (i % (((SCREEN_WIDTH - DEFAULT_CURSOR_MARGIN) >> 1) - DEFAULT_CURSOR_MARGIN))
        );

        key = getch();

        if (key == 13 || key == 27) { // Enter or Esc
            break;
        } else {
            if (key == 8) { // backspace
                if (i > 0) {
                    buf[--i] = ' ';
                    view[i] = ' ';
                }
            } else if (i < 128) {
                buf[i] = key;
                buf[i + 1] = '\0';
                view[i] = '*';
                view[i + 1] = '\0';
                i++;
            }

            putString(view, cursorR);
        }

    } while (true);

    sscanf(buf, "%s", out);

    return key == 13;
}
#pragma clang diagnostic pop

#endif //SPORTMANAGER_SPLAY_GUI_H
