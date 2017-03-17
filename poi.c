//
// Created by Tnecesoc on 2017/3/4.
//

#include "Tnecesoc/Controller.c"

int main() {

    printf("- Initializing...");

    init();

    printf("done!");

    resetScreen();

    showEditingFile(cache->name);

    int key;

    while (key = getch(), key != 26) { // Ctrl + Z

        handleKey(key);

    }

    quit();

    return 0;
}