//
// Created by Tnecesoc on 2017/3/6.
//

#ifndef SPORTMANAGER_SPLAY_FILESTREAM_H
#define SPORTMANAGER_SPLAY_FILESTREAM_H

#include <dirent.h>
#include <stdio.h>
#include <mem.h>
#include <stdbool.h>

typedef char *(*Corrector)(FILE *in);

typedef void (*Reader)(FILE *in);

bool isFileWithSuffix(char *fileName, char *suffix) {
    return strcmp(fileName + (strlen(fileName) - strlen(suffix)), suffix) == 0;
}

void openFileWithNotePad(char *fileName) {
    char buf[128];

    sprintf(buf, "notepad.exe %s", fileName);

    system(buf);
}

bool isFileNotEmpty(char *filename) {

    FILE *in;

    if ((in = fopen(filename, "r+")) == NULL) {
        return false;
    }

    bool ret = fgetc(in) != -1;

    fclose(in);

    return ret;
}

void readFileToString(char *filename, char *out) {

    FILE *in;

    if ((in = fopen(filename, "r+")) == NULL) {
        sprintf(out, "ERR_FILE_NOT_AVAILABLE");
        return;
    }

    char buf[128];

    while (fgets(buf, sizeof(buf), in) != NULL) {
        strcat(out, buf);
    }

    fclose(in);
}

void correctFile(char *in_path, char *out_path, Corrector corrector) {

    FILE *in, *out;

    if ((out = fopen(out_path, "w")) == NULL) {
        return;
    }

    if ((in = fopen(in_path, "r")) == NULL) {
        fprintf(out, "ERR_FILE_NOT_AVAILABLE");
        return;
    }

    while (!feof(in)) {
        fprintf(out, corrector(in));
    }

    fclose(in);
    fclose(out);
}

void readEachFileWithSuffixInPath(char *path, char *suffix, Reader reader) {
    DIR *dp;
    struct dirent *ep;

    char filePath[128];

    dp = opendir(path);
    if (dp != NULL) {
        ep = readdir(dp);
        while (ep) {

            if (isFileWithSuffix(ep->d_name, suffix)) {

                sprintf(filePath, "%s/%s", path, ep->d_name);

                FILE *in = fopen(filePath, "r+");
                reader(in);
                fclose(in);
            }

            ep = readdir(dp);
        }
        closedir(dp);
    }
}

void deleteFile(char *filename) {
    remove(filename);
}



#endif //SPORTMANAGER_SPLAY_FILESTREAM_H
