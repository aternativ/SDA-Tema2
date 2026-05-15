#include <stdio.h>
#include <stdlib.h>
#include "word.h"
#include "system.h"

#define INITIAL_SIZE 64

Word createWord() {
    Word word = malloc(sizeof(struct word));
    word->max_size = INITIAL_SIZE;
    word->size = 0;
    word->s = malloc(INITIAL_SIZE * sizeof(char));
    return word;
}

void resizeWord(Word word) {
    int max_size = word->max_size;
    char *temp = realloc(word->s, 2 * max_size);
    if (temp) {
        word->s = temp;
        word->max_size *= 2;
    }
    else
        fprintf(stderr, "Not enough memory for realloc.\n");
}

char *readWord(System system) {
    // we assume that the first char is at the cursor
    FILE *fi = system->fi;
    Word word = createWord();
    char *s = word->s;
    int c;
    // this also consumes the endline! don't fgetc in main
    while ((c = fgetc(fi) != '\n') && c != ' ' && c != EOF) {
        if (word->size + 1 >= word->max_size)
            resizeWord(word);
        s[word->size++] = c;
    }
    if (word->size + 1 >= word->max_size)
            resizeWord(word);
    s[word->size++] = '\0';
    free(word);
    return s;
}