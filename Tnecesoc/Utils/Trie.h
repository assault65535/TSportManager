//
// Created by Tnecesoc on 2017/3/6.
//

#ifndef SPORTMANAGER_SPLAY_TRIE_H
#define SPORTMANAGER_SPLAY_TRIE_H

#include <malloc.h>

#define MAX_TRIE_CHILD 256

#define WORD_NOT_EXIST -1

typedef struct Trie Trie;

int getIndex(char ch) {
//    if (ch >= 'a' && ch <= 'z') {
//        return ch - 'a';
//    } else if (ch >= 'A' && ch <= 'Z') {
//        return ch - 'A' + 26;
//    } else if (ch >= '0' && ch <= '9') {
//        return ch - '0' + 52;
//    } else {
//        return 63;
//    }
    return ch + 128;
}

struct Trie {
    int msg;
    Trie *child[MAX_TRIE_CHILD];
};

Trie *newTrie();
void TriePut(Trie *_this, char *key, int value);
void TrieDelete(Trie *_this, char *key);
int TrieQuery(Trie *_this, char *key);
void TrieClear(Trie *_this);

Trie *newTrie() {
    Trie *res = malloc(sizeof(Trie));
    res->msg = WORD_NOT_EXIST;
    for (int i = 0; i < MAX_TRIE_CHILD; ++i) {
        res->child[i] = NULL;
    }
    return res;
}

void TriePut(Trie *_this, char *key, int value) {

    if (value == WORD_NOT_EXIST) {
        TrieDelete(_this, key);
    }

    while (*key) {
        if (_this->child[getIndex(*key)] == NULL) {
            _this->child[getIndex(*key)] = newTrie();
        }

        _this = _this->child[getIndex(*(key++))];
    }

    _this->msg = value;
}

void TrieDelete(Trie *_this, char *key) {
    if (*key && _this->child[getIndex(*key)] != NULL) {
        TrieDelete(_this->child[getIndex(*key)], key + 1);
        free(_this->child[getIndex(*key)]);
        _this->child[getIndex(*key)] = NULL;
    }
}

int TrieQuery(Trie *_this, char *key) {
    while (*key) {
        if (_this->child[getIndex(*key)] == NULL) {
            return WORD_NOT_EXIST;
        }

        _this = _this->child[getIndex(*(key++))];
    }

    return _this->msg;
}

void TrieClear(Trie *_this) {
    for (int i = 0; i < MAX_TRIE_CHILD; ++i) {
        if (_this->child[i] != NULL) {
            TrieClear(_this->child[i]);
            free(_this->child[i]);
            _this->child[i] = NULL;
        }
    }
}

#endif //SPORTMANAGER_SPLAY_TRIE_H
