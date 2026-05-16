#ifndef MULTITREE_H
#define MULTITREE_H

#include <stdio.h>

typedef struct list ListNode;
typedef struct file *File;

typedef struct tree {
    char *keyword;
    ListNode *first;
    // 26 + (chars between 'Z' and 'a' in ASCII) + 26
    struct tree *children[58];
} *Tree;

Tree addMultitree(Tree root, char *s, File file);
Tree removeRecursive(Tree root, char *s, int index, int l);
void deleteReferences(Tree root, char *keyword, char *name);
void printRecursive(FILE *fo, Tree root);

#endif