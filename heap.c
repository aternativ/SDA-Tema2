#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "helper.h"

#define INITIAL_SIZE 64

int compare(void *a, void *b) {
    File fa = a;
    File fb = b;

    if (fa->score != fb->score)
        return fa->score - fb->score;
    
    // if strcmp is > 0, that means we prefer "a", so we return > 0.
    // function is for a max heap. > 0 is preffered for the top.
    return strcmp(fb->name, fa->name);
}

Heap initHeap() {
    Heap h = malloc(sizeof(struct heap));
    h->size = 0;
    h->capacity = INITIAL_SIZE;
    h->files = malloc(INITIAL_SIZE * sizeof(File));
    return h;
}

int isEmptyHeap(Heap h) {
    return h->size == 0;
}

void siftDown(Heap h, int index) {
    int maxIndex = index;
    int l = index * 2 + 1;
    int r = index * 2 + 2;
    
    if (l < h->size && compare(h->files[l], h->files[maxIndex]) > 0)
        maxIndex = l;
    if (r < h->size && compare(h->files[r], h->files[maxIndex]) > 0)
        maxIndex = r;

    if (index != maxIndex) {
        File aux = h->files[index];
        h->files[index] = h->files[maxIndex];
        h->files[maxIndex] = aux;
        siftDown(h, maxIndex);
    }

}

void siftUp(Heap h, int index) {
    while (index > 0 && compare(h->files[(index - 1) / 2], h->files[index]) < 0) {
        File aux = h->files[index];
        h->files[index] = h->files[(index - 1) / 2];
        h->files[(index - 1) / 2] = aux; 
        index = (index - 1) / 2;
    }
}

void insertHeap(Heap h, File file) {
    if (h->size == h->capacity) {
        File *temp = realloc(h->files, h->capacity * 2 * sizeof(File));
        if (temp) {
            h->files = temp;
            h->capacity *= 2;
        }
    }

    h->files[h->size] = file;
    siftUp(h, h->size);
    h->size++;
}

File extractMax(Heap h) {
    File max;
    if (!h || !h->size)
        return NULL;

    max = h->files[0];
    h->files[0] = h->files[h->size - 1];
    h->size--;
    siftDown(h, 0);
    return max;
}

void freeHeap(Heap h) {
    if (h != NULL)
        free(h->files);
    free(h);
}
