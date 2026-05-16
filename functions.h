#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct system *System;

void add(System system);
void delete(System system);
void addKW(System system);
void delKW(System system);
void find(System system);
void topK(System system);
void print(System system);
void prefix(System system);

#endif