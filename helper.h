#ifndef HELPER_H
#define HELPER_H

typedef struct list *List;

typedef struct file {
    char *name;
    int score;
    List keywords;
} *File;

#endif