//
// Created by Tnecesoc on 2017/3/16.
//

#ifndef TSPORTMANAGER_AUTHENTICATION_H
#define TSPORTMANAGER_AUTHENTICATION_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "Utils/Algorithm.h"
#include "Utils/FileStream.h"
#include "Utils/Trie.h"

#define ROOT_KEY_STORAGE "./root_auth"
#define ADMINSTRATOR_KEY_STORAGE "./admin_auth"

#define ADMIN_KEY_LENGTH 6

void generateAdminKey(char adminOfWhat[][128], int count, char *view_path) {

    Trie *tmp = newTrie();

    srand((unsigned int) time(NULL));

    FILE *storage = fopen(ADMINSTRATOR_KEY_STORAGE, "w");

    FILE *view = fopen(view_path, "w");

    if (!view || !storage) {
        return;
    }

    char str[256];

    char sha256[2048];

    for (int i = 0; i < count; ++i) {

        do {

            for (int j = 0; j < ADMIN_KEY_LENGTH; ++j) {
                int what = random(0, 100000);
                if (what > 100000 / 3) {
                    if (what < 2 * 100000 / 3) {
                        str[j] = (char) random('a', 'z');
                    } else {
                        str[j] = (char) random('A', 'Z');
                    }
                } else {
                    str[j] = (char) random('0', '9');
                }
            }

            str[ADMIN_KEY_LENGTH] = '\0';

        } while (TrieQuery(tmp, str) != WORD_NOT_EXIST);

        TriePut(tmp, str, 1);

        fprintf(view, "%s, name: %s\n", str, adminOfWhat[i]);
        strcat(str, adminOfWhat[i]);
        fprintf(storage, StrSHA256(str, strlen(str), sha256));
    }

    TrieClear(tmp);
    free(tmp);
    fclose(view);
    fclose(storage);
}

bool isAdminAccessAccepted(char *operationName, char *adminKey) {

    char tmp[256];
    char sha256[2048];
    char buf[65536];

    strcpy(tmp, adminKey);
    strcat(tmp, operationName);
    StrSHA256(tmp, strlen(tmp), sha256);

    readFileToString(ADMINSTRATOR_KEY_STORAGE, buf);

    return isStringMatch(buf, sha256);
}

bool isRootKeySet() {
    return isFileNotEmpty(ROOT_KEY_STORAGE);
}

void setRootKey(char *key) {

    FILE *out = fopen(ROOT_KEY_STORAGE, "w");
    if (out == NULL) {
        return;
    }

    char buf1[2048];
    char buf2[65536];
    char sha256[4096];

    readFileToString(ADMINSTRATOR_KEY_STORAGE, buf1);

    sprintf(buf2, StrSHA256(buf1, strlen(buf1), sha256));
    sprintf(buf2 + random(0, strlen(buf2)), StrSHA256(key, strlen(key), sha256));

    fprintf(out, buf2);

    fclose(out);
}

bool isRootAccessAccepted(char *rootKey) {

    char buf[65536];
    char sha256[128];

    memset(sha256, 0, sizeof(sha256));

    StrSHA256(rootKey, strlen(rootKey), sha256);

    readFileToString(ROOT_KEY_STORAGE, buf);

    return isStringMatch(buf, sha256);
}

#endif //TSPORTMANAGER_AUTHENTICATION_H
