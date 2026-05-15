#ifndef HEAP_H
#define HEAP_H

typedef struct file *File;

typedef struct heap {
    File *files;
    int size;
    int capacity;
} *Heap;

Heap initHeap();
void insertHeap(Heap h, File file);
int isEmptyHeap(Heap h);
void insertHeap(Heap h, File file);
File extractMax(Heap h);
void freeHeap(Heap h);

#endif