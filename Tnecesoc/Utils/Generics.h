//
// Created by Tnecesoc on 2017/3/5.
//

#ifndef SPORTMANAGER_SPLAY_GENERICS_H
#define SPORTMANAGER_SPLAY_GENERICS_H

typedef char T_STRING[128];

typedef void (*Consumer)(void* o);

typedef bool (*Predictor)(const void* o);

#endif //SPORTMANAGER_SPLAY_GENERICS_H
