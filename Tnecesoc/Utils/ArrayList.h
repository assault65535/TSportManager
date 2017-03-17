//
// Created by Tnecesoc on 2017/3/5.
//

#include <malloc.h>
#include <mem.h>
#include "Generics.h"

#ifndef SPORTMANAGER_SPLAY_ARRAYLIST_H
#define SPORTMANAGER_SPLAY_ARRAYLIST_H

typedef struct ArrayList ArrayList;

struct ArrayList {
    size_t e_size;
    void *begin, *end;
};

ArrayList *newArrayList(size_t element_size) {

    ArrayList *res = malloc(sizeof(ArrayList));
    res->e_size = element_size;
    res->begin = malloc(element_size);
    res->end = res->begin;

    return res;
}

bool arrayListEmpty(ArrayList *_this) {
    return _this->begin == _this->end;
}

size_t arrayListSize(ArrayList *_this) {
    return (_this->end - _this->begin) / _this->e_size;
}

void *arrayListGet(ArrayList *_this, const int index) {
    return _this->begin + index * _this->e_size;
}

void arrayListAdd(ArrayList *_this, const void *data) {

    size_t size = arrayListSize(_this);

    void *new_list = malloc((size + 2) * _this->e_size);

    memcpy(new_list, _this->begin, (size + 1) * _this->e_size);

    memcpy(new_list + (size * _this->e_size), data, _this->e_size);

    free(_this->begin);

    _this->begin = new_list;

    _this->end = _this->begin + ((size + 1) * _this->e_size);
}

void arrayListClear(ArrayList *_this) {
    free(_this->begin);
    _this->begin = malloc(_this->e_size);
    _this->end = _this->begin;
}

void arrayListDelete(ArrayList *_this, const int index) {

    int size = arrayListSize(_this);

    void *new_list = malloc(size * _this->e_size);

    memcpy(new_list, _this->begin, index * _this->e_size);
    memcpy(new_list + index * _this->e_size, _this->begin + (index + 1) * _this->e_size, (size - index) * _this->e_size);

    free(_this->begin);
    _this->begin = new_list;
    _this->end = new_list + (size - 1) * _this->e_size;
}

void arrayListInsert(ArrayList *_this, const void *data, const int index) {

    int size = arrayListSize(_this);

    void *new_list = malloc((size + 2) * _this->e_size);

    memcpy(new_list, _this->begin, index * _this->e_size);
    memcpy(new_list + index * _this->e_size, data, _this->e_size);
    memcpy(new_list + (index + 1) * _this->e_size, _this->begin + index * _this->e_size, (size - index + 1) * _this->e_size);

    free(_this->begin);
    _this->begin = new_list;
    _this->end = new_list + (size + 1) * _this->e_size;
}

void arrayListErase(ArrayList *_this, void *from, void *to) {
    if (from < _this->begin || to >= _this->end) {
        return;
    }

    void *new_list = malloc(((_this->end - _this->begin) - (to - from + 1)) * _this->e_size);

    memcpy(new_list, _this->begin, (from - _this->begin) * _this->e_size);
    memcpy(new_list, to + 1, (_this->end - to) * _this->e_size);
    free(_this->begin);
    _this->begin = new_list;
    _this->end = new_list + (((_this->end - _this->begin) - (to - from + 1)));
}

void arrayListForEach(ArrayList *_this, Consumer f) {
    for (void *i = _this->begin; i < _this->end; i += _this->e_size) {
        f(i);
    }
}

bool arrayListFilter(ArrayList *_this, Predictor condition, Consumer function) {
    bool flag = false;
    for (void *i = _this->begin; i < _this->end; i += _this->e_size) {
        if (condition != NULL && condition(i)) {
            flag = true;
            if (function != NULL) {
                function(i);
            }
        }
    }
    return flag;
}

#endif //SPORTMANAGER_SPLAY_ARRAYLIST_H
