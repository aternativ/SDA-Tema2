#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>

typedef struct list *List;
typedef struct tree *Tree;

typedef struct system {
    List files;
    FILE *fi, *fo;
    Tree root;
} *System;

#endif