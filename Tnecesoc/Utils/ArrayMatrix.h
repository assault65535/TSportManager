//
// Created by Tnecesoc on 2017/3/15.
//

#ifndef TSPORTMANAGER_ARRAYMATRIX_H
#define TSPORTMANAGER_ARRAYMATRIX_H

#include <stdbool.h>
#include "ArrayList.h"

typedef struct ArrayMatrix ArrayMatrix;

struct ArrayMatrix {
    size_t e_size;
    ArrayList *rows;
};

// beg = malloc( row * cell * e_size )
// cell_end = beg + (cell - 1) * e_size
// row_end = beg + (row - 1) * cell * e_size

ArrayMatrix *newMatrix(size_t element_size) {
    ArrayMatrix *ret = malloc(sizeof(ArrayMatrix));

    ret->e_size = element_size;
    ret->rows = newArrayList(sizeof(ArrayList));

    return ret;
}

bool matrixEmpty(ArrayMatrix *_this) {
    return arrayListEmpty(_this->rows);
}

int matrixRows(ArrayMatrix *_this) {
    return arrayListSize(_this->rows);
}

int matrixCells(ArrayMatrix *_this) {

    if (matrixEmpty(_this)) {
        return 0;
    }

    return arrayListSize(_this->rows->begin);
}

void matrixAddRow(ArrayMatrix *_this, const void *as) {

    ArrayList *new_row = newArrayList(_this->e_size);

    int cells = matrixCells(_this);
    for (int i = 0; i < cells; ++i) {
        arrayListAdd(new_row, as);
    }

    arrayListAdd(_this->rows, new_row);
    new_row->begin = NULL;
    new_row->end = NULL;
    free(new_row);
}

// add row before add cell.
void matrixAddCell(ArrayMatrix *_this, const void *as) {
    for (void *i = _this->rows->begin; i < _this->rows->end; i += sizeof(ArrayList)) {
        arrayListAdd(i, as);
    }
}

void *matrixGet(ArrayMatrix *_this, const int row, const int cell) {

    void *which = arrayListGet(arrayListGet(_this->rows, row), cell);

    return which;
}

void matrixClear(ArrayMatrix *_this) {
    for (void *i = _this->rows->begin; i < _this->rows->end; i += sizeof(ArrayList)) {
        arrayListClear(i);
    }
    arrayListClear(_this->rows);
}

// if row = 0, cell will be set to 0 automatically.
void matrixDeleteRow(ArrayMatrix *_this, const int row) {

    if (matrixRows(_this) <= 1) {
        matrixClear(_this);
        return;
    }

    ArrayList *target = arrayListGet(_this->rows, row);
    arrayListClear(target);

    arrayListDelete(_this->rows, row);
}

void matrixDeleteCell(ArrayMatrix *_this, int cell) {

    if (matrixCells(_this) <= 1) {
        for (void *i = _this->rows->begin; i < _this->rows->end; i += sizeof(ArrayList)) {
            arrayListClear(i);
        }
        return;
    }

    for (void *i = _this->rows->begin; i < _this->rows->end; i += sizeof(ArrayList)) {
        arrayListDelete(i, cell);
    }
}

void matrixInsertRow(ArrayMatrix *_this, const int row, const void *as) {

    if (row == matrixRows(_this)) {
        matrixAddRow(_this, as);
        return;
    }

    ArrayList *new_row = newArrayList(_this->e_size);

    int cells = matrixCells(_this);
    for (int i = 0; i < cells; ++i) {
        arrayListAdd(new_row, as);
    }

    arrayListInsert(_this->rows, new_row, row);

    new_row->begin = new_row->end = NULL;
    free(new_row);
}

void matrixInsertCell(ArrayMatrix *_this, const int cell, const void *as) {

    if (cell == matrixCells(_this)) {
        matrixAddCell(_this, as);
        return;
    }

    for (void *i = _this->rows->begin; i < _this->rows->end; i += sizeof(ArrayList)) {
        arrayListInsert(i, as, cell);
    }
}

ArrayList *matrixGetRow(ArrayMatrix *_this, const int row) {

    return arrayListGet(_this->rows, row);

}


#endif //TSPORTMANAGER_ARRAYMATRIX_H
