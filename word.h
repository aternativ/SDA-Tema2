#ifndef WORD_H
#define WORD_H

typedef struct system *System;

typedef struct word {
    char *s;
    int size;
    int max_size;
} *Word;

char *readWord(System system);
char *duplicate(char *s);

#endif